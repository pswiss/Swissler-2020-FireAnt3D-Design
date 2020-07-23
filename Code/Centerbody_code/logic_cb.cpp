/*///////////////////////////////////////////////////////////////////////
 Name:		startup.h / .cpp
 Created:	3/30/2019
 Author:	Petras Swissler

 Purpose:
*////////////////////////////////////////////////////////////////////////
#include "logic_cb.h"
// High Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void logic(robot_centerbody* robot, uint8_t* msg_out)
{
	switch (robot->state)
	{
	case CENTER_STATE_INITIALIZE:
		// Force Dock 0 to return
		
		// Dock zero will be the first dock to move
		robot->movement_dock = DOCK_0;

		// Attached dock = none
		break;
	case CENTER_STATE_DECIDE:
		// find highest dock. This is the next dock to move
		break;
	case CENTER_STATE_STEP:
		// detach prior docks
		// flip about movement dock until press down with some force
		// attach on next dock
		// cool
		// next dock is now movement dock
		break;
	case CENTER_STATE_IDLE:
		// Do nothing until some amount of time has passed
		break;
	default:
		robot->state = CENTER_STATE_IDLE;
		robot->stateTimer0 = millis();
	}
}
// Low Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~