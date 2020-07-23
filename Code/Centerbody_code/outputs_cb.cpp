/*///////////////////////////////////////////////////////////////////////
 Name:		startup.h / .cpp
 Created:	3/30/2019
 Author:	Petras Swissler

 Purpose:
*////////////////////////////////////////////////////////////////////////
#include "outputs_cb.h"
// Mid Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void shutdownAllDocks(robot_centerbody* robot, uint8_t* msgArray)
{
	dockStop(robot, DOCK_0, msgArray);
	dockStop(robot, DOCK_1, msgArray);
	dockStop(robot, DOCK_2, msgArray);
}
// Low Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void dockStop(robot_centerbody* robot, uint8_t dockID, uint8_t* msgArray)
{
	// Queue up the message array
	clearMsgArray(msgArray);									
	msgArray[MSG_I2C_COMMAND_BYTE] = MSG_I2C_COMMAND_STOP;		

	// Write the message to the desired dockID			
	sendArrayI2C(dockID, msgArray, COM_I2C_MAX_MSG_LENGTH, I2C_CHECK_BYTE_NUM);
}

void dockFlip(robot_centerbody* robot, uint8_t dockID, float power, int8_t direction, uint8_t* msgArray)
{
	// Queue up the message array  
	clearMsgArray(msgArray);
	msgArray[MSG_I2C_COMMAND_BYTE]	= MSG_I2C_COMMAND_FLIP;
	msgArray[ARG_FLP_DIRECTION]		= s2u_int8(direction);
	msgArray[ARG_FLP_POWER]			= floatToEncodedByte(power, MAX_VALUE_PWM);
	msgArray[ARG_FLP_TIMEOUT]		= floatToEncodedByte(FLIP_TIMEOUT_DURATION_S, ENC_TIME);
	msgArray[ARG_FLP_TYPE]			= MOVETYPE_NORMAL;

	// Write the message to the desired dockID
	sendArrayI2C(dockID, msgArray, COM_I2C_MAX_MSG_LENGTH, I2C_CHECK_BYTE_NUM);
}

void dockTwitch(robot_centerbody* robot, uint8_t dockID, float power, uint8_t* msgArray)
{
	// Queue up the message array  
	clearMsgArray(msgArray);
	msgArray[MSG_I2C_COMMAND_BYTE]	= MSG_I2C_COMMAND_FLIP;
	msgArray[ARG_FLP_DIRECTION]		= MOTORDIR_FWD;
	msgArray[ARG_FLP_POWER]			= floatToEncodedByte(power, MAX_VALUE_PWM);
	msgArray[ARG_FLP_TIMEOUT]		= floatToEncodedByte(FLIP_TIMEOUT_DURATION_S, ENC_TIME);
	msgArray[ARG_FLP_TYPE]			= MOVETYPE_TWITCH;

	// Write the message to the desired dockID
	sendArrayI2C(dockID, msgArray, COM_I2C_MAX_MSG_LENGTH, I2C_CHECK_BYTE_NUM);
}

void dockAttach(robot_centerbody* robot, uint8_t dockID, uint8_t* msgArray)
{
	// Queue up the message array  
	clearMsgArray(msgArray);
	msgArray[MSG_I2C_COMMAND_BYTE]			= MSG_I2C_COMMAND_ATTACH;
	msgArray[ARG_ATT_SWEEP_I_TARGET]		= floatToEncodedByte(ATT_SWP_CUR,ENC_CURRENT_LOW);
	msgArray[ARG_ATT_SWEEP_VOLTAGE]			= floatToEncodedByte(ATT_SWP_VLT,ENC_VOLTAGE);
	msgArray[ARG_ATT_SWEEP_HOOP_MOTOR_PWR]	= floatToEncodedByte(robot->att_sweep_motor_power,MAX_VALUE_PWM);
	msgArray[ARG_ATT_SWEEP_HOOP_MOTOR_PWR_GROWTH_PERIOD] = floatToEncodedByte(ATT_SWP_MOT_PWR_GRW,ENC_TIME);
	msgArray[ARG_ATT_I_MAX]					= floatToEncodedByte(ATT_MLT_CUR, ENC_CURRENT);
	msgArray[ARG_ATT_V_START]				= floatToEncodedByte(ATT_MLT_VLT_STRT, ENC_VOLTAGE);
	msgArray[ARG_ATT_V_GROWTH_PERIOD]		= floatToEncodedByte(ATT_MLT_VLT_GRW_PER, ENC_TIME);
	msgArray[ARG_ATT_V_MAX]					= floatToEncodedByte(ATT_MLT_VLT_END, ENC_VOLTAGE);
	msgArray[ARG_ATT_INT_I_TARGET]			= floatToEncodedByte(robot->attach_melt_integrated_current, ENC_INTCUR);
	msgArray[ARG_ATT_SWEEP_DIRECTION]		= s2u_int8(robot->sweep_direction);

	// Write the message to the desired dock
	sendArrayI2C(dockID, msgArray, COM_I2C_MAX_MSG_LENGTH, I2C_CHECK_BYTE_NUM);
}

void dockDetach(robot_centerbody* robot, uint8_t dockID, uint8_t* msgArray)
{
	// Queue up the message array  
	clearMsgArray(msgArray);
	msgArray[MSG_I2C_COMMAND_BYTE]		= MSG_I2C_COMMAND_DETACH;
	msgArray[ARG_DET_DT1_INT_I]			= floatToEncodedByte(DET_DT1_INT_I			, ENC_INTCUR);
	msgArray[ARG_DET_DT1_MAX_CURRENT]	= floatToEncodedByte(DET_DT1_MAX_CURRENT	, ENC_CURRENT);
	msgArray[ARG_DET_DT1_MAX_VOLTAGE]	= floatToEncodedByte(DET_DT1_MAX_VOLTAGE	, ENC_VOLTAGE);
	msgArray[ARG_DET_DT2_MAX_CURRENT]	= floatToEncodedByte(DET_DT2_MAX_CURRENT	, ENC_CURRENT);
	msgArray[ARG_DET_DT2_MAX_VOLTAGE]	= floatToEncodedByte(DET_DT2_MAX_VOLTAGE	, ENC_VOLTAGE);
	msgArray[ARG_DET_HOOP_MOTOR_POWER]	= floatToEncodedByte(DET_HOOP_MOTOR_POWER	, MAX_VALUE_PWM);
	msgArray[ARG_DET_HOOP_MOTOR_MODE]	= DET_HOOP_MOTOR_MODE;
	msgArray[ARG_DET_ARM_MOTOR_TWITCH_PWR] = floatToEncodedByte(DET_ARM_MOTOR_TWITCH_PWR, MAX_VALUE_PWM);
	msgArray[ARG_DET_SWEEP_DIRECTION]	= s2u_int8(robot->sweep_direction);

	// Write the message to the desired dockID
	sendArrayI2C(dockID, msgArray, COM_I2C_MAX_MSG_LENGTH, I2C_CHECK_BYTE_NUM);
}

void dockPrepare(robot_centerbody* robot, uint8_t dockID, uint8_t prepare_direction, uint8_t* msgArray)
{
	// Queue up the message array  
	clearMsgArray(msgArray);
	msgArray[MSG_I2C_COMMAND_BYTE] = MSG_I2C_COMMAND_PREPARE;
	msgArray[ARG_PRP_DIRECTION] = s2u_int8(prepare_direction);

	// Write the message to the desired dockID
	sendArrayI2C(dockID, msgArray, COM_I2C_MAX_MSG_LENGTH, I2C_CHECK_BYTE_NUM);
}

void dockPuppet(robot_centerbody* robot, uint8_t dockID, int8_t hoopDir, float hoopPow, int8_t armDir, float armPow, uint8_t* msgArray)
{
	// Queue up the message array  
	clearMsgArray(msgArray);
	msgArray[MSG_I2C_COMMAND_BYTE]			= MSG_I2C_COMMAND_PUPPET;
	msgArray[ARG_PUP_HOOP_MOTOR_DIRECTION]	= s2u_int8(hoopDir);
	msgArray[ARG_PUP_HOOP_MOTOR_POWER]		= floatToEncodedByte(hoopPow,MAX_VALUE_PWM);
	msgArray[ARG_PUP_ARM_MOTOR_DIRECTION]	= s2u_int8(armDir);
	msgArray[ARG_PUP_ARM_MOTOR_POWER]		= floatToEncodedByte(armPow,MAX_VALUE_PWM);
	msgArray[ARG_PUP_TIMEOUT]				= floatToEncodedByte(PUPPET_TIMEOUT_DURATION_S,ENC_TIME);

	// Write the message to the desired dockID
	sendArrayI2C(dockID, msgArray, COM_I2C_MAX_MSG_LENGTH, I2C_CHECK_BYTE_NUM);
}