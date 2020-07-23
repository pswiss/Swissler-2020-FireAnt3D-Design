/*///////////////////////////////////////////////////
Name:		logic.cpp
Created :	3 / 25 / 2019
Author :	Petras Swissler

Purpose :	Handle all logic functions
*/////////////////////////////////////////////////////
// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef _LOGIC_h
#define _LOGIC_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
// Include Necessary Libraries ~~~~~~~~~~~~~~~~~~~~~~~
#include "config_dock.h"
#include "def_debugConfig.h"
// Simple Definitions
#include "def_commInterpretation.h"
#include "def_hardware_properties.h"
#include "def_status_messages.h"
#include "def_calibration.h"
#include "def_decode_comm.h"
// Class Definitions
#include "dock_robot.h"
// General Helper Function Include
#include "startup_functions.h"
#include "outputs.h"
#include "simple_helper_functions.h"
// Low-Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void integrateValues(dock_robot* robot);
// Mid-Level Functions  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void behavior_attach(dock_robot*);
void behavior_detach(dock_robot*);
void behavior_flip(dock_robot*	);
void behavior_prepare(dock_robot*);
void behavior_stop(dock_robot*	);
void behavior_puppet(dock_robot*);
// Main Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void runLogic(dock_robot*);

// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif

