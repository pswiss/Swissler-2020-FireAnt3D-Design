/*///////////////////////////////////////////////////////////////////////
 Name:		Config_centerbody.h
 Author:	Petras Swissler

 Purpose:	Define the general robot behavior using #define statements
*////////////////////////////////////////////////////////////////////////
#pragma once
#define CONFIG_CHECK
// Configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// General Config--------------------------------------------------------
#define DEBUG
//#define IDENTIFY_DOCKS
#define SERIAL_REPORT
#define ROBOT_TYPE_CENTERBODY
//#define ONLY_REPORT_SENSOR_READINGS
//#define LOAD_CALIB_FROM_EEPROM
//#define WAIT_FOR_FLIP
//#define FLASH_LED_ONLY
#define WAIT_FOR_KEYPRESS_IN_STARTUP
#define DEBUG_GET_FSR_CAL
//#define FSR_ADC_REPORT_ONLY
//#define TESTING_BLOCK
#define USE_EEPROM

// Hardware Config ------------------------------------------------------
#define IMU_ENABLE

// Step Config ----------------------------------------------------------
#define SERIAL_GET_NEXT_ATTACH_AUTO_STEP

//#define ORIENTATION_TOP
#define ORIENTATION_SIDE
//#define ORIENTATION_BELOW