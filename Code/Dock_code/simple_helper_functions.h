/*////////////////////////////////////////////////////
 Name:		simple_helper_functions.h
 Created:	3/25/2019
 Author:	Petras Swissler

 Purpose:	Define simple helper functions
*/////////////////////////////////////////////////////
// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef _SIMPLE_HELPER_FUNCTIONS_h
#define _SIMPLE_HELPER_FUNCTIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// Include Simple Definitions ~~~~~~~~~~~~~~~~~~~~~~~~
#include "def_friendly_names.h"
// Include Necessary Libraries ~~~~~~~~~~~~~~~~~~~~~~~
// None: All functions intended to stand alone
// Low-Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// None: All functions intended to stand alone
// Main Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Simple Math Functions
float	bound(float input, float minimum, float maximum);
float	expFilter(float newVal, float oldVal, float alpha);
float	getFloatSign(float);
float	getIntSign(int);
float	getMidof3(float, float, float);
float	getSlope(float x1, float y1, float x2, float y2);
float	getYIntercept(float x1, float y1, float slope);
int		isqrt(int);
uint8_t	getAbsentByte(uint8_t A, uint8_t B, uint8_t option1, uint8_t option2, uint8_t option3);

// Timing Functions
bool	checkTime(unsigned long startTime, unsigned long nowTime, float targetDuration_seconds);

// Measurement Functions
float	getAvgReading(int, uint8_t);
int		getAvgReading_int(int pin, uint8_t numMeas);
int		getMaxofN(int pin, uint8_t numMeas);

// Value and type manipulation
void	floatToFourChars(float, uint8_t*);
float	fourCharsToFloat(uint8_t*);
uint8_t s2u_int8(int8_t inbyte);
int8_t	u2s_int8(uint8_t inbyte);
uint8_t floatToEncodedByte(float inValue, float conversionFactor);
float	encodedByteToFloat(uint8_t inValue, float conversionFactor);

// Serial Functions
char waitForKeypress(void);
void printArray(uint8_t* msgArray, uint8_t numBytes);

// Memory Management Functions
int getFreeMemory(void);
void clearArray(uint8_t* msgArray, uint8_t len);

// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif

