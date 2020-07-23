/*///////////////////////////////////////////////////
Name:		debug.h / .cpp
Created :	4/ 02 / 2019
Author :	Petras Swissler

Purpose :	Define debug behavior to validate 
			low-level code and hardware
*/////////////////////////////////////////////////////
// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef _DEBUG_h
#define _DEBUG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
// Include Necessary Libraries ~~~~~~~~~~~~~~~~~~~~~~~
#include "dock_robot.h"
#include "outputs.h"
// Low-Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define DEBUG_MESSAGE_STRING1 "other: shutdown | q: dock melt half (, and . incr/decr voltage) ; w: dock grounded ; e: dock high z | "
#define DEBUG_MESSAGE_STRING2 "a: M1 forward ; s: M1 reverse ; d: M1 brake ; f: M1 coast | z: M2 forward ; x: M2 reverse ; c: M2 brake ; v: M2 coast"
//char waitForKeypress(void);
void reportSensorData(dock_robot*);
void debugLogic(dock_robot*,char);
void cycleMotors(void);
void cycleMeltHB(void);
void reportRawADCData(dock_robot* robot);

void dangerCode(dock_robot* robot, uint8_t option);
// Main Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void runDebug(dock_robot*);

#endif

