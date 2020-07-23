/*///////////////////////////////////////////////////
Name:		calibration .h / .cpp
Created :	04 / 23 / 2019
Author :	Petras Swissler

Purpose :	Functions to allow calibration of the
			Various analog sensors on-board
*/////////////////////////////////////////////////////
#include "calibration.h"

// Main Function -------------------------------------
void calibration_report(dock_robot* robot)
{
	String titleString = "Calibration Report For FA3D Sensors:";
	uint8_t reportWidth = 50;
	printHeader(titleString, reportWidth, 0);
	/*xx*/
	("\t");
	/*
	int("Dock CS, Low, In:  "); reportADC(PIN_ASEN_IDOCK_IN_LOW);
	/*xx*/Serial.print("Dock CS, Low, Out: "); reportADC(PIN_ASEN_IDOCK_INOUT_LOW);
	/*xx*/Serial.print("Dock CS, High, Out:"); reportADC(PIN_ASEN_IDOCK_OUT_HIGH);
	/*xx*/Serial.print("Hoop CS, Low, In:  "); reportADC(PIN_ASEN_IHOOP_IN_LOW);
	
	/*xx*/Serial.print("Dock Voltage ADC:  "); reportADC(PIN_ASEN_VDOCK);
	
	/*xx*/Serial.print("Arm Position:      \t"); /*xx*/Serial.println(robot->calc_approx_arm_position);
	/*xx*/Serial.print("\tHoop Position:     \t"); /*xx*/Serial.println(robot->calc_approx_hoop_position);
	printFooter(reportWidth, 0);

	moveHoop_oneQuarter(robot);
	moveArm_oneQuarter(robot);
	robot->out_dock_state = HBMELT_HI_Z;
}

// Helper Functions ----------------------------------
void reportADC(uint8_t adcPin)
{

	/*xx*/Serial.print("\t");
	/*xx*/Serial.print(analogRead(adcPin));
	/*xx*/Serial.println();
	/*xx*/Serial.print("\t");
}

void moveHoop_oneQuarter(dock_robot* robot)
{
	robot->out_hoop_power = 1.0;
	if (robot->debug_uint8_1 == 0)
	{
		if (robot->calc_approx_hoop_position < 0.25) { robot->out_hoop_direction = MOTORDIR_FWD; }
		else { robot->debug_uint8_1 = 1; }
	}
	else
	{
		if (robot->calc_approx_hoop_position > 0) { robot->out_hoop_direction = MOTORDIR_REV; }
		else { robot->debug_uint8_1 = 0; }
	}
}

void moveArm_oneQuarter(dock_robot* robot)
{
	robot->out_arm_power = 1.0;
	if (robot->debug_uint8_2 == 0)
	{
		/*xx*/Serial.println("ARM_FWD");
		if (robot->calc_approx_arm_position < 0.25) { robot->out_arm_direction = MOTORDIR_FWD; }
		else { robot->debug_uint8_2 = 1; }
	}
	else
	{
		/*xx*/Serial.println("ARM_REV");
		if (robot->calc_approx_arm_position > 0) { robot->out_arm_direction = MOTORDIR_REV; }
		else { robot->debug_uint8_2 = 0; }
	}
}