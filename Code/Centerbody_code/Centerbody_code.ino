/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 Name:		Centerbody_code.ino
 Created:	3/29/2019 
 Author:	Petras Swissler

 Purpose:	Execute high-level behavior of FA3D robot

 Microcontroller: Atmega328PB
*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//-------------------------------------------------------------------------------------------------------------------------------------------------------
#include "includes_cb.h"
#ifndef CONFIG_CHECK
ERRORc 
#endif // !CONFIG_CHECK
// Function Definitions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void debugReports(				robot_centerbody* robot, uint8_t* msgArray);
char debugSerialMainLoopCheck(	robot_centerbody* robot, uint8_t* msgArray, IMUpn* imu, int16_t* temp_cal);
void setupDebugFunctions00(		robot_centerbody* robot, uint8_t* msgArray);
void setupDebugFunctions01(		robot_centerbody* robot, uint8_t* msgArray);
void setupGetFSRCal(			robot_centerbody* robot, uint8_t* msgArray);
void setupInitializeFSRCal(		robot_centerbody* robot, uint8_t* msgArray);
void setupPrintFSRCal(			robot_centerbody* robot, uint8_t* msgArray);
void restoreFSRCalib(			robot_centerbody* robot, int16_t* temp_cal);

#ifdef USE_EEPROM
	void setupLoadFSRCalibFromEEPROM(	robot_centerbody* robot);
#endif // USE_EEPROM


// Global Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Objects and Arrays
robot_centerbody robot;								// Robot object
IMUpn imu;											// IMU object
uint8_t msgArray[COM_I2C_MAX_MSG_LENGTH] = { 0 };	// Array for all messages

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// Main Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//-------------------------------------------------------------------------------------------------------------------------------------------------------
// the setup function runs once when you press reset or power the board
#include <avr\wdt.h>
void setup() 
{
	wdt_enable(WDTO_8S); wdt_reset(); MCUSR = 0; WDTCSR |= _BV(WDCE) | _BV(WDE); WDTCSR = 0; wdt_disable();
	setupDebugFunctions00(&robot, msgArray);

	// Different print statements based on robot orientation
#ifdef ORIENTATION_TOP
	Serial.println(F("Configured for topside locomotion."));
#endif
#ifdef ORIENTATION_SIDE
	Serial.println(F("Configured for sideways locomotion."));
#endif
#ifdef ORIENTATION_BELOW
	Serial.println(F("Configured for bottomside locomotion."));
#endif

	// Initialize The Robot
	robot_init(&robot, &imu, msgArray);
	shutdownAllDocks(&robot, msgArray);

	setupDebugFunctions01(&robot, msgArray);
}

// the loop function runs over and over again until power down or reset
void loop()
{
	// Variables
	int16_t remember_calibration_value = 0;			// For special 'Do not push any harder' behavior
#ifndef CONFIG_CHECK
	ERROR
#endif // !CONFIG_CHECK
#ifdef DEBUG
	char go = 1;									// debug
#endif // DEBUG
#ifndef DEBUG
	inputs(&robot,	&imu,	msgArray);
	logic(&robot,	msgArray);
#endif // !DEBUG
#ifdef DEBUG
	// Get the desired behavior 							
	shutdownAllDocks(&robot, msgArray);													 ///////////////////////////// MAIN FUNCTION LOOP ///////////////
	userInputNextStep(&robot, &imu);													 ///////////////////////////// MAIN FUNCTION LOOP ///////////////
	do																					 ///////////////////////////// MAIN FUNCTION LOOP ///////////////
	{																					 ///////////////////////////// MAIN FUNCTION LOOP ///////////////
		inputs(&robot, &imu, msgArray);													 ///////////////////////////// MAIN FUNCTION LOOP ///////////////
		go = puppetLogic(&robot, &imu, msgArray);										 ///////////////////////////// MAIN FUNCTION LOOP ///////////////
#ifdef SERIAL_REPORT																	 ///////////////////////////// MAIN FUNCTION LOOP ///////////////
		go *= debugSerialMainLoopCheck(&robot, msgArray, &imu, &remember_calibration_value);	///////////////////////////// MAIN FUNCTION LOOP ////////
		debugReports(&robot, msgArray);													 ///////////////////////////// MAIN FUNCTION LOOP ///////////////
#endif // SERIAL_REPORT																	 ///////////////////////////// MAIN FUNCTION LOOP ///////////////
	} while (go == 1);																	 ///////////////////////////// MAIN FUNCTION LOOP ///////////////
	if (remember_calibration_value != 0)
	{	restoreFSRCalib(&robot, &remember_calibration_value);	}
#endif // DEBUG																			 ///////////////////////////// MAIN FUNCTION LOOP ///////////////
}																						 ///////////////////////////// MAIN FUNCTION LOOP ///////////////

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// Secondary Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//-------------------------------------------------------------------------------------------------------------------------------------------------------
// Debug Reporting Functions ----------------------------------------------------------------------------------------------------------------------------
void setupDebugFunctions00(		robot_centerbody* robot, uint8_t* msgArray)
{
#ifdef DEBUG
	Serial.begin(SERIAL_BAUD_RATE);
	Serial.println(F("ON"));
#endif // DEBUG
}

void setupDebugFunctions01(		robot_centerbody* robot, uint8_t* msgArray)
{
	// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#ifdef SERIAL_REPORT	
#endif
	// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#ifdef FSR_ADC_REPORT_ONLY
	while (1) 
	{
		Serial.print(F("A:")); Serial.print(analogRead(PIN_ASEN_FSR_A)); Serial.print(F("\t"));
		Serial.print(F("B:")); Serial.print(analogRead(PIN_ASEN_FSR_B)); Serial.println();
	}
#endif
	// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#ifdef ONLY_REPORT_SENSOR_READINGS
	Serial.begin(SERIAL_BAUD_RATE);
	float flipspeed = 0;
	float maxflipspeed = 0;
	float fsx;
	float fsy;
	float fsz;
	while (1)
	{
		delay(5);
		if (imu.gyroAvailable())
		{
			imu.readGyro();
			fsx = 0.95 * fsx + 0.05 * imu.gx;
			fsy = 0.95 * fsy + 0.05 * imu.gy;
			fsz = 0.95 * fsz + 0.05 * imu.gz;
			flipspeed = sqrt(fsx * fsx + fsy * fsy + fsz * fsz);

			maxflipspeed = max(flipspeed, maxflipspeed * 0.999);
			Serial.print(flipspeed, 6); Serial.print(F("\t"));
			for (float ii = 0; ii < 1; ii += 0.05)
			{
				if (ii <= (flipspeed / maxflipspeed))
				{
					Serial.print(F("#"));
				}
			}
			Serial.println();
		}
#ifdef IMU_ENABLE
		delay(100);
		if (imu.gyroAvailable())
		{	imu.readGyro();		}
		if (imu.accelAvailable())
		{	imu.readAccel();	}
		if (imu.magAvailable())
		{	imu.readMag();		}

		Serial.print(F("Accel:  ")); Serial.print(imu.ax); Serial.print(F("\t")); Serial.print(imu.ay); Serial.print(F("\t")); Serial.print(imu.az); Serial.println();
		Serial.print(F("Gyro:   ")); Serial.print(imu.gx); Serial.print(F("\t")); Serial.print(imu.gy); Serial.print(F("\t")); Serial.print(imu.gz); Serial.println();
		Serial.print(F("Mag:    ")); Serial.print(imu.mx); Serial.print(F("\t")); Serial.print(imu.my); Serial.print(F("\t")); Serial.print(imu.mz); Serial.println();
		Serial.print(F("FSR 0:  ")); Serial.print(analogRead(PIN_ASEN_FSR_A)); Serial.println();
		Serial.print(F("FSR 1:  ")); Serial.print(analogRead(PIN_ASEN_FSR_B)); Serial.println();
		Serial.println(); Serial.println(); Serial.println();
#endif // !IMU_ENABLE
	}
#endif
	// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#ifdef DEBUG
	debug_start(robot); // debug
	Serial.print(F("cf")); Serial.println(robot->calib_fsr); // debug

#ifdef WAIT_FOR_KEYPRESS_IN_STARTUP
	clearSerialBuffer();
	while (!Serial.available())
	{
		delay(750);
		Serial.print(F("."));
	}
#endif // WAIT_FOR_KEYPRESS_IN_STARTUP

	clearSerialBuffer();												
	robot->step_direction = MOTORDIR_REV;
	shutdownAllDocks(robot, msgArray);
#endif // DEBUG
	// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

#ifdef FLASH_LED_ONLY
	while (1)
	{
		inputs(&robot, &imu, msgArray);
		//robot.meas_grav_x += 0.07;
		//robot.meas_grav_y
		//robot.meas_grav_z -= 0.07;
		uint8_t hiDock = findHightestDock(&robot);
		debug_flashDockLED(findHightestDock(&robot), msgArray, 1, 50);
		Serial.print(hiDock); Serial.print(F("\t"));
		Serial.print(robot.meas_grav_x); Serial.print(F(" "));
		Serial.print(robot.meas_grav_y); Serial.print(F(" "));
		Serial.print(robot.meas_grav_z); Serial.print(F(" "));
		Serial.println();
		delay(50);
	}
#endif

#ifdef WAIT_FOR_FLIP
	// Wait for Inversion to start the program 
	measureOrientation(&robot, &imu);						// todo: move this into a single function
	float initialYSign = getFloatSign(robot.meas_grav_y);
	while (getFloatSign(robot.meas_grav_y) == initialYSign)
	{
		delay(100);
		measureOrientation(&robot, &imu);
	}
	delay(5000); calibrateFSR(&robot);
	// Then Start the Program								// debug
#endif // WAIT_FOR_FLIP

#ifdef IDENTIFY_DOCKS
	debug_identifyDocks(msgArray);
#endif

#ifdef TESTING_BLOCK
#endif

#ifdef DEBUG_GET_FSR_CAL
	setupGetFSRCal(robot, msgArray);
#endif // DEBUG_GET_FSR_CAL
}

void debugReports(				robot_centerbody* robot, uint8_t* msgArray)
{
	// Debug Prints // debug stuff
	Serial.print(robot->time_this_measurement);	Serial.print(F(","));
	Serial.print(robot->meas_fsr_force, 2);		Serial.print(F("|")); Serial.print(robot->desired_force,2); Serial.print(F("\t"));

	Serial.print(F(" a")); Serial.print(robot->action_dock		- DOCK_0);	Serial.print(F(":"));
	Serial.print(robot->dockState_action);	Serial.print(F(","));
	Serial.print(robot->dockPhase_action);	Serial.print(F("\t"));

	Serial.print(F(" m")); Serial.print(robot->movement_dock	- DOCK_0);	Serial.print(F(":"));
	Serial.print(robot->dockState_move);		Serial.print(F(","));
	Serial.print(robot->dockPhase_move);		Serial.print(F("\t"));

	Serial.print(F(" p")); Serial.print(robot->passive_dock	- DOCK_0);	Serial.print(F(":"));
	Serial.print(robot->dockState_passive);	Serial.print(F(","));
	Serial.print(robot->dockPhase_passive);	Serial.print(F("\t"));

	Serial.print(F("i"));
	Serial.print(getDockInformation(robot, robot->action_dock, I2C_SEND_IDOCK, msgArray), 2); Serial.print(F(","));
	Serial.print(getDockInformation(robot, robot->action_dock, I2C_SEND_IHOOP, msgArray), 4); Serial.print(F("\t"));

	Serial.print(F("ic"));
	Serial.print(getDockInformation(robot, robot->action_dock, I2C_SEND_INT_I, msgArray));

#ifdef IMU_ENABLE
	Serial.print(F("\t{")); 
	Serial.print(robot->meas_grav_x, 3);	Serial.print(F(","));
	Serial.print(robot->meas_grav_y, 3);	Serial.print(F(","));
	Serial.print(robot->meas_grav_z, 3);	Serial.print(F("}\t{"));
	Serial.print(robot->meas_mag_x,  3);	Serial.print(F(","));
	Serial.print(robot->meas_mag_y,  3);	Serial.print(F(","));
	Serial.print(robot->meas_mag_z,  3);	Serial.print(F("}\t{"));
	Serial.print(robot->calc_flip_speed,3);	Serial.print(F(","));
	Serial.print(findHightestDock(robot));	Serial.print(F(","));
	Serial.print(findLowestDock(robot));	Serial.print(F("}"));

#endif // !IMU_ENABLE

	if (robot->pup_mode == 's')
	{
		Serial.print(robot->astep_direction);
	}
	Serial.println();
}

char debugSerialMainLoopCheck(	robot_centerbody* robot, uint8_t* msgArray, IMUpn* imu, int16_t* temp_cal)
{
	char go = 1;
	// Manual interruption								todo: find some way of specifying this to the robot without serial comm
	if (Serial.available())								// debug
	{
		char interruptedInput = Serial.read();			// debug
		if (interruptedInput == 'x')	{	go = 0;	Serial.println(("x"));	}
		if (interruptedInput == 'c')	{	calibrateFSR(robot);			}

#ifdef IMU_ENABLE
		if (interruptedInput == 'g') { calibrateGyro(robot, imu); }
#endif // IMU_ENABLE


		// Special behavior for information reporting
		if (robot->pup_mode == 'i')
		{
			if (interruptedInput == '0')	robot->movement_dock = DOCK_0;
			if (interruptedInput == '1')	robot->movement_dock = DOCK_1;
			if (interruptedInput == '2')	robot->movement_dock = DOCK_2;
			if (interruptedInput == '3')	robot->movement_dock = 0;
			if (interruptedInput == 'f')	robot->step_direction = MOTORDIR_FWD;
			if (interruptedInput == 'r')	robot->step_direction = MOTORDIR_REV;

#ifdef IMU_ENABLE
			if (interruptedInput == 'g') { calibrateGyro(robot, imu); }
#endif // IMU_ENABLE
		}

		// Special behavior for auto step (escape unfavorable conditions)
		if (robot->pup_mode == 's')
		{
			// Skip forward
			if (interruptedInput == ']')
			{
				robot->astep_phase = (robot->astep_phase - (robot->astep_phase % 10)) + 10;
				astep_next_phase(robot, msgArray, robot->astep_phase);
				Serial.print(F("\r\nSkipped Forward\r\n"));
			}
			// Skip backward
			if (interruptedInput == '[')		
			{
				if (robot->astep_phase == AUTOSTEP_120_ATTACH_ON_SELECTED_DOCK)
				{
					robot->astep_phase = AUTOSTEP_090_BACK_OFF_AFTER_CONTACT;
					astep_next_phase(robot, msgArray, robot->astep_phase);
				}
				else
				{
					shutdownAllDocks(robot, msgArray);
					robot->astep_phase = (robot->astep_phase - (robot->astep_phase % 10)) - 10;
					astep_next_phase(robot, msgArray, robot->astep_phase);
				}
				Serial.print(F("\r\nSkipped Backwards\r\n"));
			}
			// Temporary recalibration
			if (interruptedInput == 't')
			{
				Serial.print(F("Temp Recal:"));
				Serial.print(robot->astep_dock_move - DOCK_0); Serial.print(F(":"));
				switch(robot->astep_dock_move)
				{
				case DOCK_0:
					switch (robot->astep_direction)
					{
					case MOTORDIR_FWD:
						*temp_cal = robot->fsr_cal_0f;
						robot->fsr_cal_0f = robot->meas_fsr_force * 0.95;
						Serial.print(robot->fsr_cal_0f);
						break;
					case MOTORDIR_REV:
						*temp_cal = robot->fsr_cal_0r;
						robot->fsr_cal_0r = robot->meas_fsr_force * 0.95;
						Serial.print(robot->fsr_cal_0r);
						break;
					}
					break;
				case DOCK_1:
					switch (robot->astep_direction)
					{
					case MOTORDIR_FWD:
						*temp_cal = robot->fsr_cal_1f;
						robot->fsr_cal_1f = robot->meas_fsr_force * 0.95;
						Serial.print(robot->fsr_cal_1f);
						break;
					case MOTORDIR_REV:
						*temp_cal = robot->fsr_cal_1r;
						robot->fsr_cal_1r = robot->meas_fsr_force * 0.95;
						Serial.print(robot->fsr_cal_1r);
						break;
					}
					break;
				case DOCK_2:
					switch (robot->astep_direction)
					{
					case MOTORDIR_FWD:
						*temp_cal = robot->fsr_cal_2f;
						robot->fsr_cal_2f = robot->meas_fsr_force * 0.95;
						Serial.print(robot->fsr_cal_2f);
						break;
					case MOTORDIR_REV:
						*temp_cal = robot->fsr_cal_2r;
						robot->fsr_cal_2r = robot->meas_fsr_force * 0.95;
						Serial.print(robot->fsr_cal_2r);
						break;
					}
					break;
				}
				Serial.println();				
			}
		}
		// There are no instances where we care about more than one keypress
		clearSerialBuffer();
	}
	return go;
}

void restoreFSRCalib(			robot_centerbody* robot, int16_t* temp_cal)
{
	switch (robot->astep_dock_move)
	{
	case DOCK_0:
		switch (robot->astep_direction)
		{
		case MOTORDIR_FWD:
			robot->fsr_cal_0f = *temp_cal;
			break;
		case MOTORDIR_REV:
			robot->fsr_cal_0r = *temp_cal;
			break;
		}
		break;
	case DOCK_1:
		switch (robot->astep_direction)
		{
		case MOTORDIR_FWD:
			robot->fsr_cal_1f = *temp_cal;
			break;
		case MOTORDIR_REV:
			robot->fsr_cal_1r = *temp_cal;
			break;
		}
		break;
	case DOCK_2:
		switch (robot->astep_direction)
		{
		case MOTORDIR_FWD:
			robot->fsr_cal_2f = *temp_cal;
			break;
		case MOTORDIR_REV:
			robot->fsr_cal_2r = *temp_cal;
			break;
		}
		break;
	}
	*temp_cal = 0;
	Serial.println("Calibration Values Restored");
}

void setupGetFSRCal(			robot_centerbody* robot, uint8_t* msgArray)
{
	setupInitializeFSRCal(robot, msgArray);
	setupPrintFSRCal(robot, msgArray);
	
	// Input Calibration Values if Desired
	Serial.println(F("Input Calibration Values? (y/n)"));
	if (checkYN(20))
	{
#ifdef USE_EEPROM
		Serial.print(F("0f: ")); writeFloatToEEPROM(	getSerialFloat(100, SERIAL_INPUT_ECHO_ON), EEPROM_ADDRESS_FSR_CAL_0f_B0); Serial.println();
		Serial.print(F("0r: ")); writeFloatToEEPROM(	getSerialFloat(100, SERIAL_INPUT_ECHO_ON), EEPROM_ADDRESS_FSR_CAL_0r_B0); Serial.println();
		Serial.print(F("1f: ")); writeFloatToEEPROM(	getSerialFloat(100, SERIAL_INPUT_ECHO_ON), EEPROM_ADDRESS_FSR_CAL_1f_B0); Serial.println();
		Serial.print(F("1r: ")); writeFloatToEEPROM(	getSerialFloat(100, SERIAL_INPUT_ECHO_ON), EEPROM_ADDRESS_FSR_CAL_1r_B0); Serial.println();
		Serial.print(F("2f: ")); writeFloatToEEPROM(	getSerialFloat(100, SERIAL_INPUT_ECHO_ON), EEPROM_ADDRESS_FSR_CAL_2f_B0); Serial.println();
		Serial.print(F("2r: ")); writeFloatToEEPROM(	getSerialFloat(100, SERIAL_INPUT_ECHO_ON), EEPROM_ADDRESS_FSR_CAL_2r_B0); Serial.println();
		setupLoadFSRCalibFromEEPROM(robot);
#else
		Serial.print(F("0f: ")); robot->fsr_cal_0f = getSerialFloat(100, SERIAL_INPUT_ECHO_ON); Serial.println();
		Serial.print(F("0r: ")); robot->fsr_cal_0r = getSerialFloat(100, SERIAL_INPUT_ECHO_ON); Serial.println();
		Serial.print(F("1f: ")); robot->fsr_cal_1f = getSerialFloat(100, SERIAL_INPUT_ECHO_ON); Serial.println();
		Serial.print(F("1r: ")); robot->fsr_cal_1r = getSerialFloat(100, SERIAL_INPUT_ECHO_ON); Serial.println();
		Serial.print(F("2f: ")); robot->fsr_cal_2f = getSerialFloat(100, SERIAL_INPUT_ECHO_ON); Serial.println();
		Serial.print(F("2r: ")); robot->fsr_cal_2r = getSerialFloat(100, SERIAL_INPUT_ECHO_ON); Serial.println();
#endif
	}
	else
	{
#ifdef USE_EEPROM
		Serial.println(F("Calibration Skipped. Using existing values from EEPROM"));
#else
		Serial.println(F("Calibration Skipped. Using programmed values"));
#endif
	}

	// Print out all the calibration values
	setupPrintFSRCal(robot, msgArray);
}

void setupInitializeFSRCal(		robot_centerbody* robot, uint8_t* msgArray)
{
#ifdef USE_EEPROM
	setupLoadFSRCalibFromEEPROM(robot);
#else USE_EEPROM
	robot->fsr_cal_0f = 65;
	robot->fsr_cal_0r = 170;
	robot->fsr_cal_1f = 75;
	robot->fsr_cal_1r = 142;
	robot->fsr_cal_2f = 110;
	robot->fsr_cal_2r = 186;
#endif // !else USE_EEPROM
}

void setupPrintFSRCal(			robot_centerbody* robot, uint8_t* msgArray)
{
	Serial.println(F("0f\t0r\t1f\t1r\t2f\t2r\t"));
	Serial.print(robot->fsr_cal_0f); Serial.print("\t");
	Serial.print(robot->fsr_cal_0r); Serial.print("\t");
	Serial.print(robot->fsr_cal_1f); Serial.print("\t");
	Serial.print(robot->fsr_cal_1r); Serial.print("\t");
	Serial.print(robot->fsr_cal_2f); Serial.print("\t");
	Serial.print(robot->fsr_cal_2r); Serial.print("\t");
	Serial.println();
}

#ifdef USE_EEPROM
void setupLoadFSRCalibFromEEPROM(robot_centerbody* robot)
{
	robot->fsr_cal_0f = readFloatFromEEPROM(EEPROM_ADDRESS_FSR_CAL_0f_B0);
	robot->fsr_cal_0r = readFloatFromEEPROM(EEPROM_ADDRESS_FSR_CAL_0r_B0);
	robot->fsr_cal_1f = readFloatFromEEPROM(EEPROM_ADDRESS_FSR_CAL_1f_B0);
	robot->fsr_cal_1r = readFloatFromEEPROM(EEPROM_ADDRESS_FSR_CAL_1r_B0);
	robot->fsr_cal_2f = readFloatFromEEPROM(EEPROM_ADDRESS_FSR_CAL_2f_B0);
	robot->fsr_cal_2r = readFloatFromEEPROM(EEPROM_ADDRESS_FSR_CAL_2r_B0);
}
#endif