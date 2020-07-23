/*///////////////////////////////////////////////////
Name:		communications.h
Created :	3 / 25 / 2019
Author :	Petras Swissler

Purpose :	Handle all necessary communications
*/////////////////////////////////////////////////////
// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef _COMMUNICATIONS_h
#define _COMMUNICATIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// Include Necessary Libraries ~~~~~~~~~~~~~~~~~~~~~~~
#include "config_dock.h"
#include "def_debugConfig.h"
// Simple Definitions
#include "def_friendly_names.h"
#include "def_pins.h"
#include "def_calibration.h"
#include "def_commInterpretation.h"
#include "def_decode_comm.h"
// Class Definitions
#include "dock_robot.h"
#include "Wire.h"
// General dependencies
#include "outputs.h"
// General Helper Function Include
#include "simple_helper_functions.h"
// Low-Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void processIncoming_i2c_communication(dock_robot*, uint8_t*);
void read_i2c_Data(uint8_t*, int numbytes);
void flush_i2c_Buffer(void);
void get_i2c_ID(dock_robot*);

bool i2cCheckByte(uint8_t checkByteNum, uint8_t* outArray);

void sendRequestedData_i2c(dock_robot*, uint8_t*);

// Main Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void interpret_i2c_Data(uint8_t*, dock_robot*);
void handle_i2c_communications(dock_robot*, uint8_t*, uint8_t*, uint8_t*);
void handleDockCommunications(dock_robot*, unsigned long*, uint8_t*);

// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif

