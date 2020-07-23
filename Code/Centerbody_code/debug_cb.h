/*///////////////////////////////////////////////////////////////////////
 Name:		debug_cb.h / .cpp
 Author:	Petras Swissler

 Purpose:	All required debug functions
*////////////////////////////////////////////////////////////////////////
#ifndef _DEBUG_CB_h
#define _DEBUG_CB_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
/////////////////////////////////////////////////////////////////////////
// Include Libraries
#include "config_centerbody.h"
#include "def_names.h"
#include "def_commInterpretation.h"
#include "robot_centerbody.h"
#include "inputs_cb.h"
#include "outputs_cb.h"
#include "communications_cb.h"
#include "cb_phases.h"
#include "serial_helper_programs.h"

// Low-Level Functions
float getValFor1000g(uint8_t movement_dock, int8_t astep_direction, robot_centerbody* robot);
void debug_setLED(uint8_t dock, uint8_t* msgArray, uint8_t level);
void debug_flashDockLED(uint8_t dockID, uint8_t* msgArray, uint8_t numFlashes, uint32_t duration);
void debug_identifyDocks(uint8_t* msgArray);
uint8_t dock_num_to_I2C_addr(uint8_t num);
void translateDockNumbersToProper(robot_centerbody* robot);

// Mid-Level Functions
void debug_sensorReport(robot_centerbody* robot);
bool action_detach(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray);
bool action_attach(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray);
bool action_flip(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray);
bool action_hoopMove(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray);
bool action_armMove(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray);
bool action_prepare(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray);
bool action_information(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray);
void astep_next_phase(robot_centerbody* robot, uint8_t* msgArray, uint8_t nextPhase);

// High-Level Functions
void debug_start(robot_centerbody*);
void debugLogic(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray);
void userInputNextStep(robot_centerbody* robot, IMUpn* imu);
bool automatedStep(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray);
bool puppetLogic(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray);

#endif