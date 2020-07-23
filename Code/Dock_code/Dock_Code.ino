/*///////////////////////////////////////////////////////////////////////
 Name:		Dock_Code.ino
 Created:	3/25/2019 
 Author:	Petras Swissler

 Purpose:	Perform all functions necessary for the
			dock to move, attach etc.
*////////////////////////////////////////////////////////////////////////
#include <avr/wdt.h>
#include "config_dock.h"
#ifdef ENABLE_USE_OF_EEPROM
#include <EEPROM.h>
#endif // ENABLE_USE_OF_EEPROM
// Include libraries ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "def_includes.h"
// DEBUG STUFF
#include "calibration.h"

// Small Function Prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void conditionalBeginFunctions();
void conditionalLoopFunctions();

// Global variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Object to contain all robot state information etc
dock_robot robot;

// I2C Message Variables
uint8_t flag_receive;
uint8_t flag_request;
uint8_t msg_in[	COM_I2C_CENTER_OUT_LENGTH]	= { 0 };
uint8_t msg2[	COM_I2C_CENTER_OUT_LENGTH]	= { 0 };

// Global Message Variables
unsigned long dd_comm_array[COM_DD_MSG_LENGTH] = { 0 };
uint8_t dd_comm_index = 0;

// Main Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// the setup function runs once when you press reset or power the board
void setup() 
{
	// Disable watchdog ------------------
	wdt_reset(); MCUSR = 0; WDTCSR |= _BV(WDCE) | _BV(WDE); WDTCSR = 0;
	conditionalBeginFunctions();

	// Initialization --------------------
	robot_startup(&robot);
	shutdown_all(&robot);

	// Configure I2C communication--------
#ifdef USE_HARDCODED_I2C_ADDR
	robot.self_ID =  HARDCODED_I2C_ADDRESS;
#endif // HARDCODED_I2C_ADDR
		
	Wire.begin(robot.self_ID);
	Wire.setClock(10000);
	Wire.onReceive(receiveEvent);
	Wire.onRequest(requestEvent);

#ifdef USE_WATCHDOG_TIMER
	wdt_enable(WDTO_8S);
#endif // USE_WATCHDOG_TIMER
}

// Interrupt handlers ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void receiveEvent(int numbytes)
{
#ifdef USE_WATCHDOG_TIMER
	wdt_reset();
#endif // USE_WATCHDOG_TIMER

	// Set the flag to true
	flag_receive = true;
	// Interpretation must be done here to avoid overlapping requests
	// Interpretation has minimal cost
	read_i2c_Data(msg_in, numbytes);
	interpret_i2c_Data(msg_in, &robot);
	clearArray(msg_in, COM_I2C_CENTER_OUT_LENGTH);
}

void requestEvent(void)		
{
#ifdef USE_WATCHDOG_TIMER
	wdt_reset();
#endif // USE_WATCHDOG_TIMER

	flag_request = true;
	sendRequestedData_i2c(&robot,msg2);
}

// the loop function runs over and over again until power down or reset
void loop() 
{
	// Measure Inputs
	measureSensors(&robot);	
	// Handle any communications
	handle_i2c_communications(&robot, &flag_request, &flag_receive, msg_in);
	handleDockCommunications(&robot, dd_comm_array, &dd_comm_index); // todo: implement 
	// Based on communications and measurements, decide what to do
	runLogic(&robot);
	// Execute outputs safely
	executeOutputs(&robot);

	// *************************************************************************
	// Conditional functions using preprocessor blocks
	conditionalLoopFunctions();
}

void conditionalBeginFunctions(void)
{
#ifdef LED_FLASH_ONLY
	boardConfig();
	while (1)
	{
		ledON();		delay(1000);		ledOFF();		delay(1000);
	}
#endif // LED_FLASH_ONLY

#ifdef SERIAL_ENABLE
	Serial.begin(38400); // DEBUG
#endif // SERIAL_ENABLE

	// Report Calibration Data -----------
#ifdef REPORT_CALIBRATION_DATA
	Serial.print("CS DO HI:  ");	Serial.println(readFloatFromEEPROM(EEPROM_POSITION_CS_DO_HI_A_PER_ADC), 8);
	Serial.print("CS DIO LO: ");	Serial.println(readFloatFromEEPROM(EEPROM_POSITION_CS_DIO_LO_A_PER_ADC), 8);
	Serial.print("CS HI LO:	 ");	Serial.println(readFloatFromEEPROM(EEPROM_POSITION_CS_HI_LO_A_PER_ADC), 8);
	Serial.print("VBat(-):   ");	Serial.println(readFloatFromEEPROM(EEPROM_POSITION_VBATMINUS_PER_ADC), 8);
	Serial.print("Vdock:     ");	Serial.println(readFloatFromEEPROM(EEPROM_POSITION_VDOCK_PER_ADC), 8);
	Serial.print("ThpSwp:    ");	Serial.println(readFloatFromEEPROM(EEPROM_POSITION_TIME_FOR_HOOP_SWEEP), 8);
	Serial.print("TarmSwp:   ");	Serial.println(readFloatFromEEPROM(EEPROM_POSITION_TIME_FOR_ARM_MOVE), 8);
	Serial.print("MinHpPWM:  ");	Serial.println(readFloatFromEEPROM(EEPROM_POSITION_MIN_HOOP_MOVE_PWM), 8);
	Serial.print("MinArmPWM: ");	Serial.println(readFloatFromEEPROM(EEPROM_POSITION_MIN_ARM_MOVE_PWM), 8);
	Serial.println(robot.self_ID);
	delay(500);
#endif // REPORT_CALIBRATION_DATA
}

void conditionalLoopFunctions(void)
{
#ifdef USE_WATCHDOG_TIMER
	wdt_reset();
#endif // USE_WATCHDOG_TIMER

#ifdef SERIAL_ENABLE
	Serial.print(robot.command_behavior);
	Serial.print(",");
	Serial.print(robot.state_phase); Serial.print("\t");
	Serial.print(analogRead(PIN_ASEN_IHOOP_IN_LOW));	Serial.print("\t");
	Serial.print(analogRead(PIN_ASEN_IDOCK_INOUT_LOW));	Serial.print("\t");
	Serial.print(analogRead(PIN_ASEN_IDOCK_OUT_HIGH));	Serial.print("\t");
	Serial.print(robot.meas_current_dock); Serial.print("\t");
	Serial.print(robot.calc_integrated_current);
	Serial.println();
#endif // SERIAL_ENABLE
}