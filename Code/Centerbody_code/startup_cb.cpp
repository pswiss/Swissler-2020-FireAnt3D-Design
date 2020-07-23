/*///////////////////////////////////////////////////////////////////////
 Name:		startup_cb
 Created:	3/30/2019
 Author:	Petras Swissler

 Purpose:	Initialize all aspects of the board
 Notes:		IMU-related functions are lifted from the Spartfun examples
*////////////////////////////////////////////////////////////////////////
#include "startup_cb.h"
// High Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void robot_init(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray)
{
	// Begin the I2C Communications
	Wire.begin();
	Wire.setClock(I2C_CLOCK);

	// Configure the board
	config_board();
	// Load hardware properties
	config_hw_properties(robot);

#ifdef LOAD_CALIB_FROM_EEPROM
	robot->calib_fsr = readFloatFromEEPROM(EEPROM_ADDRESS_FSR_SLOPE);
#else
	robot->calib_fsr = -20;
#endif
	// Start and configure the IMU 

	delay(1000);
	
#ifdef IMU_ENABLE
	beginIMU(imu);
	calibrateGyro(robot, imu);
#endif // !IMU_ENABLE

	// Calibrate the force sensitive resistors
	calibrateFSR(robot);
		
	// Tell all connected docks to be at the 'stop all' state
	shutdownAllDocks(robot, msgArray);
}

// Low Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Configure the pins on the board
void config_board(void)
{
	// Digital Outputs
	pinMode(PIN_DO_ENABLE_FSR, OUTPUT);
	digitalWrite(PIN_DO_ENABLE_FSR, LOW);
	// Undefined Pins

	// Disable unused perhipherals	
	//reducePowerConsumption();
}

void beginIMU(IMUpn* imu)
{
	// Example I2C Setup //
	uint8_t LSM9DS1_M	= 0x1E; // Would be 0x1C if SDO_M is LOW
	uint8_t LSM9DS1_AG	= 0x6B; // Would be 0x6A if SDO_AG is LOW

	// Before initializing the IMU, there are a few settings
	// we may need to adjust. Use the settings struct to set
	// the device's communication mode and addresses:
	imu->settings.device.commInterface = IMU_MODE_I2C;
	imu->settings.device.mAddress = LSM9DS1_M;
	imu->settings.device.agAddress = LSM9DS1_AG;

	uint8_t ii = 0;
	while (imu->begin() == 0)
	{
		delay(100);
		Serial.println(F("Try ")); Serial.println(ii++);
	}
}

void reducePowerConsumption(void)
{
	power_usart1_disable();
}

void config_hw_properties(robot_centerbody* robot)
{
	robot->vec_d0_x = -0.8660;
	robot->vec_d0_y = 0;
	robot->vec_d0_z = 0.5;
	robot->vec_d1_x = 0.0;
	robot->vec_d1_y = 0.0;
	robot->vec_d1_z = -1.0;
	robot->vec_d2_x = 0.8660;
	robot->vec_d2_y = 0.0;
	robot->vec_d2_z = 0.5;
}