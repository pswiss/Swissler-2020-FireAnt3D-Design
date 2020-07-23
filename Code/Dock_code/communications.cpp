 /*///////////////////////////////////////////////////
Name:		communications.cpp
Created :	3 / 25 / 2019
Author :	Petras Swissler

Purpose :	Handle all necessary communication functions
*/////////////////////////////////////////////////////
#include "communications.h"

// Main Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void handle_i2c_communications(dock_robot* robot, uint8_t* flag_request, uint8_t* flag_receive, uint8_t* in_msg)
{
	// Update the robot command and configuration data if 
	// a message has been recieved
	if (*flag_receive)
	{
		// Clear the flag
		*flag_receive = false;

		robot->time_timeout_start = robot->time_this_measurement;

		// Check if have a new commanded behavior. If so, the phase should be reset to INIT
		if (robot->last_command_behavior != robot->command_behavior)
		{
			robot->last_command_behavior = robot->command_behavior;
			robot->state_phase = PHASE_INIT;
		}
	}
	// I just sent some data
	if (*flag_request)
	{
		// Clear the flag
		*flag_request = false;
	}
}

void handleDockCommunications(dock_robot* robot, unsigned long* dd_comm_array, uint8_t* comm_index)
{
	// *************** TODO: IMPLEMENT ***************
}

// Implementation-specific functions ~~~~~~~~~~~~~~~~~~
// Take recieved message data and update robot commands and parameters
void interpret_i2c_Data(uint8_t* inMsg, dock_robot* robot)
{
	// Only operate on the message if the message is valid
	if (i2cCheckByte(I2C_CHECK_BYTE_NUM, inMsg))
	{
		//Serial.println("Pass");
		// Keep track of last commanded behavior for report functions
		uint8_t temp_commanded_behavior = robot->command_behavior;
		robot->command_behavior = inMsg[MSG_I2C_COMMAND_BYTE];

		switch (robot->command_behavior)
		{
			// Note: because all inputs are 8-bit chars, some modifications are made to the raw messages
		case MSG_I2C_COMMAND_STOP:
			// No special arguements, simply shut down any outputs immediately
			shutdown_all(robot);
			break;
		case MSG_I2C_COMMAND_DETACH:
			robot->config_DET_DT1_integrated_I_target = encodedByteToFloat(inMsg[ARG_DET_DT1_INT_I], ENC_INTCUR);
			robot->config_DET_DT1_max_I =				encodedByteToFloat(inMsg[ARG_DET_DT1_MAX_CURRENT], ENC_CURRENT);
			robot->config_DET_DT1_max_V =				encodedByteToFloat(inMsg[ARG_DET_DT1_MAX_VOLTAGE], ENC_VOLTAGE);
			robot->config_DET_DT2_max_I =				encodedByteToFloat(inMsg[ARG_DET_DT2_MAX_CURRENT], ENC_CURRENT);
			robot->config_DET_DT2_max_V =				encodedByteToFloat(inMsg[ARG_DET_DT2_MAX_VOLTAGE], ENC_VOLTAGE);
			robot->config_DET_hoop_motor_power =		encodedByteToFloat(inMsg[ARG_DET_HOOP_MOTOR_POWER], MAX_VALUE_PWM);
			robot->config_DET_hoop_motor_mode =			inMsg[ARG_DET_HOOP_MOTOR_MODE];
			robot->config_DET_arm_twitch_power =		encodedByteToFloat(inMsg[ARG_DET_ARM_MOTOR_TWITCH_PWR], MAX_VALUE_PWM);
			robot->config_DET_sweep_direction =			u2s_int8(inMsg[ARG_DET_SWEEP_DIRECTION]);
			break;
		case MSG_I2C_COMMAND_ATTACH:
			robot->config_SWP_I_target =				encodedByteToFloat(inMsg[ARG_ATT_SWEEP_I_TARGET], ENC_CURRENT_LOW);
			robot->config_SWP_voltage =					encodedByteToFloat(inMsg[ARG_ATT_SWEEP_VOLTAGE], ENC_VOLTAGE);
			robot->config_SWP_hoop_motor_power =		encodedByteToFloat(inMsg[ARG_ATT_SWEEP_HOOP_MOTOR_PWR], MAX_VALUE_PWM);
			robot->config_SWP_hoop_motor_pwr_growth_period = encodedByteToFloat(inMsg[ARG_ATT_SWEEP_HOOP_MOTOR_PWR_GROWTH_PERIOD], ENC_TIME);
			robot->config_ATT_I_max =					encodedByteToFloat(inMsg[ARG_ATT_I_MAX], ENC_CURRENT);
			robot->config_ATT_V_start =					encodedByteToFloat(inMsg[ARG_ATT_V_START], ENC_VOLTAGE);
			robot->config_ATT_V_growth_period =			encodedByteToFloat(inMsg[ARG_ATT_V_GROWTH_PERIOD], ENC_TIME);
			robot->config_ATT_V_max =					encodedByteToFloat(inMsg[ARG_ATT_V_MAX], ENC_VOLTAGE);
			robot->config_ATT_integrated_I_target =		encodedByteToFloat(inMsg[ARG_ATT_INT_I_TARGET], ENC_INTCUR);
			robot->config_ATT_sweep_direction =			u2s_int8(inMsg[ARG_ATT_SWEEP_DIRECTION]);
			break;
		case MSG_I2C_COMMAND_PREPARE:
			robot->config_PRP_direction =				u2s_int8(inMsg[ARG_PRP_DIRECTION]);
			break;
		case MSG_I2C_COMMAND_FLIP:
			robot->config_FLP_direction =				u2s_int8(inMsg[ARG_FLP_DIRECTION]);
			robot->config_FLP_power =					encodedByteToFloat(inMsg[ARG_FLP_POWER], MAX_VALUE_PWM);
			robot->config_FLP_timeout =					encodedByteToFloat(inMsg[ARG_FLP_TIMEOUT], ENC_TIME);
			robot->config_FLP_type =					inMsg[ARG_FLP_TYPE];
			break;
		case MSG_I2C_COMMAND_PUPPET:
			robot->config_PUP_hoop_direction =			u2s_int8(inMsg[ARG_PUP_HOOP_MOTOR_DIRECTION]);
			robot->config_PUP_hoop_power =				encodedByteToFloat(inMsg[ARG_PUP_HOOP_MOTOR_POWER], MAX_VALUE_PWM);
			robot->config_PUP_arm_direction =			u2s_int8(inMsg[ARG_PUP_ARM_MOTOR_DIRECTION]);
			robot->config_PUP_arm_power =				encodedByteToFloat(inMsg[ARG_PUP_ARM_MOTOR_POWER], MAX_VALUE_PWM);
			robot->config_PUP_timeout =					encodedByteToFloat(inMsg[ARG_PUP_TIMEOUT], ENC_TIME);
			break;
		case MSG_I2C_COMMAND_REPORT:
			robot->config_RPT_whatToReport =			inMsg[ARG_RPT_WHICH_ONE];
			robot->command_behavior =					temp_commanded_behavior;
			break;
		case MSG_I2C_COMMAND_SENDMSG:
			robot->config_RPT_whatToSend =				inMsg[ARG_SND_OUTMSG];
			robot->command_behavior =					temp_commanded_behavior;
			break;
		case MSG_I2C_COMMAND_LED:
			// This command is primarily meant for debugging, so all logic is simply executed here
			if (inMsg[ARG_LED_LEVEL] == HIGH)	{	ledON();	}
			else								{	ledOFF();	}
			robot->command_behavior = temp_commanded_behavior;
			break;
		default:
			// Something went wrong. Revert to last behavior
			robot->command_behavior = temp_commanded_behavior;
			#ifdef DEBUG
				Serial.println("err:cinterp");
			#endif // DEBUG
			break;
		}
	}
	// Otherwise do nothing
#ifdef DEBUG
	else
	{
		Serial.print("Fail");
		printArray(inMsg, I2C_CHECK_BYTE_NUM);
	}
#endif
}

// Send data in response to a request
void sendRequestedData_i2c(dock_robot* robot, uint8_t* dataOut)
{
	// Use existing array to minimize memory usage
	dataOut[0] = 0;
	dataOut[1] = 0;
	dataOut[2] = 0;
	dataOut[3] = 0;
	//uint8_t dataOut[4] = { 0 };
	switch (robot->config_RPT_whatToReport)
	{
		case I2C_SEND_IDOCK		:
			floatToFourChars(robot->meas_current_dock, dataOut);
			break;
		case I2C_SEND_IHOOP		:
			floatToFourChars(robot->meas_current_hoop, dataOut);
			break;
		case I2C_SEND_LEVELS	:
			dataOut[0] = (uint8_t)robot->sen_dockLevel;
			dataOut[1] = (uint8_t)robot->sen_hoop_level;
			dataOut[2] = (uint8_t)robot->sen_armLim;
			dataOut[3] = 0;
			break;
		case I2C_SEND_IN_COM	:
			dataOut[0] = robot->com_dock_communication_in;
			dataOut[1] = robot->com_dock_communication_out;
			dataOut[2] = 0;
			dataOut[3] = 0;
			break;
		case I2C_SEND_VDOCK		:
			floatToFourChars(robot->meas_dock_voltage, dataOut);
			break;
		case I2C_SEND_INT_I		:
			floatToFourChars(robot->calc_integrated_current, dataOut);
			break;
		case I2C_SEND_HMOT_PWR	:
			floatToFourChars(robot->out_hoop_power, dataOut);
			break;
		case I2C_SEND_AMOT_PWR	:
			floatToFourChars(robot->out_arm_power, dataOut);
			break;
		case I2C_SEND_STATUS	:
			dataOut[0] = robot->status;
			break;
		case I2C_SEND_CALC_ARM_POS	:
			floatToFourChars(robot->calc_approx_arm_position, dataOut);
			break;
		case I2C_SEND_CALC_HOOP_POS	:
			floatToFourChars(robot->calc_approx_hoop_position, dataOut);
			break;
		case I2C_SEND_VBAT_MINUS:
			// Slightly dangerous. Need to be careful of when centerbody requests
			// the voltage of the negative battery terminal; should never be requested 
			// if the robot is doing anything that requires processing (should only 
			// be used if the robot is stopped.
			digitalWrite(PIN_DO_ENABLE_SEN_VBAT_MINUS, HIGH);
			delay(5);
			floatToFourChars(getMaxofN(PIN_ASEN_VBAT_MINUS, READS_FOR_CALIBRATION) * robot->calib_vbatminus_per_adc,dataOut);
			delay(5);
			digitalWrite(PIN_DO_ENABLE_SEN_VBAT_MINUS, LOW);
			break;
		case I2C_SEND_PHASE:
			dataOut[0] = robot->state_phase;
			break;
		case I2C_SEND_STATE:
			dataOut[0] = robot->command_behavior;
			break;
		case I2C_SEND_MEMORY_USAGE:
			floatToFourChars((float)getFreeMemory(), dataOut);
		default:
			dataOut[0] = 69; dataOut[1] = 69; dataOut[2] = 69; dataOut[3] = 69;
			break;
	}	
	Wire.write(dataOut, 4);
}

// I2C low-level functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool i2cCheckByte(uint8_t checkByteNum, uint8_t* msgArray)
{
	// Sum preceeding bytes and check if the result matches the expected result
	uint8_t checkVal = 0;
	for (uint8_t ii = 0; ii < (checkByteNum-1); ii++)
	{	checkVal = checkVal + msgArray[ii];	}

	// Check is Valid
	if(msgArray[checkByteNum] == checkVal)
	{	return true;	}
	// Check is not Valid
	else
	{	return false;	}
}

// Read data into given array
void read_i2c_Data(uint8_t* inArray, int numbytes)
{
	uint8_t ii = 0;
	while(Wire.available() > 0)
	{
		inArray[ii] = Wire.read();
		ii++;
	}
}

// Clear everything in the i2c buffer
void flush_i2c_Buffer(void)
{
	while (Wire.available() > 0) 
	{
		Wire.read();
	}
}

// Hardware-Specific Functions~~~~~~~~~~~~~~~~~~~~~~~~~~
// Determine which i2c ID to identify with
void get_i2c_ID(dock_robot* robot)
{
	// Get the level of the battery minus level
	digitalWrite(PIN_DO_ENABLE_SEN_VBAT_MINUS, HIGH);
	delay(50);
	robot->meas_vbat_negative = (getMaxofN(PIN_ASEN_VBAT_MINUS, READS_FOR_CALIBRATION) * robot->calib_vbatminus_per_adc);
	//float measured_battery_minus_voltage = 12;
	delay(5);
	digitalWrite(PIN_DO_ENABLE_SEN_VBAT_MINUS, LOW);

	#ifdef DEBUG
		Serial.println();
		Serial.println();
		Serial.println(robot->meas_vbat_negative);
	#endif // DEBUG

	// Determine which bin this level falls into
	if (robot->meas_vbat_negative < 4.0)
	{	robot->self_ID = DOCK_0_I2C_ADDR;	}
	else
	{
		if (robot->meas_vbat_negative < 18.0)
		{	robot->self_ID = DOCK_1_I2C_ADDR;	}
		else
		{	robot->self_ID = DOCK_2_I2C_ADDR;	}
	}
}