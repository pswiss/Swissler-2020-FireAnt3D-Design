/*////////////////////////////////////////////////////
 Name:		def_hardware_properties.h
 Created:	3/27/2019
 Author:	Petras Swissler

 Purpose:	Define hardware (electrical and 
								mechanical) properties
*/////////////////////////////////////////////////////
#pragma once

// Hardware Configuration Properties
#define NUMBER_OF_DOCKS			(float)3.0

// Microcontroller Properties
#define ATMEGA328_MAX_PWM		255
#define ATMEGA328_MAX_ADC		1023

// H-Bridge Properties
#define DRV8871_MAX_CURRENT		(float)3.6
#define DESIRED_MAX_CURRENT		(DRV8871_MAX_CURRENT*0.9)

// Battery Properties
#define V_1CELL_LIPO_MIN		(float)3.2			
#define V_1CELL_LIPO_NOM		(float)3.7
#define V_1CELL_LIPO_MAX		(float)4.2
#define LIPO_CELLS_PER_DOCK		(float)3.0
#define LIPO_CELLS_PER_ROBOT	(LIPO_CELLS_PER_DOCK*NUMBER_OF_DOCKS)
#define MIN_BATTERY_VOLTAGE		(V_1CELL_LIPO_MIN*LIPO_CELLS_PER_ROBOT)
#define NOM_BATTERY_VOLTAGE		33.3
#define MAX_BATTERY_VOLTAGE		(V_1CELL_LIPO_MAX*LIPO_CELLS_PER_ROBOT)

// Voltage Regulator Properties
#define VREG_OUT_VOLTAGE		(float)5.0