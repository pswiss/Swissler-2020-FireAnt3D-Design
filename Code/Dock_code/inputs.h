/*////////////////////////////////////////////////////
 Name:		inputs.h
 Created:	3/25/2019
 Author:	Petras Swissler

 Purpose:	Update measurements for all inputs
*/////////////////////////////////////////////////////
// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef _INPUTS_h
#define _INPUTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// Include Necessary Libraries ~~~~~~~~~~~~~~~~~~~~~~~
#include "config_dock.h"
// Simple Definitions
#include "def_pins.h"
#include "def_friendly_names.h"
#include "def_calibration.h"
#include "def_hardware_properties.h"
#include "def_config.h"

// Class Definitions
#include "dock_robot.h"

// General Helper Function Include
#include "simple_helper_functions.h"

// Low-Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void updateSenseTimes(dock_robot*);
void senseDockCurrent(dock_robot*);
void senseHoopCurrent(dock_robot*);
void senseDockVoltage(dock_robot*);
void checkHoopLevel(dock_robot*);
void checkArmLimit(dock_robot*);
void checkDockLevel(dock_robot*);
void removeAnalogChargeBuildup(void);

// Main Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void measureSensors(dock_robot*);

// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif