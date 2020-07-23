/*///////////////////////////////////////////////////////////////////////
 Name:		vectorMathFunctions.h / .cpp
 Author:	Petras Swissler

 Purpose:	Various functions to perform vector math
*////////////////////////////////////////////////////////////////////////
#ifndef _VECTORMATHFUNCTIONS_h
#define _VECTORMATHFUNCTIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
/////////////////////////////////////////////////////////////////////////
// Include Libraries

// Low-Level Functions
float vec_dot(float ax, float ay, float az, float bx, float by, float bz);
float vec_len(float ax, float ay, float az);
void vec_cross(float* result, float ax, float ay, float az, float bx, float by, float bz);
float makeUnit(float component, float vec_mag);

// Mid-Level Functions
float vec_angle(float ax, float ay, float az, float bx, float by, float bz);

// High-Level Functions

#endif

