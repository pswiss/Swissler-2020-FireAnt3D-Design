/*////////////////////////////////////////////////////
 Name:		def_pins.h
 Created:	3/25/2019
 Author:	Petras Swissler

 Purpose:	Define simple helper functions
*/////////////////////////////////////////////////////

#include "simple_helper_functions.h"

//////////////////////////////////////////////////////
// Bound an input between two limits
float bound(float input, float minimum, float maximum)
{
	return max(min(input, maximum), minimum);
}

//////////////////////////////////////////////////////
// Calculate the output value of an exponential filter
float expFilter(float newVal, float oldVal, float alpha)
{
	alpha = bound(alpha, 0.0, 1.0);
	return (newVal*alpha + (1-alpha)*oldVal);
}

//////////////////////////////////////////////////////
// Get the sign of a number
float getFloatSign(float num)
{
	/*	if (*(long*)&num >> 31)
	{	// sign bit is 1, thus the number is negative
		return -1.0;	}
	else
	{	// sign bit is 0, thus the number is positive
		return 1.0;	}*/
	if (num < 0)
	{
		// sign bit is 1, thus the number is negative
		return -1.0;
	}
	else
	{
		// sign bit is 0, thus the number is positive
		return 1.0;
	}
}

float getIntSign(int num)
{
	if (num >= 0){return (float)1;}
	else		{return (float)-1;}
}

//////////////////////////////////////////////////////
// Get the average reading of an analog pin over a 
// certain number of measurements
float getAvgReading(int pin, uint8_t numMeas) {
	float avg = 0;
	for (uint8_t ii = 0; ii < numMeas; ii++) 
	{
		avg = avg + analogRead(pin);
	}
	return (avg / (float)numMeas);
}

int getAvgReading_int(int pin, uint8_t numMeas) {
	int avg = 0;
	for (uint8_t ii = 0; ii < numMeas; ii++)
	{
		avg = avg + analogRead(pin);
	}
	return (avg / numMeas);
}

//////////////////////////////////////////////////////
// Convert a float into an array of chars
void floatToFourChars(float inFloat, uint8_t* outChar)
{
	
	for (uint8_t ii = 0; ii < 4; ii++)
	{	
		outChar[ii] = (uint8_t)((*(uint32_t*) &inFloat) >> (8 * ii)) & 0xFF;	
	}
}

float fourCharsToFloat(uint8_t* inChar)
{
	/*uint8_t tempArray[4];
	tempArray[0] = inChar[3];
	tempArray[1] = inChar[2];
	tempArray[2] = inChar[1];
	tempArray[3] = inChar[0];*/
	return  *(float*)&inChar[0];
}

//////////////////////////////////////////////////////
// Get the middle value of three floats
float getMidof3(float a, float b, float c)
{
	if (a > b)
	{
		if (b > c)
		{
			return b;
		}
		else
		{
			if (c > a)
			{
				return a;
			}
			else {
				return c;
			}
		}
	}
	else
	{
		if (c > b)
		{
			return b;
		}
		else
		{
			if (c > a)
			{
				return c;
			}
			else {
				return a;
			}
		}
	}

	// Should never get here
	return b;
}


//////////////////////////////////////////////////////
// Check whether a certain duration has elapsed
int getMaxofN(int pin, uint8_t numMeas)
{
	int maxMeas = 0;
	for (uint8_t ii = 0; ii < numMeas; ii++)
	{
		int newMeas = analogRead(pin);
		if (newMeas > maxMeas)
		{
			maxMeas = newMeas;
		}
	}
	return maxMeas;
}

//////////////////////////////////////////////////////
// Check whether a certain duration has elapsed
bool checkTime(unsigned long startTime, unsigned long nowTime, float targetDuration_seconds)
{
	if ((nowTime - startTime) >= (targetDuration_seconds*MS_PER_S)) 
	{	return true;	}
	else 
	{	return false;	}
}

//////////////////////////////////////////////////////
// integer square root
int isqrt(int N)
{
	int a = 1;
	int b = N;

	while (abs(a - b) > 1)
	{
		b = N / a;
		a = (a + b) / 2;
	}

	return a;
}

//////////////////////////////////////////////////////
// Get whatever byte is missing from a list of three optionsd
uint8_t	getAbsentByte(uint8_t A, uint8_t B, uint8_t option1, uint8_t option2, uint8_t option3)
{
	// Take in two unique values and three options. Return the unchosen option.
	// If both unique values noth within the options, then return 0
	if (!((A == option1 || A == option2 || A == option3) && (B == option1 || B == option2 || B == option3)))
	{
		return 0;
	}
	else
	{
		return ((uint8_t)(!(A == option1 || B == option1)) * option1 + (uint8_t)(!(A == option2 || B == option2)) * option2 + (uint8_t)(!(A == option3 || B == option3)) * option3);
	}
}

//////////////////////////////////////////////////////
// Encode / Decode floats using an identical conversion factor
uint8_t floatToEncodedByte(float inValue, float conversionFactor)
{
	// Special behavior to prevent overflow
	return ((uint8_t)(bound(inValue * conversionFactor,0,255)));
}

float encodedByteToFloat(uint8_t inValue, float conversionFactor)
{
	return ((float)inValue / conversionFactor);
}

uint8_t s2u_int8(int8_t inbyte)
{
	return (*(uint8_t*)&inbyte);
}

int8_t u2s_int8(uint8_t inbyte)
{
	return (*(int8_t*)&inbyte);
}

//////////////////////////////////////////////////////
// Reporting / Printing Functions
void printArray(uint8_t* msgArray, uint8_t numBytes)
{
	Serial.print(F("["));
	for (uint8_t ii = 0; ii < numBytes; ii++)
	{
		Serial.print(msgArray[ii]);
		Serial.print(F(", "));
	}
	Serial.print(F("]"));
}

// Get free memory
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char* __brkval;
#endif  // __arm__

int getFreeMemory() {
	char top;
#ifdef __arm__
	return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
	return &top - __brkval;
#else  // __arm__
	return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

char waitForKeypress(void) {
	char symbolIn = 0;

	// Wait until have serial data available
	while (Serial.available() == 0) {
		// Do nothing
	}

	symbolIn = Serial.read();

	// Clear the serial buffer
	delay(5);
	while (Serial.available()) {
		Serial.read();
	}

	return symbolIn;
}

// Linear Regression Functions
float getSlope(float x1, float y1, float x2, float y2)
{
	return ((y2 - y1) / (x2 - x1));
}

float getYIntercept(float x1, float y1, float slope)
{
	return (y1 - slope * x1);
}

// Clear Array
void clearArray(uint8_t* msgArray, uint8_t len)
{
	for (uint8_t ii = 0; ii < len; ii++)
	{
		msgArray[ii] = 0;
	}
	return;
}