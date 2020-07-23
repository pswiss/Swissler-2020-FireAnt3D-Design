/*////////////////////////////////////////////////////
 Name:		hw_h_bridges.h
 Created:	3/25/2019
 Author:	Petras Swissler

 Purpose:	Create functions to control h-bridges
*/////////////////////////////////////////////////////
// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef _HW_H_BRIDGES_h
#define _HW_H_BRIDGES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
// Simple Aliases for Functions ~~~~~~~~~~~~~~~~~~~~~~
#define hw_melt_hb		hw_hb_drv8871ddar
#define hw_motor_hb		hw_hb_lv8548mc
#define pins_armMotor	PIN_PWM_ARM_MOTOR_A, PIN_PWM_ARM_MOTOR_B
#define pins_hoopMotor	PIN_PWM_HOOP_MOTOR_A, PIN_PWM_HOOP_MOTOR_B

// Include Necessary Libraries ~~~~~~~~~~~~~~~~~~~~~~~
#include "config_dock.h"
// Simple Definitions
#include "def_pins.h"
#include "def_hardware_properties.h"
#include "def_friendly_names.h"
// Class Definitions
// General Helper Function Include
// Low-Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Main Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void hw_hb_drv8871ddar(int direction,float power);
void hw_hb_lv8548mc(int direction,float power,int pina,int pinb);

// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif