/*///////////////////////////////////////////////////////////////////////
 Name:		startup.h / .cpp
 Created:	3/30/2019
 Author:	Petras Swissler

 Purpose:
*////////////////////////////////////////////////////////////////////////
#ifndef _OUTPUTS_h
#define _OUTPUTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
/////////////////////////////////////////////////////////////////////////
// Include Libraries
#include "communications_cb.h"
#include "def_commInterpretation.h"
#include "def_decode_comm.h"
#include "def_friendly_names.h"
#include "def_behaviorConfig.h"

// Low-Level Functions
void dockStop(robot_centerbody* robot, uint8_t dock, uint8_t* msgArray);
void dockFlip(robot_centerbody* robot, uint8_t dock, float power, int8_t direction, uint8_t* msgArray);
void dockTwitch(robot_centerbody* robot, uint8_t dock, float power , uint8_t* msgArray);
void dockMeltStop(robot_centerbody* robot, uint8_t dock, uint8_t* msgArray);
void dockPuppet(robot_centerbody* robot, uint8_t dock, int8_t hoopDir, float hoopPow, int8_t armDir, float armPow, uint8_t* msgArray);

void dockAttach(robot_centerbody* robot, uint8_t dock, uint8_t* msgArray);
void dockDetach(robot_centerbody* robot, uint8_t dock, uint8_t* msgArray);
void dockPrepare(robot_centerbody* robot, uint8_t dock, uint8_t prepare_direction, uint8_t* msgArray);
// Mid-Level Functions
void shutdownAllDocks(robot_centerbody* robot, uint8_t* msgArray);
// High-Level Functions
/////////////////////////////////////////////////////////////////////////
#endif

