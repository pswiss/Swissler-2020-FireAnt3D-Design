/*///////////////////////////////////////////////////////////////////////
 Name:		def_behavior_config.h
 Author:	Petras Swissler

 Purpose:	Define the specifics of robot behavior during locomotion
*////////////////////////////////////////////////////////////////////////
#pragma once
/////////////////////////////////////////////////////////////////////////
// Include Libraries
#include "def_friendly_names.h"
#include "config_centerbody.h"
// Communication Timeouts ----------------------------------------
#define STD_TIMEOUT_DURATION_S					0.25
#define PUPPET_TIMEOUT_DURATION_S				1.0
// Serial Input Behavior -----------------------------------------
#define SERIAL_INPUT_ECHO_ON					true
#define SERIAL_INPUT_ECHO_OFF					false
#define SER_IN_TIMEOUT							100
// Full Step Constants -------------------------------------------
#ifdef ORIENTATION_TOP
	#define PARAM_ASTEP_01_TIME_TO_REVERSE		2.5
	#define PARAM_ASTEP_02_TIME_TO_PULL_UP		0.25
	#define PARAM_ASTEP_04_TIME_PRELOAD			0.25
	#define PARAM_ASTEP_05_DETACH_PULL_FORCE	50
	#define PARAM_ASTEP_10_ATT_PRESS_FORCE		1000.0
	#define PARAM_ASTEP_08_FLIP_PRESS_FORCE		(PARAM_ASTEP_10_ATT_PRESS_FORCE * 0.9)
	#define PARAM_ASTEP_09_BACKOFF_TIME			7.5
#endif // ORIENTATION_TOP
#ifdef ORIENTATION_SIDE
#define PARAM_ASTEP_01_TIME_TO_REVERSE		2.0
#define PARAM_ASTEP_02_TIME_TO_PULL_UP		0.2
#define PARAM_ASTEP_04_TIME_PRELOAD			0.2
#define PARAM_ASTEP_05_DETACH_PULL_FORCE	25
#define PARAM_ASTEP_10_ATT_PRESS_FORCE		1000
#define PARAM_ASTEP_08_FLIP_PRESS_FORCE		(PARAM_ASTEP_10_ATT_PRESS_FORCE * 0.9)
#define PARAM_ASTEP_09_BACKOFF_TIME			3.5
#endif // ORIENTATION_SIDE
#ifdef ORIENTATION_BELOW
	#define PARAM_ASTEP_01_TIME_TO_REVERSE		1.5
	#define PARAM_ASTEP_02_TIME_TO_PULL_UP		0.1
	#define PARAM_ASTEP_04_TIME_PRELOAD			0.1
	#define PARAM_ASTEP_05_DETACH_PULL_FORCE	0
	#define PARAM_ASTEP_10_ATT_PRESS_FORCE		800
	#define PARAM_ASTEP_08_FLIP_PRESS_FORCE		(PARAM_ASTEP_10_ATT_PRESS_FORCE * 0.9)
	#define PARAM_ASTEP_09_BACKOFF_TIME			2.5
#endif // ORIENTATION_BELOW

#define PARAM_ASTEP_06_TIME_TO_MOVE				6
#define PARAM_ASTEP_07_MIN_PREP_TIME			1.0
#define PARAM_ASTEP_075_PREP_REV_TIME			4

#define	PARAM_10_MAX_TIME						(10 * PARAM_ASTEP_09_BACKOFF_TIME)

#define	PARAM_ASTEP_110_INTEGRATED_CURRENT		30
#define PARAM_ASTEP_12_ATT_INT_CURR				30
#define PARAM_ASTEP_13_FINAL_COOL_S				60

// Flip constants
#define FLIP_SPEED_PRESS						1.0
#define FLIP_DIRECTION_FORWARD					1
#define FLIP_DIRECTION_REVERSE					2
#define FLIP_TIMEOUT_DURATION_S					STD_TIMEOUT_DURATION_S
#define FLIP_RECAL_WAIT_DURATION				0.05
#define FLIP_FORWARD_TIME_WITHOUT_RECAL			3.0
#define FLIP_FORWARD_FORCE_RATIO				0.3
#define FLIP_REVERSE_TIME_BEFORE_RECAL			0.75

// Attach Constants
#define ATT_SPEED_PRESS							0.8
#define ATT_DIRECTION_FORWARD					1
#define ATT_TWITCH_POWER						0.8
#define ATT_MLT_CUR								3.0
#define ATT_MLT_INT_CUR							25
#define ATT_MLT_VLT_STRT						50
#define ATT_MLT_VLT_GRW_PER						1
#define ATT_MLT_VLT_END							50
#define ATT_SWP_CUR								0.02
#define ATT_SWP_VLT								26
#define ATT_SWP_MOT_PWR							0.35
#define ATT_SWP_MOT_PWR_GRW						2.5

// Detach Constants
#define DET_TIME_BEFORE_PULL					3
#define DET_PULL_FORCE_FSR						400
#define DET_PULL_POWER							1.0
#define DET_DT1_INT_I							15
#define DET_DT1_MAX_CURRENT						ATT_MLT_CUR
#define DET_DT1_MAX_VOLTAGE						ATT_MLT_VLT_END
#define DET_DT2_MAX_CURRENT						DET_DT1_MAX_CURRENT
#define DET_DT2_MAX_VOLTAGE						DET_DT1_MAX_VOLTAGE	
#define DET_HOOP_MOTOR_POWER					1.0
#define DET_HOOP_MOTOR_MODE						MOVETYPE_NORMAL
#define DET_ARM_MOTOR_TWITCH_PWR				1.0
#define DET_TIME_BUFFER							6
#define DET_FORCE_MOVEMENT						0.5
#define PARAM_DET_HOOP_PULL_HELP_AMT			0.25
#define PARAM_DET_INT_CURRENT_THRESHOLD_TO_PULL 10

// EZ step constants
#define EZ_NEXT_STEP_IS_REVERSE					21
#define ASTEP_SPECIAL_RESETTLE					9
#define ASTEP_SPECIAL_EZ_REVERSE				8
#define ASTEP_SPECIAL_REVERSE_ATT				70