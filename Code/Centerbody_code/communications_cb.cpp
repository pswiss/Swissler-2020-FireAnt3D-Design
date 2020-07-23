/*///////////////////////////////////////////////////////////////////////
 Name:		communications.h / .cpp
 Created:	3/30/2019
 Author:	Petras Swissler

 Purpose:	Handle all communications to and from the docks
*////////////////////////////////////////////////////////////////////////
#include "communications_cb.h"

// High Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float getDockInformation(robot_centerbody* robot, uint8_t dockID, uint8_t whatInformation, uint8_t* msgArray)
{
	clearMsgArray(msgArray);
	msgArray[MSG_I2C_COMMAND_BYTE]	= MSG_I2C_COMMAND_REPORT;
	msgArray[ARG_RPT_WHICH_ONE]		= whatInformation;

	sendArrayI2C(dockID, msgArray, COM_I2C_MAX_MSG_LENGTH, I2C_CHECK_BYTE_NUM);
	requestArrayI2C(dockID, NUMBYTES_FROM_REPORT, msgArray);
	
	return returnInformationInterpretation(whatInformation, msgArray);
}

// Mid Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float returnInformationInterpretation(uint8_t whatInformation, uint8_t* msgArray)
{
	// Most reports simply convert four chars to float, but others require more work
	switch (whatInformation)
	{
		case I2C_SEND_IDOCK			:
			return fourCharsToFloat(msgArray);
			break;
		case I2C_SEND_IHOOP			:
			return fourCharsToFloat(msgArray);
			break;
		case I2C_SEND_LEVELS		:
			return 1 * (msgArray[0] == 1) + 2 * (msgArray[1] == 1) + 4 * (msgArray[2] == 1);
			break;
		case I2C_SEND_IN_COM		:
			return msgArray[0];
			break;
		case I2C_SEND_VDOCK			:
			return fourCharsToFloat(msgArray);
			break;
		case I2C_SEND_INT_I			:
			return fourCharsToFloat(msgArray);
			break;
		case I2C_SEND_HMOT_PWR		:
			return fourCharsToFloat(msgArray);
			break;
		case I2C_SEND_AMOT_PWR		:
			return fourCharsToFloat(msgArray);
			break;
		case I2C_SEND_STATUS		:
			return msgArray[0];
			break;
		case I2C_SEND_CALC_HOOP_POS	:
			return fourCharsToFloat(msgArray);
			break;
		case I2C_SEND_CALC_ARM_POS	:
			return fourCharsToFloat(msgArray);
			break;
		
		case I2C_SEND_PHASE			:
			return msgArray[0];
			break;
		case I2C_SEND_STATE:
			return msgArray[0];
			break;
		default:
			return 0;
	}
}

void writeCheckByte(uint8_t checkByteNum, uint8_t* msgArray)
{
	// Sum preceeding bytes and write the value into the appropriate entry
	uint8_t checkVal = 0;
	for (uint8_t ii = 0; ii < (checkByteNum-1); ii++)
	{	checkVal = checkVal + msgArray[ii];	}
	msgArray[checkByteNum] = checkVal;
}

// Low Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void writeArrayI2C(uint8_t device, uint8_t numbytes, uint8_t *outArray)
{
	Wire.beginTransmission(device); // transmit to device 
	Wire.write(outArray, numbytes);
	Wire.endTransmission();    // stop transmitting
}

void requestArrayI2C(uint8_t device, uint8_t numbytes, uint8_t *inArray)
{
	// Max number of requested bytes must be less than 255. I2C default buffer size is 32
	clearMsgArray(inArray);
	Wire.requestFrom(device, numbytes);
	uint8_t ii = 0; 
	while (Wire.available() > 0) 
	{ 
		inArray[ii] = Wire.read(); 
		ii++;
	}
}
void clearMsgArray(uint8_t* msgArray)
{	
	for (uint8_t ii = 0; ii < COM_I2C_MAX_MSG_LENGTH; ++ii)
	{	msgArray[ii] = 0;	}
}

void sendArrayI2C(uint8_t dockID, uint8_t* msgArray, uint8_t msg_length, uint8_t check_byte_number)
{
	writeCheckByte(check_byte_number, msgArray);
	writeArrayI2C(dockID, msg_length, msgArray);
}
