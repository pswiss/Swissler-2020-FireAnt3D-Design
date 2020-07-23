// print_templates.h

#ifndef _PRINT_TEMPLATES_h
#define _PRINT_TEMPLATES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <string.h>
#include <stdint.h>

void printHeader(String Title, uint8_t width, uint8_t type );
void printFooter(uint8_t width, uint8_t type );
#endif

