/*////////////////////////////////////////////////////
 Name:		EEPROM_functions
 Created:	5/28/2019
 Author:	Petras Swissler

 Purpose:	Functions to aid reading / writing to/from EEPROM
*/////////////////////////////////////////////////////
// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef _EEPROM_FUNCTIONS_h
#define _EEPROM_FUNCTIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// Include Necessary Libraries ~~~~~~~~~~~~~~~~~~~~~~~
#ifdef ROBOT_TYPE_DOCK
	#include "config_dock.h"
#endif // ROBOT_TYPE_DOCK

// Simple Definitions
#include "def_friendly_names.h"
// Class Definitions
// Necessary Libraries
#include <EEPROM.h>
#include "simple_helper_functions.h"
// Low-Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void writeFloatToEEPROM(float floatToWrite, int EEPROM_position);
float readFloatFromEEPROM(int EEPROM_position_0);

void writeByteToEEPROM(uint8_t byteToWrite, int EEPROM_position);
uint8_t readByteFromEEPROM(int EEPROM_position);

// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif

