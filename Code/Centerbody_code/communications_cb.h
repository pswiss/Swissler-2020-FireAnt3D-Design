/*///////////////////////////////////////////////////////////////////////
 Name:		communications.h / .cpp
 Created:	3/30/2019
 Author:	Petras Swissler

 Purpose:	Handle all communications to and from the docks
*////////////////////////////////////////////////////////////////////////
#ifndef _COMMUNICATIONS_h
#define _COMMUNICATIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
/////////////////////////////////////////////////////////////////////////
// Include Libraries
#include "Wire.h"

#include "robot_centerbody.h"

#include "def_commInterpretation.h"
#include "simple_helper_functions.h"

/////////////////////////////////////////////////////////////////////////
// Low-Level Functions
void writeArrayI2C(uint8_t , uint8_t , uint8_t* );
void requestArrayI2C(uint8_t , uint8_t , uint8_t* );

uint8_t getPhaseI2C(uint8_t, uint8_t*);
void clearMsgArray(uint8_t*);
void sendArrayI2C(uint8_t dockID, uint8_t* msgArray, uint8_t msg_length, uint8_t check_byte_number);

// Mid-Level Functions
float returnInformationInterpretation(uint8_t whatInformation, uint8_t* msgArray);
void writeCheckByte(uint8_t checkByteNum, uint8_t* outArray);

// High-Level Functions
float getDockInformation(robot_centerbody* robot, uint8_t dockID, uint8_t whatInformation, uint8_t* msgArray);
/////////////////////////////////////////////////////////////////////////
#endif

