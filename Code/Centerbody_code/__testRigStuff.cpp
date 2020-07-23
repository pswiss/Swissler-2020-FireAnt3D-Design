/*///////////////////////////////////////////////////////////////////////
 Name:		__testRigStuff.h / .cpp
 Created:	04/05/2019
 Author:	Petras Swissler

 Purpose:	Include everything needed for the test rig
*////////////////////////////////////////////////////////////////////////
#include "__testRigStuff.h"

// High-Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void testRig_init(robot_centerbody* robot, LC_SENSEFORCE* loadCell)
{
	// Set Pins
	pinMode(PIN_do_COOLING_FAN, OUTPUT);		digitalWrite(PIN_do_COOLING_FAN, LOW);
	pinMode(RANDOMROTATIONCONTROLPIN, OUTPUT);	digitalWrite(RANDOMROTATIONCONTROLPIN, LOW);
	pinMode(PIN_LINAC_DIR, OUTPUT);				digitalWrite(PIN_LINAC_DIR, LOW);
	pinMode(PIN_LINAC_PWR, OUTPUT);				digitalWrite(PIN_LINAC_PWR, LOW);
	// Initialize the load cell
	loadCell->tare();
	delay(250);
	// Calibrate the linear actuator
	robot->zeroLinacDisplacement = getAvgReading(PIN_LINEAR_POSITION, READS_FOR_CALIBRATION);

	// Start on the zeroth test
	robot->currentTrial = 0;
	// Set the moving dock and the attach dock (same throughout all tests)
	robot->movement_dock = DOCK_0;
	robot->action_dock = DOCK_1;


	robot->state = TESTRIG_STATE_INIT;
}

void testRig_run(robot_centerbody* robot, LC_SENSEFORCE* loadCell, uint8_t* msgArray)
{
	testRigInputs(robot, loadCell);
	unsigned long nowTime;

	switch (robot->state)
	{
	case TESTRIG_STATE_INIT		:
		robot->currentTrial += 1;
		robot->state = TESTRIG_STATE_TOUCH;
		// Tell all docks to shutdown
		shutdownAllDocks(robot, msgArray);
		setFan(MOTORDIR_IDLE);	
		break;
	case TESTRIG_STATE_TOUCH	:
		// If Force is under some threshold:
		if (robot->meas_fsr_force < FLIP_PRESS_FORCE)
		{
			// Tell the moving dock to move
			dockFlip(robot, robot->movement_dock, FLIP_SPEED_PRESS, FLIP_DIRECTION_FORWARD, msgArray);
			// Tell the attaching dock to twitch if force is under some other threshold,
			// Otherwise stop the dock
			if (robot->meas_fsr_force < FLIP_TWITCH_FORCE_THRESHOLD)
			{
				dockTwitch(robot, robot->action_dock, ATT_TWITCH_POWER, msgArray);
			}
			else
			{
				dockStop(robot, robot->action_dock, msgArray);
			}
		}
		// Otherwise, shut down all movement and continue to the next state
		else
		{
			shutdownAllDocks(robot, msgArray);
			robot->state = TESTRIG_STATE_ATTACH;
			dockAttach(robot, robot->action_dock, msgArray);
			setFan(MOTORDIR_IDLE);
		}		
		break;
	case TESTRIG_STATE_ATTACH	:
		// If attaching dock status is not complete
		if (getDockInformation(robot, robot->action_dock, I2C_SEND_PHASE, msgArray)
			!= PHASE_COMPLETE)
		{
			dockAttach(robot, robot->action_dock, msgArray);
			// If Force is under some threshold:
			if (robot->meas_fsr_force < FLIP_PRESS_FORCE)
			{
				// Tell the moving dock to move
				dockFlip(robot, robot->movement_dock, ATT_SPEED_PRESS, ATT_DIRECTION_FORWARD, msgArray);
			}
			// Otherwise, tell the moving dock to stay still
			else
			{
				dockStop(robot, robot->movement_dock, msgArray);
			}
			// Grab info from docks
			robot->attachDock_dockCurrent = getDockInformation(robot, robot->action_dock, I2C_SEND_IDOCK, msgArray);
			robot->attachDock_intCurrent = getDockInformation(robot, robot->action_dock, I2C_SEND_INT_I, msgArray);
		}
		// Otherwise, shut down all and move to next state
		else
		{
			shutdownAllDocks(robot, msgArray);
			robot->state = TESTRIG_STATE_COOL;
			robot->stateTimer0 = millis();
		}
		break;
	case TESTRIG_STATE_COOL		:
		// If time has not been met
		nowTime = millis();
		if (!checkTime(robot->stateTimer0, nowTime, COL_TIME_TO_COOL_S))
		{
			// Tell all docks to stay still
			shutdownAllDocks(robot, msgArray);
			// If time threshold 2 has been met:
			if (checkTime(robot->stateTimer0, nowTime, COL_TIME_BEFORE_FAN_ON))
			{
				// Turn on Fan
				setFan(MOTORDIR_FWD);
			}
			else
			{
				// Fan off
				setFan(MOTORDIR_IDLE);
			}
		}
		// Otherwise, turn off fan and move to next state
		else
		{
			shutdownAllDocks(robot, msgArray);
			setFan(MOTORDIR_IDLE);
		}
		break;
	case TESTRIG_STATE_PULL		:
		// Tell all docks to shutdown.
		shutdownAllDocks(robot, msgArray);
		// If LC force threshold not met:
		if (fabs(robot->lastForce) < PULL_FORCE)
		{
			// Pull with linear actuator
			runLinac(MOTORDIR_REV, PULL_LINAC_STRENGTH);
		}
		// Otherwise, turn off the linear actuator and move to the next state
		else
		{
			runLinac(MOTORDIR_BRAKE, 0);
			robot->state = TESTRIG_STATE_HOLD;
			shutdownAllDocks(robot, msgArray);
			robot->stateTimer0 = millis();
		}
		break;
	case TESTRIG_STATE_HOLD:
		// Do nothing until time is up
		shutdownAllDocks(robot, msgArray);
		// if time is up transition to next state
		if (checkTime(robot->stateTimer0, millis(), HOLD_TIME))
		{
			robot->state = TESTRIG_STATE_PREPARE;
		}
		break;
	case TESTRIG_STATE_PREPARE	:
		// Turn off all docks
		shutdownAllDocks(robot, msgArray);
		// If loadcell is above a certain force
		if (robot->lastForce < 0)
		{
			// Move linear actuator forward
			runLinac(MOTORDIR_FWD, RTN_LINAC_STRENGTH);
		}
		// Otherwise, turn off the linear actuator and move to the next state
		else
		{
			runLinac(MOTORDIR_IDLE, 0);
			robot->state = TESTRIG_STATE_DETACH;
			dockDetach(robot, robot->action_dock, msgArray);
			robot->stateTimer0 = millis();
		}
		break;
	case TESTRIG_STATE_DETACH	:
		// If the attaching docks state is not complete
		if (getDockInformation(robot, robot->action_dock, I2C_SEND_PHASE, msgArray)
			== PHASE_COMPLETE)
		{
			// Tell the dock to detach
			dockDetach(robot, robot->action_dock, msgArray);
			// If the time is above some threshold
			if (checkTime(robot->stateTimer0, millis(), DET_TIME_BEFORE_PULL))
			{
				// If the centerbody force is below some threshold
				if (fabs(robot->meas_fsr_force) < DET_PULL_FORCE_FSR)
				{
					// Pull with the movement dock
					dockFlip(robot, robot->movement_dock, DET_PULL_POWER, MOTORDIR_REV, msgArray);
				}
				else
				{
					dockStop(robot, robot->movement_dock, msgArray);
				}
			} 
		}
		// Otherwise, turn off all docks and move to the next state
		else
		{
			shutdownAllDocks(robot, msgArray);
			robot->state = TESTRIG_STATE_RETRACT;
		}
		break;
	case TESTRIG_STATE_RETRACT	:
		// If the linear actuator is not at (some threshold to) 0
		if (fabs(robot->linacDisplacement))
		{
			// Move the linear actuator towards 0
			runLinac(-getFloatSign(robot->linacDisplacement), RETRACT_LINAC_POWER);
		}
		// Otherwise, Turn off the linear actuator and continue to next state
		else
		{
			shutdownAllDocks(robot, msgArray);
			robot->state = TESTRIG_STATE_PIVOT;
			dockPrepare(robot, robot->action_dock, robot->step_direction, msgArray);
		}		
		break;
	case TESTRIG_STATE_PIVOT	:
		// If the attaching docks state is not complete
		if (getDockInformation(robot, robot->action_dock, I2C_SEND_PHASE, msgArray)
			== PHASE_COMPLETE)
		{
			// Tell the dock to pivot
			dockPrepare(robot, robot->action_dock, robot->step_direction, msgArray);
		}
		// Otherwise, turn off all docks and move to the next state
		else
		{
			shutdownAllDocks(robot, msgArray);
			robot->state = TESTRIG_STATE_COOL2;
			robot->stateTimer0 = millis();
		}
		break;
	case TESTRIG_STATE_COOL2	:
		// If time has not been met
		nowTime = millis();
		if (!checkTime(robot->stateTimer0, nowTime, CL2_TIME_TO_COOL_S))
		{
			// Tell all docks to stay still
			shutdownAllDocks(robot, msgArray);
			// If time threshold 2 has been met:
			if (checkTime(robot->stateTimer0, nowTime, CL2_TIME_BEFORE_FAN_ON))
			{
				// Turn on Fan
				setFan(MOTORDIR_FWD);
			}
			else
			{
				// Fan off
				setFan(MOTORDIR_IDLE);
			}
		}
		// Otherwise, turn off fan and move to next state
		else
		{
			shutdownAllDocks(robot, msgArray);
			setFan(MOTORDIR_IDLE);
			robot->state = TESTRIG_STATE_COMPLETE;
		}				
		break;
	case TESTRIG_STATE_COMPLETE:
		shutdownAllDocks(robot, msgArray);
		Serial.println("--");// TRIAL COMPLETE-------------------------- - ");
		// Re-Calibrate sensors
		calibrateFSR(robot);
		loadCell->tare();
		// Continue to next trial
		robot->state = TESTRIG_STATE_INIT;
		break;
	default:
		robot->state = TESTRIG_STATE_COMPLETE;
		// Tell all docks to shut down
		break;
	}

	reportData(robot);
}
// Low Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void senseLCForce(robot_centerbody* robot, LC_SENSEFORCE *loadCell) {
	// Get Load Cell Reading
	float sForce1 = loadCell->get_units();
	if (fabs(sForce1) > SEN_LC_ERR_HI) { sForce1 = robot->lastForce; }

	float sForce2 = loadCell->get_units();
	if (fabs(sForce2) > SEN_LC_ERR_HI) { sForce2 = robot->lastForce; }

	robot->lastForce = getMidof3(robot->lastForce, sForce1, sForce2);
}
void runLinac(int8_t direction, uint8_t power)
{
	// Pin 1 is PWM, Pin2 is DIR
	////////////////////////////////////////////////////////////////////////
	switch (direction)
	{
	case MOTORDIR_FWD:
		analogWrite(PIN_LINAC_PWR, power);
		digitalWrite(PIN_LINAC_DIR, HIGH);
		break;
	case MOTORDIR_REV:
		analogWrite(PIN_LINAC_PWR, power);
		digitalWrite(PIN_LINAC_DIR, LOW);
		break;
	default:
		analogWrite(PIN_LINAC_PWR, LOW);
		digitalWrite(PIN_LINAC_DIR, LOW);
	}
	return;
}
void senseLinacDisplacement(robot_centerbody* robot)
{
	// Find difference between measured ADC and known-zero ADC
	signed int measADC = getAvgReading(PIN_LINEAR_POSITION, READS_FOR_SENSING) - (robot->zeroLinacDisplacement);
	// Convert from ADC to location
	robot->linacDisplacement = measADC * CALIB_MM_PER_ADC;
}
void reportData(robot_centerbody* robot)
{
	Serial.print(millis());					Serial.print("\t");
	Serial.print(robot->currentTrial);		Serial.print("\t");
	Serial.print(robot->state);				Serial.print("\t");
	//Serial.print(robot->lastForce);			Serial.print("\t");
	//Serial.print(robot->linacDisplacement);	Serial.print("\t");
	Serial.print(robot->attachDock_dockCurrent);Serial.print("\t");
	Serial.print(robot->attachDock_intCurrent); Serial.print("\t");

	Serial.println();
}
void testRigInputs(robot_centerbody* robot, LC_SENSEFORCE* loadCell)
{
	// Measure Load Cell
	senseLCForce(robot, loadCell);
	// Measure Position
	senseLinacDisplacement(robot);
}
void setFan(uint8_t mode)
{
	// Set the fan to be on or off based on input "mode"
	switch (mode)
	{
	case MOTORDIR_IDLE:
	case MOTORDIR_BRAKE:
		digitalWrite(PIN_do_COOLING_FAN, LOW);
		break;
	default:
		digitalWrite(PIN_do_COOLING_FAN, HIGH);
	}
}