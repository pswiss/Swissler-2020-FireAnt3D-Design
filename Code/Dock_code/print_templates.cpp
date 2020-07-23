// 
// 
// 

#include "print_templates.h"

void printHeader(String Title, uint8_t width, uint8_t type = 0)
{
	Serial.println();
	switch (type)
	{
	default:
		for (uint8_t ii = 0; ii < width; ii++) {
			Serial.print("/");
		}

		Serial.println();
		Serial.println(Title);
		for (uint8_t ii = 0; ii < width; ii++) {
			Serial.print("-");
		}
		break;
	}
	Serial.println();
}

void printFooter(uint8_t width, uint8_t type = 0)
{
	Serial.println();
	switch (type)
	{
	default:
		for (uint8_t ii = 0; ii < width; ii++) {
			Serial.print("/");
		}
		break;
	}
	Serial.println();
}