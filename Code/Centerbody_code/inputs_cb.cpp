/*///////////////////////////////////////////////////////////////////////
 Name:		startup.h / .cpp
 Created:	3/30/2019
 Author:	Petras Swissler

 Notes:		IMU-related functions are lifted from the Spartfun examples
*////////////////////////////////////////////////////////////////////////
#include "inputs_cb.h"
// High Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void inputs(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray)
{
	updateMeasurementTimes(robot);
	measureFSR(robot);
	getDockStatesAndPhases(robot, msgArray);

#ifdef IMU_ENABLE
	measureFlipSpeed(robot, imu);
	measureOrientation(robot, imu);
	measureHeading(robot, imu);
#endif // !IMU_ENABLE
}

// Low Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void updateMeasurementTimes(robot_centerbody* robot)
{
	robot->time_prev_measurement = robot->time_this_measurement;
	robot->time_this_measurement = millis();
}

void getFSRCalib(robot_centerbody* robot)
{
	switch (robot->movement_dock)
	{
	case DOCK_0:
		switch (robot->step_direction)
		{
		case MOTORDIR_FWD:
			CALIB_GRAMS_PER_ADC = CAL_G_PER_ADC_0_POS;
			break;
		case MOTORDIR_REV:
			CALIB_GRAMS_PER_ADC = CAL_G_PER_ADC_0_NEG;
			break;
		default:
			CALIB_GRAMS_PER_ADC = 1;
		}
		break;
	case DOCK_1:
		switch (robot->step_direction)
		{
		case MOTORDIR_FWD:
			CALIB_GRAMS_PER_ADC = CAL_G_PER_ADC_1_POS;
			break;
		case MOTORDIR_REV:
			CALIB_GRAMS_PER_ADC = CAL_G_PER_ADC_1_NEG;
			break;
		default:
			CALIB_GRAMS_PER_ADC = 1;
		}
		break;
	case DOCK_2:
		switch (robot->step_direction)
		{
		case MOTORDIR_FWD:
			CALIB_GRAMS_PER_ADC = CAL_G_PER_ADC_2_POS;
			break;
		case MOTORDIR_REV:
			CALIB_GRAMS_PER_ADC = CAL_G_PER_ADC_2_NEG;
			break;
		default:
			CALIB_GRAMS_PER_ADC = 1;
		}
		break;
	default:
		CALIB_GRAMS_PER_ADC = 1;
	}
}

void measureFSR(robot_centerbody* robot)
{
	digitalWrite(PIN_DO_ENABLE_FSR, HIGH); 
	getFSRCalib(robot);
	float avgAdcA = getAvgReading(PIN_ASEN_FSR_A, READS_FOR_SENSING);
	float avgAdcB = getAvgReading(PIN_ASEN_FSR_B, READS_FOR_SENSING);
	robot->meas_fsr_force = expFilter(
		(((avgAdcA - robot->calib_adc0_fsr_A) - (avgAdcB - robot->calib_adc0_fsr_B)) * CALIB_GRAMS_PER_ADC),
		robot->meas_fsr_force, FILT_EXP_ALPHA_FSR);
	digitalWrite(PIN_DO_ENABLE_FSR, LOW);
	if (robot->pup_mode == 'i')
	{
		Serial.print(F("[")); Serial.print(avgAdcA); Serial.print(F(",")); Serial.print(avgAdcB); Serial.print(F("]"));
	}
}

void calibrateFSR(robot_centerbody* robot)
{
	digitalWrite(PIN_DO_ENABLE_FSR, HIGH);
	robot->calib_adc0_fsr_A = getAvgReading(PIN_ASEN_FSR_A, READS_FOR_CALIBRATION);
	robot->calib_adc0_fsr_B = getAvgReading(PIN_ASEN_FSR_B, READS_FOR_CALIBRATION);
	// Reset the force measurement to bypass the exponential filter
	robot->meas_fsr_force = 0;
}

void measureOrientation(robot_centerbody* robot, IMUpn* imu)
{
	if (imu->accelAvailable())
	{
		// To read from the accelerometer, first call the
		// readAccel() function. When it exits, it'll update the
		// ax, ay, and az variables with the most current data.
		imu->readAccel();
		// Flip x to have RHR
		robot->meas_grav_x = -imu->calcAccel(imu->ax);
		robot->meas_grav_y = imu->calcAccel(imu->ay);
		robot->meas_grav_z = imu->calcAccel(imu->az);
	}
}

void measureFlipSpeed(robot_centerbody* robot, IMUpn* imu)
{
	// Variable declarations
	float rx, ry, rz;
	// Logic
	if (imu->gyroAvailable())
	{
		// To read from the gyroscope,  first call the
		// readGyro() function. When it exits, it'll update the
		// gx, gy, and gz variables with the most current data.
		imu->readGyro();
		// Flip x to have RHR
		rx = -imu->calcGyro(imu->gx) - robot->calib_gyro_x;
		ry =  imu->calcGyro(imu->gy) - robot->calib_gyro_y;
		rz =  imu->calcGyro(imu->gz) - robot->calib_gyro_z;
		robot->calc_flip_speed = sqrt(rx * rx + ry * ry + rz * rz);
	}
}

void calibrateGyro(robot_centerbody* robot, IMUpn* imu)
{
	robot->calib_gyro_x = 0;
	robot->calib_gyro_y = 0;
	robot->calib_gyro_z = 0;
	const uint8_t numReads = 200;
	for (uint8_t ii = 0; ii < numReads; ii++)
	{
		while (!imu->gyroAvailable())
		{
			imu->readGyro();
			robot->calib_gyro_x += (-1) * imu->calcGyro(imu->gx);
			robot->calib_gyro_y += (-1) * imu->calcGyro(imu->gy);
			robot->calib_gyro_z += (-1) * imu->calcGyro(imu->gz);

		}
	}

	robot->calib_gyro_x = robot->calib_gyro_x / numReads;
	robot->calib_gyro_y = robot->calib_gyro_y / numReads;
	robot->calib_gyro_z = robot->calib_gyro_z / numReads;
}

void measureHeading(robot_centerbody* robot, IMUpn* imu)
{
	// Variable Declarations
	//float DECLINATION, mx, my, mz, heading;

	// Logic
	if (imu->magAvailable())
	{
		imu->readMag();
		robot->meas_mag_x = -imu->calcMag(imu->mx);
		robot->meas_mag_y =  imu->calcMag(imu->my);
		robot->meas_mag_z =  imu->calcMag(imu->mz);
		/*
		// To read from the magnetometer, first call the
		// readMag() function. When it exits, it'll update the
		// mx, my, and mz variables with the most current data.
		imu->readMag();

		// http://www.ngdc.noaa.gov/geomag-web/#declination
		// 2019-05-22: 3° 54' W  ± 0° 22'  changing by  0° 2' W per year 
		DECLINATION = -3.9;

		mx = -imu->calcMag(imu->my);
		my = -imu->calcMag(imu->mx);
		mz = imu->calcMag(imu->mz);

		heading;
		if (my == 0)
			heading = (mx < 0) ? PI : 0;
		else
			heading = atan2(mx, my);

		heading -= DECLINATION * PI / 180;

		if (heading > PI) heading -= (2 * PI);
		else if (heading < -PI) heading += (2 * PI);

		// Convert everything from radians to degrees:
		heading *= 180.0 / PI;

		robot->heading = heading;*/

	}
}

void getDockStatesAndPhases(robot_centerbody* robot, uint8_t* msgArray)
{
	// Attach Dock
	robot->dockState_action =	getDockInformation(robot, robot->action_dock,	I2C_SEND_STATE, msgArray);
	robot->dockPhase_action =	getDockInformation(robot, robot->action_dock,	I2C_SEND_PHASE, msgArray);
	// Move Dock
	robot->dockState_move =		getDockInformation(robot, robot->movement_dock, I2C_SEND_STATE, msgArray);
	robot->dockPhase_move =		getDockInformation(robot, robot->movement_dock, I2C_SEND_PHASE, msgArray);
	// Passive Dock
	robot->dockState_passive =	getDockInformation(robot, robot->passive_dock,	I2C_SEND_STATE, msgArray);
	robot->dockPhase_passive =	getDockInformation(robot, robot->passive_dock,	I2C_SEND_PHASE, msgArray);
}

uint8_t findClosestDockToDesiredDirection(float dir_x, float dir_y, float dir_z, robot_centerbody* robot)
{
	// Variable Declarations -----
	float calcAngle, evalAngle;
	uint8_t closestDock;

	// Logic ---------------------
	// Start by assuming that dock 0 is the closest
	closestDock = DOCK_0;
	calcAngle = fabs(vec_angle(dir_x, dir_y, dir_z, robot->vec_d0_x, robot->vec_d0_y, robot->vec_d0_z));
	evalAngle = fabs(vec_angle(dir_x, dir_y, dir_z, robot->vec_d1_x, robot->vec_d1_y, robot->vec_d1_z));
	if (calcAngle > evalAngle)
	{
		closestDock = DOCK_1;
		calcAngle = evalAngle;
	}
	evalAngle = fabs(vec_angle(dir_x, dir_y, dir_z, robot->vec_d2_x, robot->vec_d2_y, robot->vec_d2_z));
	if (calcAngle > evalAngle)
	{
		closestDock = DOCK_2;
		calcAngle = evalAngle;
	}
	return closestDock;
}

uint8_t findFurthestDockToDesiredDirection(float dir_x, float dir_y, float dir_z, robot_centerbody* robot)
{
	return findClosestDockToDesiredDirection(-dir_x, -dir_y, -dir_z, robot);
}

uint8_t findHightestDock(robot_centerbody* robot)
{
	return findClosestDockToDesiredDirection(robot->meas_grav_x, robot->meas_grav_y, robot->meas_grav_z, robot);
}

uint8_t findLowestDock(robot_centerbody* robot)
{
	return findFurthestDockToDesiredDirection(robot->meas_grav_x, robot->meas_grav_y, robot->meas_grav_z, robot);
}

