/*///////////////////////////////////////////////////////////////////////
 Name:		startup.h / .cpp
 Created:	3/30/2019
 Author:	Petras Swissler

 Purpose:
*////////////////////////////////////////////////////////////////////////
#ifndef _INPUTS_h
#define _INPUTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
/////////////////////////////////////////////////////////////////////////
#include "def_names.h"
#include "config_centerbody.h"
// Include Libraries
//#include <SparkFunLSM6DS3.h>
//#include <SparkFunLSM9DS1.h>
//#include "Narcoleptic.h"

#include "robot_centerbody.h"

#include "def_commInterpretation.h"
#include "def_friendly_names.h"
#include "def_calib_cb.h"
#include "def_pins_cb.h"

#include "simple_helper_functions.h"
#include "communications_cb.h"
#include "vectorMathFunctions.h"

// Low-Level Functions
void getFSRCalib(robot_centerbody*);
void calibrateGyro(robot_centerbody* robot, IMUpn* imu);
void measureFSR(robot_centerbody* );
void calibrateFSR(robot_centerbody*);
void measureOrientation(robot_centerbody* , IMUpn* );
void measureFlipSpeed(robot_centerbody* , IMUpn* );
void measureHeading(robot_centerbody* robot, IMUpn* imu);
void getDockStatesAndPhases(robot_centerbody* robot, uint8_t* msgArray);
float adjustFSRVals(robot_centerbody* robot);
void updateMeasurementTimes(robot_centerbody* robot);

// Mid-Level Functions
uint8_t findClosestDockToDesiredDirection(float dir_x, float dir_y, float dir_z, robot_centerbody* robot);
uint8_t findFurthestDockToDesiredDirection(float dir_x, float dir_y, float dir_z, robot_centerbody* robot);
uint8_t findHightestDock(robot_centerbody* robot);
uint8_t findLowestDock	(robot_centerbody* robot);


// High-Level Functions
void inputs(robot_centerbody*, IMUpn*, uint8_t*);
/////////////////////////////////////////////////////////////////////////
#endif

