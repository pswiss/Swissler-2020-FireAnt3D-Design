/*///////////////////////////////////////////////////////////////////////
 Name:		vectorMathFunctions.h / .cpp
 Author:	Petras Swissler

 Purpose:	Various functions to perform vector math
*////////////////////////////////////////////////////////////////////////
#include "vectorMathFunctions.h"

// High-Level Functions

// Mid-Level Functions
float vec_angle(float ax, float ay, float az, float bx, float by, float bz)
{	return acos(vec_dot(ax, ay, az, bx, by, bz) / (vec_len(ax, ay, az) * vec_len(bx, by, bz)));}

// Low-Level Functions
// Find the dot product of two vectors
float vec_dot(float ax, float ay, float az, float bx, float by, float bz)
{	return ax * bx + ay * by + az * bz;	}

// Find the length of a vector
float vec_len(float ax, float ay, float az)
{	return sqrt(ax * ax + ay * ay + az * az);	}

// Find the cross product of two vectors and put into result vector
void vec_cross(float* result_vec3, float ax, float ay, float az, float bx, float by, float bz)
{
	result_vec3[0] = ay * bz - az * by;
	result_vec3[1] = ax * bz - az * bx;
	result_vec3[2] = az * by - ay * bx;
	return;
}

// Make a component of a vector consistent with a unit vector
float makeUnit(float component, float vec_mag)
{	return (component / vec_mag);	}