/*///////////////////////////////////////////////////
Name:		calibration .h / .cpp
Created :	04 / 23 / 2019
Author :	Petras Swissler

Purpose :	Functions to allow calibration of the 
			Various analog sensors on-board
*/////////////////////////////////////////////////////
// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef _CALIBRATION_h
#define _CALIBRATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
// Include Necessary Libraries ~~~~~~~~~~~~~~~~~~~~~~~
#include "def_pins.h"
#include "def_hardware_properties.h"
#include "def_calibration.h"
#include "print_templates.h"
// Simple Definitions
// Class Definitions
#include "dock_robot.h"
// General Helper Function Include
// Low-Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void reportADC(uint8_t adcPin);
// Mid-Level Functions  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void moveHoop_oneQuarter(dock_robot* robot);
void moveArm_oneQuarter(dock_robot* robot);
// Main Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void calibration_report(dock_robot*);

// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif

