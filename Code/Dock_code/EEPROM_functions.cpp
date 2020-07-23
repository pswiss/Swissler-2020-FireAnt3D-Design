/*////////////////////////////////////////////////////
 Name:		EEPROM_functions
 Created:	5/28/2019
 Author:	Petras Swissler

 Purpose:	Functions to aid reading / writing to/from EEPROM
*/////////////////////////////////////////////////////
#include "EEPROM_functions.h"

// Write Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void writeFloatToEEPROM(float floatToWrite, int EEPROM_position)
{
	// Translate the float to 4 bytes
	uint8_t byteArray[4];
	floatToFourChars(floatToWrite, &byteArray[0]);

	// Write each byte in turn
	for (uint8_t ii = 0; ii < 4; ii++)
	{
		EEPROM.update(EEPROM_position + ii, byteArray[ii]);
	} 
}

void writeByteToEEPROM(uint8_t byteToWrite, int EEPROM_position)
{
	EEPROM.update(EEPROM_position, byteToWrite);
}

// Read Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float readFloatFromEEPROM(int EEPROM_position_0)
{
	// Load all bytes into an array
	uint8_t byteArray[4];
	for (uint8_t ii = 0; ii < 4; ii++)
	{
		byteArray[ii] = EEPROM.read(EEPROM_position_0 + ii);
	}

	// Convert the array to a float and return this float
	return fourCharsToFloat(&byteArray[0]);
}

uint8_t readByteFromEEPROM(int EEPROM_position)
{
	return EEPROM.read(EEPROM_position);
}