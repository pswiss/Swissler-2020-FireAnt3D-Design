/*///////////////////////////////////////////////////////////////////////
 Name:		__testRigStuff.h / .cpp
 Created:	04/05/2019
 Author:	Petras Swissler

 Purpose:	Include everything needed for the test rig
*////////////////////////////////////////////////////////////////////////
#ifndef ___TESTRIGSTUFF_h
#define ___TESTRIGSTUFF_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
/////////////////////////////////////////////////////////////////////////
// Definitions
#define LC_SENSEFORCE HX711

#define PIN_com_lcCom				9000
#define PIN_com_lcClk				9000
#define RANDOMROTATIONCONTROLPIN	9000
#define PIN_do_COOLING_FAN			9000
#define PIN_LINAC_DIR				9000
#define PIN_LINAC_PWR				9000
#define PIN_LINEAR_POSITION			9000

#define BAUD_RATE					115200
#define SEN_LC_ERR_HI				5000

#define CALIB_MM_PER_ADC			0.6

/////////////////////////////////////////////////////////////////////////
// Include Libraries
#include "HX711.h"

#include "robot_centerbody.h"

#include "def_friendly_names.h"
#include "def_commInterpretation.h"
#include "def_behaviorConfig.h"

#include "communications_cb.h"
#include "simple_helper_functions.h"
#include "outputs_cb.h"
#include "inputs_cb.h"

// Low-Level Functions
void senseLCForce(robot_centerbody* robot, LC_SENSEFORCE* loadCell);
void runLinac(int8_t direction, uint8_t power);
void manualSetup(robot_centerbody* robot);
void testRigInputs(robot_centerbody* robot, LC_SENSEFORCE* loadCell);
void setFan(uint8_t mode);
// High-Level Functions
void testRig_init(robot_centerbody* robot, LC_SENSEFORCE* loadCell);
void testRig_run(robot_centerbody* robot, LC_SENSEFORCE* loadCell, uint8_t* msgArray);
void reportData(robot_centerbody* robot);

#endif

