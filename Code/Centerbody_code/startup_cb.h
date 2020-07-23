/*///////////////////////////////////////////////////////////////////////
 Name:		startup.h / .cpp
 Created:	3/30/2019
 Author:	Petras Swissler

 Purpose:	Initialize all aspects of the board
*////////////////////////////////////////////////////////////////////////
#ifndef _STARTUP_h
#define _STARTUP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
/////////////////////////////////////////////////////////////////////////
#include "def_names.h"
#include "def_EEPROM_addresses_cb.h"
#include "config_centerbody.h"
// Include Libraries
#include "robot_centerbody.h"
#include "EEPROM_functions.h"
#include <avr/power.h>
//#include <SparkFunLSM9DS1.h>
//#include <SparkFunLSM6DS3.h>
#include "def_pins_cb.h"
//#include "Narcoleptic.h"
#include "inputs_cb.h"
#include "outputs_cb.h"
// Low-Level Functions
void config_board(void);
void beginIMU(IMUpn*);
void reducePowerConsumption(void);
void config_hw_properties(robot_centerbody* robot);
// High-Level Functions
void robot_init(robot_centerbody*, IMUpn*, uint8_t*);
/////////////////////////////////////////////////////////////////////////
#endif

