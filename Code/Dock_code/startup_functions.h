/*////////////////////////////////////////////////////
 Name:		startup_functions.h
 Created:	3/25/2019
 Author:	Petras Swissler

 Purpose:	Functions to run before anything else
*/////////////////////////////////////////////////////
// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef _STARTUP_FUNCTIONS_h
#define _STARTUP_FUNCTIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// Include Necessary Libraries ~~~~~~~~~~~~~~~~~~~~~~~
#include "config_dock.h"
#include <avr/wdt.h>
#include <EEPROM.h>
// Simple Definitions
#include "def_friendly_names.h"
#include "def_pins.h"
#include "def_calibration.h"
#include "def_commInterpretation.h"
#include "hardcodedvalues.h"
// Class Definitions
#include "dock_robot.h"
// General Helper Function Include
#include "EEPROM_functions.h"
#include "def_EEPROM_addresses.h"
#include "communications.h"
#include "outputs.h"
#include "simple_helper_functions.h"
#include "inputs.h"
// Low-Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void boardConfig(void);
void calibrateSensors(dock_robot*);
void loadCalib(dock_robot* robot);
void set_arm_to_known_position(	dock_robot* robot, int8_t mdir);
void set_hoop_to_known_position(dock_robot* robot, int mdir);

// Main Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void robot_startup(dock_robot*);

// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif