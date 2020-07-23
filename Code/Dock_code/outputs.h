/*///////////////////////////////////////////////////
Name:		outputs.h
Created :	3 / 25 / 2019
Author :	Petras Swissler

Purpose :	Run all hardware outputs safely
*/////////////////////////////////////////////////////
// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef _OUTPUTS_h
#define _OUTPUTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// Include Necessary Libraries ~~~~~~~~~~~~~~~~~~~~~~~
#include <EEPROM.h>
#include "config_dock.h"
// Simple Definitions
#include "def_friendly_names.h"
#include "def_hardware_properties.h"
#include "def_pins.h"
#include "def_config.h"
// Class Definitions
#include "dock_robot.h"
// General Helper Function Include
#include "simple_helper_functions.h"
#include "hw_h_bridges.h"
// Low-Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void safety_armLimit	(dock_robot*	);
void output_hoopMotor	(dock_robot*	);
void output_armMotor	(dock_robot*	);
void output_meltOutput	(dock_robot*	);

void safety_valueInitialize	(dock_robot*);
void safety_armLimit		(dock_robot*);
void safety_hoopLimit		(dock_robot*);
void safety_hoopPushed		(dock_robot*);
void safety_overCurrent		(dock_robot*);

void ledON(void);
void ledOFF(void);

float pulseLogic(float baseValue, float lowScale, float highScale, unsigned long time, unsigned long pulsePeriod);
int twitchLogic(unsigned long nowTime, unsigned long twitchPeriod);
// Medium-Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void modifyOutputsForSafety(dock_robot*	);
// Main Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void executeOutputs(dock_robot*	);
void shutdown_all(dock_robot*	);
// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif