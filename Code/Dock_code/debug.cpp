/*///////////////////////////////////////////////////
Name:		debug.h / .cpp
Created :	4/ 02 / 2019
Author :	Petras Swissler

Purpose :	Define debug behavior to validate
			low-level code and hardware
*/////////////////////////////////////////////////////
#include "debug.h"

//////////////////////////////////////////////////////
// Main Function
void runDebug(dock_robot* robot)
{
	reportSensorData(robot);
	char command = waitForKeypress();
	debugLogic(robot, command);
}

//////////////////////////////////////////////////////
// Helper Functions
void dangerCode(dock_robot* robot, uint8_t option)
{
	switch (option)
	{

	}
}

/*char waitForKeypress( void ) 
{
	char symbolIn = 0;

	
	ln(DEBUG_MESSAGE_STRING1);
	
	ln(DEBUG_MESSAGE_STRING2);
	// Wait until have serial data available
	while (Serial.available() == 0) 
	{
		// Do nothing
	}

	symbolIn = Serial.read();

	// Clear the serial buffer
	delay(5);
	while (Serial.available()) 
	{
		Serial.read();
	}

	return symbolIn;
}*/

void reportSensorData(dock_robot* robot)
{
	/*Serial.print("T:  ");			*/Serial.print(robot->time_last_measurement);		Serial.print(" "); Serial.print(robot->command_behavior); Serial.print("-"); Serial.print(robot->state_phase); Serial.println(); Serial.print("\t");
	/*Serial.print("IDK:");			*/Serial.print(robot->meas_current_dock,4); 		Serial.print("\t");
	/*Serial.print("IHP:");			*/Serial.print(robot->meas_current_hoop,4); 		Serial.print("\t");
	/*Serial.print("VB-:");			*/Serial.print(robot->meas_vbat_negative);		Serial.print("\t");
	/*Serial.print("VD: ");			*/Serial.print(robot->meas_dock_voltage); 		Serial.print("\t");
	/*Serial.print("A_L:");			*/Serial.print(robot->sen_armLim);				Serial.print("\t");
	/*Serial.print("LDK:");			*/Serial.print(robot->sen_dockLevel);				Serial.print("\t");
	/*Serial.print("LHP:");			*/Serial.print(robot->sen_hoop_level);				Serial.print("\t");
	/*Serial.print("CAP:");			*/Serial.print(robot->calc_approx_arm_position);	Serial.print("\t");
	/*Serial.print("CHP:");			*/Serial.print(robot->calc_approx_hoop_position);	Serial.print("\t");
	Serial.print(robot->calc_integrated_current);
	Serial.println();
}

void reportRawADCData(dock_robot* robot)
{
	///*Serial.print("IDOH:");		*/Serial.print(analogRead(PIN_ASEN_IDOCK_OUT_HIGH));Serial.print("\t");
	/*Serial.print("IDOL:");		*/Serial.print(analogRead(PIN_ASEN_IDOCK_INOUT_LOW)); Serial.print("\t");
	///*Serial.print("IDIL:");		*/Serial.print(analogRead(PIN_ASEN_IDOCK_IN_LOW));  Serial.print("\t");
	/*Serial.print("IHIL:");		*/Serial.print(analogRead(PIN_ASEN_IHOOP_IN_LOW));  Serial.print("\t");
	Serial.print(robot->self_ID);  Serial.print("\t");// No vb measurement
	/*Serial.print("VDK: ");		*/Serial.print(analogRead(PIN_ASEN_VDOCK));		  Serial.print("\t");
	/*Serial.print("ALIM:");		*/Serial.print(digitalRead(PIN_DI_ARMLIMIT));		  Serial.print("\t");
	/*Serial.print("DLEV:");		*/Serial.print(digitalRead(PIN_DI_DOCKLEVEL));	  Serial.print("\t");
	/*Serial.print("HLEV:");		*/Serial.print(digitalRead(PIN_DI_HOOPLEVEL));	  
}

void debugLogic(dock_robot* robot, char command)
{
	switch (command)
	{
	case 'q':
		robot->out_melt_dock_voltage = 18;
		robot->out_max_melt_voltage = 40;
		robot->out_melt_dock_current_limit = 1;
		robot->out_dock_state = HBMELT_HI_V;
		break;
	case ',':
		robot->out_melt_dock_voltage -= 1;
		break;
	case '.':
		robot->out_melt_dock_voltage += 1;
		break;
	case 'w':
		robot->out_melt_dock_voltage = 0;
		robot->out_max_melt_voltage = 0;
		robot->out_melt_dock_current_limit = 0;
		robot->out_dock_state = HBMELT_GND;
		break;
	case 'e':
		robot->out_melt_dock_voltage = 0;
		robot->out_max_melt_voltage = 0;
		robot->out_melt_dock_current_limit = 0;
		robot->out_dock_state = HBMELT_HI_Z;
		break;
		//'a: M1 forward ; s: M1 reverse ; d: M1 brake ; f: M1 coast | z: M2 forward ; x: M2 reverse ; c: M2 brake ; v: M2 coast'
	case 'a':
		robot->out_hoop_direction = MOTORDIR_FWD;
		robot->out_hoop_power = 0.5;
		break;
	case 's':
		robot->out_hoop_direction = MOTORDIR_REV;
		robot->out_hoop_power = 0.5;
		break;
	case 'd':
		robot->out_hoop_direction = MOTORDIR_BRAKE;
		robot->out_hoop_power = 0;
		break;
	case 'f':
		robot->out_hoop_direction = MOTORDIR_IDLE;
		robot->out_hoop_power = 0;
		break;
	case 'z':
		robot->out_arm_direction = MOTORDIR_FWD;
		robot->out_arm_power = 0.5;
		break;
	case 'x':
		robot->out_arm_direction = MOTORDIR_REV;
		robot->out_arm_power = 0.5;
		break;
	case 'c':
		robot->out_arm_direction = MOTORDIR_BRAKE;
		robot->out_arm_power = 0;
		break;
	case 'v':
		robot->out_arm_direction = MOTORDIR_IDLE;
		robot->out_arm_power = 0;
		break;
	case '8':
		digitalWrite(PIN_DO_ENABLE_SEN_VBAT_MINUS, HIGH);
		break;
	case '9':
		digitalWrite(PIN_DO_ENABLE_SEN_VBAT_MINUS, LOW);
		break;
	default:
		shutdown_all(robot);
		break;
	}
}

void cycleMotors(void)
{
	hw_hb_lv8548mc(MOTORDIR_FWD, 1.0, PIN_PWM_HOOP_MOTOR_A, PIN_PWM_HOOP_MOTOR_B);
	delay(1000);
	hw_hb_lv8548mc(MOTORDIR_REV, 1.0, PIN_PWM_HOOP_MOTOR_A, PIN_PWM_HOOP_MOTOR_B);
	delay(1000);
	hw_hb_lv8548mc(MOTORDIR_BRAKE, 0.0, PIN_PWM_HOOP_MOTOR_A, PIN_PWM_HOOP_MOTOR_B);
	delay(1000);
	// All low-level implementation of the h-bridge 
	// control is handled by the h-bridge function
	hw_hb_lv8548mc(MOTORDIR_FWD, 1.0, PIN_PWM_ARM_MOTOR_A, PIN_PWM_ARM_MOTOR_B);
	delay(1000);
	hw_hb_lv8548mc(MOTORDIR_REV, 1.0, PIN_PWM_ARM_MOTOR_A, PIN_PWM_ARM_MOTOR_B);
	delay(1000);
	hw_hb_lv8548mc(MOTORDIR_BRAKE, 0.0, PIN_PWM_ARM_MOTOR_A, PIN_PWM_ARM_MOTOR_B);
	delay(1000);
}

void cycleMeltHB(void)
{
	// Melt On Full
	digitalWrite(PIN_PWM_DOCK_MELT_HOOP, HIGH);
	digitalWrite(PIN_PWM_DOCK_MELT_DOCK, LOW);
	//hw_hb_drv8871ddar(HBMELT_HI_V, 0.5, PIN_PWM_DOCK_MELT_DOCK, PIN_PWM_DOCK_MELT_HOOP);
	delay(10000);
	// Melt Off
	hw_hb_drv8871ddar(HBMELT_GND, 0);
	delay(500);
	// High Z
	hw_hb_drv8871ddar(HBMELT_HI_Z, 0);
	delay(500);
}