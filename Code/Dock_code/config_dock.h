#pragma once
// Configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Debug configurations ---------------------
//#define DEBUG
//#define REPORT_CALIBRATION_DATA
//#define LED_FLASH_ONLY
//#define SERIAL_ENABLE
#define USE_WATCHDOG_TIMER

// System configurations --------------------
#define ENABLE_MELT
#define ENABLE_ARM_MOTOR
#define ENABLE_HOOP_MOTOR
#define ENABLE_LED
#define HARDCODED_CALIBRATION_DATA
#define FAST_START_FLASH


#ifdef HARDCODED_CALIBRATION_DATA
#define		BOARD_CIRCLE
//#define	BOARD_TRIANGLE
//#define	BOARD_SQUARE
#endif // HARDCODED_CALIBRATION_DATA

#define USE_HARDCODED_I2C_ADDR
#ifdef BOARD_CIRCLE
#define		HARDCODED_I2C_ADDRESS	DOCK_1	
#endif // BOARD_CIRCLE
#ifdef BOARD_SQUARE
#define		HARDCODED_I2C_ADDRESS	DOCK_0
#endif // BOARD_CIRCLE
#ifdef BOARD_TRIANGLE
#define		HARDCODED_I2C_ADDRESS	DOCK_2
#endif // BOARD_CIRCLE

// Override And Other Special Conditions ----
#define OVERRIDE_ARM_MOTOR_DURING_ATT
#define ENABLE_USE_OF_EEPROM
// EEPROM ADDRESSES
#define VAL_INTEGRITY_0 44
#define VAL_INTEGRITY_1 21

#define EEPROM_ADDR_SFAD	0
#define EEPROM_ADDR_SABOS	1
#define EEPROM_ADDR_SLDC	2
#define EEPROM_ADDR_INTEGRITY_0 4
#define EEPROM_ADDR_INTEGRITY_1 5
