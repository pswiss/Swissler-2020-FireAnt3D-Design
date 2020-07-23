/*////////////////////////////////////////////////////
 Name:		def_commInterpretation.h
 Created:	3/26/2019
 Author:	Petras Swissler

 Purpose:	Define friendly names for communications 
			processing
*/////////////////////////////////////////////////////
#pragma once

// RECIEVE DATA //////////////////////////////////////
// Commands from centerbody ~~~~~~~~~~~~~~~~~~~~~~~~~~
#define MSG_I2C_COMMAND_BYTE	0

#define MSG_I2C_COMMAND_ATTACH	10
#define MSG_I2C_COMMAND_DETACH	12
#define MSG_I2C_COMMAND_FLIP	14
#define MSG_I2C_COMMAND_PREPARE	16
#define MSG_I2C_COMMAND_PUPPET	20
#define MSG_I2C_COMMAND_REPORT	30
#define MSG_I2C_COMMAND_SENDMSG	32
#define MSG_I2C_COMMAND_LED		90
#define MSG_I2C_COMMAND_STOP	100
#define CALIBRATION_STATE		120
#define INITIALIZATION_STATE	500

// Byte Orders for I2C commands ~~~~~~~~~~~~~~~~~~~~~~~
// Byte order for attach command
#define ARG_ATT_SWEEP_I_TARGET			1
#define ARG_ATT_SWEEP_VOLTAGE			2
#define ARG_ATT_SWEEP_HOOP_MOTOR_PWR    3
#define ARG_ATT_SWEEP_HOOP_MOTOR_PWR_GROWTH_PERIOD 4
#define ARG_ATT_I_MAX					5
#define ARG_ATT_V_START					6
#define ARG_ATT_V_GROWTH_PERIOD			7
#define ARG_ATT_V_MAX					8
#define ARG_ATT_INT_I_TARGET			9
#define ARG_ATT_SWEEP_DIRECTION			10

// Byte order for detach command
#define ARG_DET_DT1_INT_I				1
#define ARG_DET_DT1_MAX_CURRENT			2
#define ARG_DET_DT1_MAX_VOLTAGE			3
#define ARG_DET_DT2_MAX_CURRENT			4
#define ARG_DET_DT2_MAX_VOLTAGE			5
#define ARG_DET_HOOP_MOTOR_POWER		6
#define ARG_DET_HOOP_MOTOR_MODE			7
#define ARG_DET_ARM_MOTOR_TWITCH_PWR	8
#define ARG_DET_SWEEP_DIRECTION			9

// Byte order for flip command
#define ARG_FLP_DIRECTION				1
#define ARG_FLP_POWER					2
#define ARG_FLP_TIMEOUT					3
#define ARG_FLP_TYPE					4

// Byte order for return command
#define ARG_PRP_DIRECTION				1

// Byte order for stop command
// No arguements for stop command

// Byte order for puppet command
#define ARG_PUP_HOOP_MOTOR_DIRECTION	1
#define ARG_PUP_HOOP_MOTOR_POWER		2
#define ARG_PUP_ARM_MOTOR_DIRECTION		3
#define ARG_PUP_ARM_MOTOR_POWER			4
#define ARG_PUP_TIMEOUT					5

// Byte order for report command
#define ARG_RPT_WHICH_ONE				1

// Byte order for sendmsg command
#define ARG_SND_OUTMSG					1

// Byte order for setting the LED
#define ARG_LED_LEVEL					1

// REPORT DATA ////////////////////////////////////////
#define I2C_SEND_IDOCK					10
#define I2C_SEND_IHOOP					11
#define I2C_SEND_LEVELS					12
#define I2C_SEND_IN_COM					13
#define I2C_SEND_VDOCK					14
#define I2C_SEND_INT_I					15
#define I2C_SEND_HMOT_PWR				16
#define I2C_SEND_AMOT_PWR				17
#define I2C_SEND_STATUS					18
#define I2C_SEND_CALC_HOOP_POS			19
#define I2C_SEND_VBAT_MINUS				20
#define I2C_SEND_CALC_ARM_POS			110
#define I2C_SEND_STATE					111
#define I2C_SEND_PHASE					112
#define I2C_SEND_MEMORY_USAGE			200

// DOCK COMM DATA /////////////////////////////////////
#define DOCK_MSG_NOTHING				0

// Message Sizes //////////////////////////////////////
#define NUMBYTES_FOR_CMD_INFO			2
#define NUMBYTES_FROM_REPORT			4

#define I2C_CHECK_BYTE_NUM				12