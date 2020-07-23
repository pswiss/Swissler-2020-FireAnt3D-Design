// Author: Petras Swissler
// Purpose: Do all necessary behaviors for tethered debug test
// ************************************************************************
#include "debug_cb.h"

void debug_start(robot_centerbody* robot)
{
	robot->debug_state = 0;
	robot->timer_debug = millis();

	// Hard-code the dock roles
	robot->action_dock		= DOCK_0;
	robot->movement_dock	= DOCK_1;
	robot->passive_dock		= DOCK_2;

	Serial.println(F("ON"));
	printCompileInfo();
}

void userInputNextStep(robot_centerbody* robot, IMUpn* imu)
{
	bool success = false;
	int8_t dirChoose;
	robot->cb_phase_action = PHASE_ACTION_BEGIN;
	Serial.println();

	while (!success)
	{
		robot->ez_mode_active = false;

		Serial.println(F("(d)etach, (a)ttach, (f)lip, (h)oop move, ar(m) move, (p)repare, (i)nformation, automated (s)tep, e(z) step mode"));
		//Serial.println(F("Docks are 0, 1, 2"));
		robot->pup_mode = getSerialKeypress();
		
		switch (robot->pup_mode)
		{
		case 'z':
			// EZ step mode simply piggybacks on step mode, meaning that the only user input from here on out 
			// will be the exit program command (x), and the next attach dock that the autostep asks for
			robot->ez_mode_active = true;
			Serial.print(F("Define step behavior for first step.\r\n"));
			robot->pup_mode = 's';
			// Do not break, simply fall through to basic step declaration for the first step
		case 's':
			// Get the parameters
			do
			{
				// Direction
				Serial.print(F("Dir?1f2r"));
				if (getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON) == 1) 
				{ robot->astep_direction = MOTORDIR_FWD; }
				else { robot->astep_direction = MOTORDIR_REV; }
				// Detach Dock
				Serial.println();
				do 
				{
					Serial.print(F("Det?"));
					robot->astep_dock_detach = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON) + DOCK_0_I2C_ADDR; Serial.println();
				} 
				while ((robot->astep_dock_detach != DOCK_0)
					&& (robot->astep_dock_detach != DOCK_1)
					&& (robot->astep_dock_detach != DOCK_2));
				// Attach Dock
				do {
					Serial.print(F("Att?"));
					robot->astep_dock_attach = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON) + DOCK_0_I2C_ADDR; Serial.println();
				} while ((robot->astep_dock_attach != DOCK_0)
					&& (robot->astep_dock_attach != DOCK_1)
					&& (robot->astep_dock_attach != DOCK_2));
				// Movement Dock
				do {
					Serial.print(F("Mov?"));
					robot->astep_dock_move = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON) + DOCK_0_I2C_ADDR; Serial.println();
				} while (((robot->astep_dock_move != DOCK_0)
						&& (robot->astep_dock_move != DOCK_1)
						&& (robot->astep_dock_move != DOCK_2))
					|| (robot->astep_dock_move == robot->astep_dock_detach)
					|| (robot->astep_dock_move == robot->astep_dock_detach));
				// Passive Dock
				robot->astep_dock_passive = getAbsentByte(robot->astep_dock_detach, robot->astep_dock_move, DOCK_0, DOCK_1, DOCK_2);
				// Non-Start Enter
				do
				{
					Serial.print(F("Enter @"));
					robot->astep_phase = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
				} while ((robot->astep_phase % 10) != 0);
				// Special case: this is a reversal of previous step (i.e. 
				do {
					Serial.print(F("Is Forward Step? (1:0)"));
					if (getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON) == 1)  { robot->astep_isForwardMotion = 1; }
					else { robot->astep_isForwardMotion = -1; }
					Serial.println();

				} while (!((robot->astep_isForwardMotion == 1) || (robot->astep_isForwardMotion == -1)));
				// Report
				Serial.print(F("["));
				Serial.print(robot->astep_direction	  ); Serial.print(F(","));
				Serial.print(robot->astep_dock_attach  - DOCK_0_I2C_ADDR); Serial.print(F(","));
				Serial.print(robot->astep_dock_detach  - DOCK_0_I2C_ADDR); Serial.print(F(","));
				Serial.print(robot->astep_dock_move	   - DOCK_0_I2C_ADDR); Serial.print(F(","));
				Serial.print(robot->astep_dock_passive - DOCK_0_I2C_ADDR); Serial.print(F(","));
				Serial.print(robot->astep_phase); Serial.print(F("]"));
			} while (!checkYN(SER_IN_TIMEOUT));
			robot->stateTimer0 = millis();
			break;
		case 'd':
			// Get Detach Dock and assign
			Serial.print(F("Detach?"));			
			robot->pup_actionDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get Move Dock and assign
			Serial.print(F("Move?"));			
			robot->pup_moveDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get Passive Dock and assign
			Serial.print(F("Passive?"));			
			robot->pup_passiveDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get lifting force and assign
			Serial.print(F("Lift Force (g)?"));		
			robot->desired_force = getSerialFloat(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get Sweep Direction
			Serial.print(F("Sweep direction? 1f;2r"));	
			dirChoose = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			if (dirChoose == 1)		{	robot->sweep_direction = MOTORDIR_FWD;		}
			else					{	robot->sweep_direction = MOTORDIR_REV;		}
			// Get Pull Direction
			Serial.print(F("P?1f;2r"));	
			dirChoose = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			if (dirChoose == 1)		{	robot->pull_direction = MOTORDIR_FWD;		}
			else					{	robot->pull_direction = MOTORDIR_REV;		}
			break;
		case 'a':
			// Get Attach Dock and assign
			Serial.print(F("Attach?"));		
			robot->pup_actionDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get Move Dock and assign
			Serial.print(F("Move?"));		
			robot->pup_moveDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get Passive Dock and assign
			Serial.print(F("Passive?"));		
			robot->pup_passiveDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get press force and assign
			Serial.print(F("Press Force?"));		
			robot->desired_force = getSerialFloat(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get Sweep Direction
			Serial.print(F("Sweep Direction? 1f;2r"));dirChoose = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			if (dirChoose == 1)		{	robot->sweep_direction = MOTORDIR_FWD;	}
			else					{	robot->sweep_direction = MOTORDIR_REV;	}
			// Get press Direction
			Serial.print(F("Press direction? 1f;2r"));
			dirChoose = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			if (dirChoose == 1)		{	robot->pressDirection = MOTORDIR_FWD;	}
			else					{	robot->pressDirection = MOTORDIR_REV;	}
			// Get integrated current (removes need for reprogramming as often)
			Serial.print(F("Integrated Current?"));
			robot->attach_melt_integrated_current = bound(getSerialFloat(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON), 5, 46);
			Serial.println(robot->attach_melt_integrated_current);
			break;
		case 'f':
			// Get Attach Dock and assign
			Serial.print(F("N?"));
			robot->pup_actionDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get movement dock. No other docks matter
			Serial.print(F("Move?"));
			robot->pup_moveDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get Passive Dock and assign
			Serial.print(F("Passive?"));
			robot->pup_passiveDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get the target force and assign
			Serial.print(F("Force (g)?"));
			robot->desired_force = getSerialFloat(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get flip Direction
			Serial.print(F("Flip Direction 1f;2r?"));
			dirChoose = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			if (dirChoose == 1)		{	robot->flip_direction = MOTORDIR_FWD;	}
			else					{	robot->flip_direction = MOTORDIR_REV;	}
			break;
		case 'h':
			Serial.print(F("H?"));
			robot->pup_actionDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			Serial.print(F("?"));
			robot->pup_moveDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get Passive Dock and assign
			Serial.print(F("?"));
			robot->pup_passiveDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			Serial.print(F("Direction? 1f;2r"));
			robot->desired_force = getSerialFloat(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			Serial.print(F("Time?"));
			robot->dock_movement_timeout = getSerialFloat(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			break;
		case 'm':
			Serial.print(F("Arm?"));
			robot->pup_actionDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			Serial.print(F("?"));
			robot->pup_moveDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get Passive Dock and assign
			Serial.print(F("?"));
			robot->pup_passiveDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			Serial.print(F("Direction? 1f;2r"));
			robot->desired_force = getSerialFloat(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			Serial.print(F("T?"));
			robot->dock_movement_timeout = getSerialFloat(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			break;
		case 'p':
			// Get Attach Dock and assign
			Serial.print(F("Prepare?"));
			robot->pup_actionDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get movement dock. No other docks matter
			Serial.print(F("?"));
			robot->pup_moveDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get Passive Dock and assign
			Serial.print(F("?"));
			robot->pup_passiveDock_number = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			// Get prepare Direction (same as flip
			Serial.print(F("Direction? 1f;2r?"));
			dirChoose = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON); Serial.println();
			if (dirChoose == 1)		{	robot->flip_direction = MOTORDIR_FWD;	}
			else					{	robot->flip_direction = MOTORDIR_REV;	}
			break;
		case 'i':
			Serial.print(F("!"));
			robot->pup_actionDock_number	   = 0;
			robot->pup_moveDock_number		   = 1;
			robot->pup_passiveDock_number	   = 2;
			robot->desired_force	   = 0;
			break;
		}

		Serial.print(F("A\t")); Serial.println(robot->pup_actionDock_number);
		Serial.print(F("M\t")); Serial.println(robot->pup_moveDock_number);
		Serial.print(F("P\t")); Serial.println(robot->pup_passiveDock_number);
		Serial.print(F("F\t")); Serial.println(robot->desired_force);
		Serial.print(F("S\t")); Serial.println(robot->sweep_direction);

		success = checkYN(SER_IN_TIMEOUT);		
		success = success
			&& ((robot->pup_moveDock_number != robot->pup_actionDock_number) && (robot->pup_moveDock_number != robot->pup_passiveDock_number) && (robot->pup_passiveDock_number != robot->pup_actionDock_number)
				|| (robot->pup_mode == 's')); // If puppet mode is 's' then nothing matters but the variable success
	}

	if(robot->pup_mode != 's')	translateDockNumbersToProper(robot);
	robot->cb_phase_action = PHASE_ACTION_BEGIN;
	robot->timer_debug = millis();
	robot->stateTimer0 = millis();
}

bool automatedStep(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray)
{
	// This function executes the automated step proceedure given the following information: [det dock, move dock, att dock]
	bool continue_step = true;
	int8_t oldAttachDock;

	switch (robot->astep_phase)
	{
	case AUTOSTEP_000_BEGIN:
		// A: Simple initialization
		robot->isReverseAttachment = false;
		astep_next_phase(robot, msgArray, AUTOSTEP_010_DET_DOCK_PRESS);
		break;

	case AUTOSTEP_010_DET_DOCK_PRESS:
		// B: Detaching dock will press down with some duration
		robot->movement_dock = robot->astep_dock_detach;
		if (checkTime(robot->stateTimer0, robot->time_this_measurement, PARAM_ASTEP_01_TIME_TO_REVERSE))
		{
			astep_next_phase(robot, msgArray, AUTOSTEP_020_SMALL_INITIAL_LIFT);
		}
		else
		{
			dockPuppet(robot, robot->movement_dock, MOTORDIR_IDLE, 0, (-1) * robot->astep_direction, MOTOR_POWER_FULL, msgArray);
		}
		break;

	case AUTOSTEP_020_SMALL_INITIAL_LIFT:
		// C: Flipping dock will move slightly in the step direction to add a small amount of lifting force
		robot->movement_dock = robot->astep_dock_move;
		if (checkTime(robot->stateTimer0, robot->time_this_measurement, PARAM_ASTEP_02_TIME_TO_PULL_UP))
		{
			astep_next_phase(robot, msgArray, AUTOSTEP_030_CALIB_FORCE_SENSORS_BEFORE_LIFT);
		}
		else
		{
			dockPuppet(robot, robot->movement_dock, MOTORDIR_IDLE, 0, (robot->astep_direction), MOTOR_POWER_FULL, msgArray);
		}
		break;

	case AUTOSTEP_030_CALIB_FORCE_SENSORS_BEFORE_LIFT:
		// D: Calibrate the force sensors before actually lifting upwards
		calibrateFSR(robot);
		astep_next_phase(robot, msgArray, AUTOSTEP_040_PULL_UPWARDS_BEFORE_DET);
		break;

	case AUTOSTEP_040_PULL_UPWARDS_BEFORE_DET:
		// E: Flip forwards about the flipping dock to pre-load the flip with some pulling force
		if (checkTime(robot->stateTimer0, robot->time_this_measurement, PARAM_ASTEP_04_TIME_PRELOAD))
		{	astep_next_phase(robot, msgArray, AUTOSTEP_050_DETACH_REAR_DOCK);	}
		else
		{	dockPuppet(robot, robot->astep_dock_move, MOTORDIR_IDLE, 0, (robot->astep_direction), MOTOR_POWER_FULL, msgArray);		}
		break;

	case AUTOSTEP_050_DETACH_REAR_DOCK:
		// F: Commmand the detaching dock to undertake the detaching behavior. Command the movement dock to maintain some pull force.
		robot->action_dock = robot->astep_dock_detach;
		robot->movement_dock = robot->astep_dock_move;
		robot->passive_dock = getAbsentByte(robot->action_dock, robot->movement_dock, DOCK_0, DOCK_1, DOCK_2);
		robot->step_direction = robot->astep_direction;
		robot->pull_direction = robot->step_direction;
		robot->sweep_direction = robot->astep_isForwardMotion * robot->step_direction;
		robot->desired_force = getValFor1000g(robot->movement_dock, robot->astep_direction, robot) * (PARAM_ASTEP_05_DETACH_PULL_FORCE / 1000.0);
		if (!action_detach(robot, imu, msgArray))
		{	astep_next_phase(robot, msgArray, AUTOSTEP_060_CONTINUE_MOVEMENT_AFTER_DETACH);	}
		else {}
		break;

	case AUTOSTEP_060_CONTINUE_MOVEMENT_AFTER_DETACH:
		// G: Open-loop Movement after detach to ensure that clear of the now-hot surface
		robot->movement_dock = robot->astep_dock_move;
		if (checkTime(robot->stateTimer0, robot->time_this_measurement, PARAM_ASTEP_06_TIME_TO_MOVE))
		{	astep_next_phase(robot, msgArray, AUTOSTEP_070_POST_DETACH_PREPARATION);	}
		else
		{	dockPuppet(robot, robot->movement_dock, MOTORDIR_IDLE, 0, (robot->astep_direction), 1.0, msgArray);		}
		break;

	case AUTOSTEP_070_POST_DETACH_PREPARATION:
		// H: Prepare all but the flipping dock for next contact. After this is complete, calibrate the FSRs
		robot->action_dock = robot->astep_dock_attach;
		robot->movement_dock = robot->astep_dock_move;
		robot->passive_dock = getAbsentByte(robot->action_dock, robot->movement_dock, DOCK_0, DOCK_1, DOCK_2);
		robot->step_direction = robot->astep_direction;
		robot->flip_direction = robot->astep_direction;
		dockPrepare(robot, robot->action_dock, robot->step_direction, msgArray);
		dockPrepare(robot, robot->passive_dock, robot->step_direction, msgArray);
		if (checkTime(robot->stateTimer0, robot->time_this_measurement, PARAM_ASTEP_07_MIN_PREP_TIME))
		{
			if (((robot->dockState_action != MSG_I2C_COMMAND_STOP) && (robot->dockPhase_action == PHASE_COMPLETE))
				&& ((robot->dockState_passive != MSG_I2C_COMMAND_STOP) && (robot->dockPhase_passive == PHASE_COMPLETE)))
			{
				calibrateFSR(robot);
				astep_next_phase(robot, msgArray, AUTOSTEP_075_REVERSE_PREP);
			}
		}
		break;

	case AUTOSTEP_075_REVERSE_PREP:
		// H.5: Slight reverse of docks to allow future press if reverse
		robot->action_dock = robot->astep_dock_attach;
		robot->movement_dock = robot->astep_dock_move;
		robot->passive_dock = getAbsentByte(robot->action_dock, robot->movement_dock, DOCK_0, DOCK_1, DOCK_2);
		robot->step_direction = robot->astep_direction;
		if (checkTime(robot->stateTimer0, robot->time_this_measurement, PARAM_ASTEP_075_PREP_REV_TIME))
		{
			astep_next_phase(robot, msgArray, AUTOSTEP_077_PREP_HOOP_RETURN);
		}
		else
		{
			dockPuppet(robot, robot->astep_dock_attach, MOTORDIR_IDLE, 0, (-1) * robot->astep_direction, 1, msgArray);
			dockPuppet(robot, robot->passive_dock,		MOTORDIR_IDLE, 0, (-1) * robot->astep_direction, 1, msgArray);
		}
		break;

	case AUTOSTEP_077_PREP_HOOP_RETURN:
		// H.7: Slight reverse of docks to allow future press if reverse
		robot->action_dock = robot->astep_dock_attach;
		robot->movement_dock = robot->astep_dock_move;
		robot->passive_dock = getAbsentByte(robot->action_dock, robot->movement_dock, DOCK_0, DOCK_1, DOCK_2);
		robot->step_direction = robot->astep_direction;
		if (checkTime(robot->stateTimer0, robot->time_this_measurement, 0.75))
		{	astep_next_phase(robot, msgArray, AUTOSTEP_080_FLIP_FORWARD_UNTIL_CONTACT);		}
		else
		{
			dockPuppet(robot, robot->astep_dock_attach, (-1) * robot->astep_direction, 1, MOTORDIR_IDLE, 0, msgArray);
			dockPuppet(robot, robot->passive_dock,		(-1) * robot->astep_direction, 1, MOTORDIR_IDLE, 0, msgArray);
		}
		break;

	case AUTOSTEP_080_FLIP_FORWARD_UNTIL_CONTACT:
		// I: Move the docks forward until contact is achieved
		robot->movement_dock = robot->astep_dock_move;
		robot->flip_direction = robot->astep_direction;
		Serial.print("**"); Serial.print(robot->fsr_cal_0f); Serial.print("**");
		robot->desired_force = getValFor1000g(robot->movement_dock, robot->astep_direction, robot) * (PARAM_ASTEP_08_FLIP_PRESS_FORCE/1000);
		if (!action_flip(robot, imu, msgArray))
		{	astep_next_phase(robot, msgArray, AUTOSTEP_090_BACK_OFF_AFTER_CONTACT);		}
		else {}
		break;

	case AUTOSTEP_090_BACK_OFF_AFTER_CONTACT:
		// J: After contact is achieved, back off open-loop until sure that no contact, then recalibrate
		robot->movement_dock = robot->astep_dock_move;
		if (checkTime(robot->stateTimer0, robot->time_this_measurement, PARAM_ASTEP_09_BACKOFF_TIME))
		{
			calibrateFSR(robot);
			astep_next_phase(robot, msgArray, AUTOSTEP_100_PRESS_INTO_SURFACE);
		}
		else
		{	dockPuppet(robot, robot->movement_dock, MOTORDIR_IDLE, 0, (-1) * (robot->astep_direction), 1.0, msgArray);		}
		break;

	case AUTOSTEP_100_PRESS_INTO_SURFACE:
		// K: Press into surface until desired force is reached
		robot->flip_direction = robot->astep_direction;
		robot->movement_dock = robot->astep_dock_move;
		robot->desired_force = getValFor1000g(robot->movement_dock, robot->astep_direction, robot) * (PARAM_ASTEP_10_ATT_PRESS_FORCE / 1000.0);
		if ((fabs(robot->meas_fsr_force) >= robot->desired_force) || (checkTime(robot->stateTimer0, robot->time_this_measurement, PARAM_10_MAX_TIME)))
		{	astep_next_phase(robot, msgArray, AUTOSTEP_110_SELECT_NEXT_ATTACH_DOCK);	}
		else
		{
			dockPuppet(robot, robot->movement_dock, MOTORDIR_IDLE, 0, (robot->astep_direction), 1.0, msgArray);
			// Slight press of hoop against the centerbody
			dockPuppet(robot, robot->astep_dock_detach, 
				(-1) * (robot->astep_direction), 0.5, MOTORDIR_IDLE, 0, msgArray);
			dockPuppet(robot, getAbsentByte(robot->movement_dock, robot->astep_dock_detach, DOCK_0, DOCK_1, DOCK_2), 
				(-1) * (robot->astep_direction), 0.5, MOTORDIR_IDLE, 0, msgArray);
		}
		break;

	case AUTOSTEP_110_SELECT_NEXT_ATTACH_DOCK:
		// L: Decide which dock to step on next
#ifdef SERIAL_GET_NEXT_ATTACH_AUTO_STEP
		shutdownAllDocks(robot, msgArray);
		// Get valid dock number for next attach
		oldAttachDock = robot->astep_dock_attach;
		do
		{
			Serial.print(F("Next Attach (7x =ReversedAttachment on dock x; 8=Go to reverse step; 9=Resettle):"));
			robot->astep_dock_attach = getSerialInt(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON);
		} while ((robot->astep_dock_attach == robot->astep_dock_move)
			&& ((robot->astep_dock_attach != 0) && (robot->astep_dock_attach != 1) && (robot->astep_dock_attach != 2))
			&& ((robot->astep_dock_attach != ASTEP_SPECIAL_RESETTLE) && (robot->astep_dock_attach != ASTEP_SPECIAL_EZ_REVERSE)
				&& (robot->astep_dock_attach != (ASTEP_SPECIAL_REVERSE_ATT + 0) && (robot->astep_dock_attach != (ASTEP_SPECIAL_REVERSE_ATT + 1) && (robot->astep_dock_attach != (ASTEP_SPECIAL_REVERSE_ATT + 2))))));

		robot->attach_melt_integrated_current = PARAM_ASTEP_110_INTEGRATED_CURRENT;	//do	{	Serial.print(F("Integrated Current:"));	robot->attach_melt_integrated_current = getSerialFloat(SER_IN_TIMEOUT, SERIAL_INPUT_ECHO_ON);		} while ((robot->attach_melt_integrated_current < 10) || (robot->attach_melt_integrated_current > 70));
#else
		Error!Behavior not yet implemented
#endif // SERIAL_GET_NEXT_ATTACH_AUTO_STEP
		if ((robot->astep_dock_attach == (ASTEP_SPECIAL_REVERSE_ATT+0))
			|| (robot->astep_dock_attach == (ASTEP_SPECIAL_REVERSE_ATT + 1))
				|| (robot->astep_dock_attach == (ASTEP_SPECIAL_REVERSE_ATT + 2)))
		{
			robot->isReverseAttachment = true;
			// **************************************************
			// This is just hardcoded and not fancy because the code needs a cleanup anyways and it would be more complex to do this properly given the current structure of the code
			// TODO: Make this not be hardcoded
			// **************************************************
			// Correct the dock assignment
			robot->astep_dock_attach = robot->astep_dock_attach - ASTEP_SPECIAL_REVERSE_ATT;
			robot->astep_dock_attach = robot->astep_dock_attach + DOCK_0;
			inputs(robot, imu, msgArray);
			robot->stateTimer0 = robot->time_this_measurement;
			bool behavior_continue = true;
			bool loop_continue = true;
			clearSerialBuffer();
			// Pull away
			while (!checkTime(robot->stateTimer0, robot->time_this_measurement, 7.5) && behavior_continue && loop_continue)
			{
				if (Serial.available())
				{
					if (Serial.read() == 'x')
					{	behavior_continue = false;	}
					if(Serial.read() == 'z')
					{	loop_continue = false;		}
					clearSerialBuffer();
				}
				inputs(robot, imu, msgArray);
				dockPuppet(robot, robot->astep_dock_move, MOTORDIR_IDLE, 0, ((-1)*robot->astep_direction), 1.0, msgArray);
				Serial.print(F("A"));
			}
			shutdownAllDocks(robot, msgArray);
			loop_continue = true;

			// Reverse sweep
			robot->stateTimer0 = robot->time_this_measurement;
			robot->passive_dock = getAbsentByte(robot->action_dock, robot->movement_dock, DOCK_0, DOCK_1, DOCK_2);
			while (!checkTime(robot->stateTimer0, robot->time_this_measurement, 3.5) && behavior_continue)
			{
				if (Serial.available())
				{
					if (Serial.read() == 'x')
					{	behavior_continue = false;	}
					if (Serial.read() == 'z')
					{	loop_continue = false;		}
					clearSerialBuffer();
				}
				inputs(robot, imu, msgArray);
				dockPuppet(robot, robot->astep_dock_attach, robot->astep_direction, 1.0, MOTORDIR_IDLE, 0.0, msgArray);
				dockPuppet(robot, robot->passive_dock, robot->astep_direction, 1.0, MOTORDIR_IDLE, 0.0, msgArray);
				Serial.print(F("B"));
			}
			shutdownAllDocks(robot, msgArray);
			loop_continue = true;
			delay(2000);
			
			// Press back
			calibrateFSR(robot);
			robot->stateTimer0 = robot->time_this_measurement;
			while (behavior_continue)
			{
				inputs(robot, imu, msgArray);
				if ((fabs(robot->meas_fsr_force) >= robot->desired_force) 
					|| (checkTime(robot->stateTimer0, robot->time_this_measurement, PARAM_10_MAX_TIME)))
				{
					behavior_continue = false;
					shutdownAllDocks(robot, msgArray);
				}
				else
				{
					if (Serial.available())
					{
						if (Serial.read() == 'x')
						{
							behavior_continue = false;
						}
						clearSerialBuffer();
					}
					dockPuppet(robot, robot->movement_dock, MOTORDIR_IDLE, 0, (robot->astep_direction), 1.0, msgArray);
					// Slight press of hoop against the centerbody
					dockPuppet(robot, robot->astep_dock_detach,
						(robot->astep_direction), 0.5, MOTORDIR_IDLE, 0, msgArray);
					dockPuppet(robot, getAbsentByte(robot->movement_dock, robot->astep_dock_detach, DOCK_0, DOCK_1, DOCK_2),
						(robot->astep_direction), 0.5, MOTORDIR_IDLE, 0, msgArray);
					Serial.print(F("C")); Serial.print(robot->meas_fsr_force, 1);
				}
			}
			
			// Continue
			astep_next_phase(robot, msgArray, AUTOSTEP_120_ATTACH_ON_SELECTED_DOCK);
		}
		else
		{
			switch (robot->astep_dock_attach)
			{
			case ASTEP_SPECIAL_RESETTLE:
				robot->astep_dock_attach = robot->astep_dock_attach + DOCK_0;
				astep_next_phase(robot, msgArray, AUTOSTEP_090_BACK_OFF_AFTER_CONTACT);
				robot->astep_dock_attach = oldAttachDock;
				robot->astep_dock_passive = getAbsentByte(robot->astep_dock_attach, robot->astep_dock_move, DOCK_0, DOCK_1, DOCK_2);
				break;
			case ASTEP_SPECIAL_EZ_REVERSE:
				robot->astep_dock_attach = robot->astep_dock_attach + DOCK_0;
				astep_next_phase(robot, msgArray, AUTOSTEP_130_FINAL_COOL);
				continue_step = false;
				robot->astep_isForwardMotion = EZ_NEXT_STEP_IS_REVERSE;
				robot->astep_dock_attach = oldAttachDock;
				robot->astep_dock_passive = getAbsentByte(robot->astep_dock_attach, robot->astep_dock_move, DOCK_0, DOCK_1, DOCK_2);
				break;
			default:
				robot->astep_dock_attach = robot->astep_dock_attach + DOCK_0;
				astep_next_phase(robot, msgArray, AUTOSTEP_120_ATTACH_ON_SELECTED_DOCK);
				break;
			}
		}
		break;

		case AUTOSTEP_120_ATTACH_ON_SELECTED_DOCK			:
			// M: Execute attach behavior
			robot->action_dock		= robot->astep_dock_attach;
			robot->movement_dock	= robot->astep_dock_move;
			robot->passive_dock		= getAbsentByte(robot->action_dock, robot->movement_dock, DOCK_0, DOCK_1, DOCK_2);
			robot->flip_direction	= robot->astep_direction;
			robot->pressDirection	= robot->astep_direction;
			if (robot->isReverseAttachment)
			{
				robot->sweep_direction = (-1) * robot->astep_direction;
			}
			else
			{
				robot->sweep_direction = robot->astep_direction;
			}
			//robot->attach_melt_integrated_current = PARAM_ASTEP_12_ATT_INT_CURR;	// Now a definable parameter in the previous step
			robot->desired_force	= getValFor1000g(robot->movement_dock, robot->astep_direction, robot) * (PARAM_ASTEP_10_ATT_PRESS_FORCE / 1000.0);
			if (!action_attach(robot, imu, msgArray))
			{	astep_next_phase(robot, msgArray, AUTOSTEP_130_FINAL_COOL);	}
			else{}
			break;

		case AUTOSTEP_130_FINAL_COOL						:
			// N: Simple cleanup operations
			shutdownAllDocks(robot, msgArray);
			if (checkTime(robot->stateTimer0, robot->time_this_measurement, PARAM_ASTEP_13_FINAL_COOL_S))
			{
				Serial.println(F("SCom"));
				continue_step = false;
			}
			break;

		default												:
			astep_next_phase(robot, msgArray, AUTOSTEP_130_FINAL_COOL);
			Serial.println(F("BSP"));
			continue_step = false;
			break;
	}
	return continue_step;
}

void astep_next_phase(robot_centerbody* robot, uint8_t* msgArray, uint8_t nextPhase)
{
	shutdownAllDocks(robot, msgArray);
	robot->stateTimer0		= robot->time_this_measurement;
	robot->cb_phase_action	= PHASE_ACTION_BEGIN;
	robot->astep_phase		= nextPhase;
}

bool puppetLogic(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray)
{
	bool continue_puppet = true;
	Serial.write(robot->pup_mode);
	
	switch (robot->pup_mode)
	{
	///////////////
	case 's':
		Serial.write((robot->astep_phase/10) + 0x41);
		continue_puppet = automatedStep(robot, imu, msgArray);
		break;
	///////////////
	case 'd':	// Detach Behavior
		continue_puppet = action_detach(robot, imu, msgArray);
		break;
	case 'a':	// Attach behavior
		continue_puppet = action_attach(robot, imu, msgArray);
		break;
	case 'f':	// Flip Behavior
		continue_puppet = action_flip(robot, imu, msgArray);
		break;
	case 'h':	// Hoop movement behavior
		continue_puppet = action_hoopMove(robot, imu, msgArray);
		break;
	case 'm':	// Arm movement behavior
		continue_puppet = action_armMove(robot, imu, msgArray);
		break;
	case 'p':	// Prepare for next flip behavior
		continue_puppet = action_prepare(robot, imu, msgArray);
		break;
	case 'i':	// Information reporting behavior
		continue_puppet = action_information(robot, imu, msgArray);
		break;
	default:
		Serial.println(F("BAD"));
		continue_puppet = false;
	}
	if ((robot->ez_mode_active == true) && (continue_puppet == false))
	{
		// Shutdown, then print messages prior to initiating new step
		shutdownAllDocks(robot, msgArray);
		Serial.println(); Serial.println();
		Serial.print(F("STEP COMPLETED. INITIATING NEW STEP."));
		Serial.println();
		// EZ logic
		continue_puppet = true;
		
		// Update autostep behaviors
		robot->astep_direction = (-1) * robot->astep_direction; // Steps will always be opposite
		
		if (robot->astep_isForwardMotion == EZ_NEXT_STEP_IS_REVERSE)
		{
			robot->astep_isForwardMotion = -1;
			robot->astep_phase = AUTOSTEP_060_CONTINUE_MOVEMENT_AFTER_DETACH;
			robot->ez_attached_dock = robot->astep_dock_move;
			robot->ez_dock_to_detach = robot->astep_dock_attach;
			// No change to movement, attaching, detaching docks
		}
		else
		{
			robot->astep_isForwardMotion = 1;
			robot->astep_phase = AUTOSTEP_000_BEGIN;
			robot->astep_dock_detach = robot->ez_dock_to_detach;
			robot->astep_dock_attach = robot->ez_dock_to_detach; // Simply here to satisfy astep, will actually be handled during the step
			robot->astep_dock_move = robot->ez_attached_dock;
			robot->astep_dock_passive = getAbsentByte(robot->astep_dock_detach, robot->astep_dock_move, DOCK_0, DOCK_1, DOCK_2);
			robot->ez_attached_dock = robot->astep_dock_attach;
			robot->ez_dock_to_detach = robot->astep_dock_move;
		}
		
		// Print step information
		Serial.print(F("["));
		Serial.print(robot->astep_direction); Serial.print(F(","));
		Serial.print(robot->astep_dock_attach - DOCK_0_I2C_ADDR); Serial.print(F(","));
		Serial.print(robot->astep_dock_detach - DOCK_0_I2C_ADDR); Serial.print(F(","));
		Serial.print(robot->astep_dock_move - DOCK_0_I2C_ADDR); Serial.print(F(","));
		Serial.print(robot->astep_dock_passive - DOCK_0_I2C_ADDR); Serial.print(F(","));
		Serial.print(robot->astep_phase); Serial.print(F("]"));
		Serial.println(); Serial.println();
	}
	// Return false if everything is complete, true if need to continue
	return continue_puppet;	
}

// Mid-level functions for all the individual actions that might be called upon /////////////////
bool action_detach(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray)
{
	bool action_continue = true;
	// Detaching dock must execute detach proceedure	// Movement dock must pull upward with some force
	// Passive dock should do nothing; dock prepare will take place during flip
	switch (robot->cb_phase_action)
	{
	case PHASE_ACTION_BEGIN:
		robot->cb_phase_action = PHASE_ACTION_DET_CONTINUE;
		dockDetach(robot, robot->action_dock, msgArray);
		break;
	case PHASE_ACTION_DET_CONTINUE:
		dockDetach(robot, robot->action_dock, msgArray);
		if (((robot->dockPhase_action == PHASE_DT_DT2) && (fabs(robot->meas_fsr_force) < robot->desired_force)
			|| (robot->dockPhase_action == PHASE_DT_BUF) || (getDockInformation(robot, robot->action_dock, I2C_SEND_INT_I, msgArray) > PARAM_DET_INT_CURRENT_THRESHOLD_TO_PULL))) 
			{	dockPuppet(	robot, robot->movement_dock, (-1) * robot->pull_direction, PARAM_DET_HOOP_PULL_HELP_AMT, robot->pull_direction, 0.5, msgArray);	}
		else{	dockPuppet(	robot, robot->movement_dock, (-1) * robot->pull_direction, PARAM_DET_HOOP_PULL_HELP_AMT, ARMDIR_IDLE,			0.0, msgArray); }
		// Check for being complete
		if ((robot->dockPhase_action >= PHASE_DT_BUF)&&(robot->dockPhase_action != PHASE_DT_HF))
		{
			robot->cb_phase_action = PHASE_ACTION_DET_BUFFER;
			robot->stateTimer0 = robot->time_this_measurement;
		}
		break;
	case PHASE_ACTION_DET_BUFFER:
		// Detach dock is still expecting dock to be pulled
		dockDetach(robot, robot->action_dock, msgArray);
		dockPuppet(robot, robot->movement_dock, HOOPDIR_IDLE, 0, robot->pull_direction, 1.0, msgArray);
		if (robot->dockPhase_action == PHASE_COMPLETE)
		{
			action_continue = false;
			robot->cb_phase_action = PHASE_ACTION_COMPLETE;
		}
		break;
	case PHASE_ACTION_COMPLETE:
		action_continue = false; shutdownAllDocks(robot, msgArray);
		break;
	default:
		Serial.print(F("z"));
		action_continue = false;
		shutdownAllDocks(robot, msgArray);
		robot->cb_phase_action = PHASE_ACTION_COMPLETE;
		break;
	}
	return action_continue;
}
bool action_attach(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray)
{
	bool action_continue = true;
	// Attaching dock must execute detach proceedure
	dockAttach(robot, robot->action_dock, msgArray);

	// Movement dock must push with some force
	if (fabs(robot->meas_fsr_force) < robot->desired_force)	{	dockFlip(robot, robot->movement_dock, 1.0, robot->pressDirection, msgArray);	}
	else													{	dockStop(robot, robot->movement_dock, msgArray);	}
	// Passive dock must do nothing
	dockStop(robot, robot->passive_dock, msgArray);

	switch (robot->cb_phase_action)
	{
	case PHASE_ACTION_BEGIN:
		robot->cb_phase_action = PHASE_ACTION_ATT_CONTINUE;
		robot->att_sweep_motor_power = 0.5;
		robot->stateTimer0 = robot->time_this_measurement;
		break;
	case PHASE_ACTION_ATT_CONTINUE:
		// Check for being complete
		if ((robot->dockPhase_action == PHASE_COMPLETE)||(robot->dockPhase_action==PHASE_ATT_HRN))
		{
			action_continue = false;
			shutdownAllDocks(robot, msgArray);
			robot->cb_phase_action = PHASE_ACTION_COMPLETE;
		}
		if (checkTime(robot->stateTimer0, robot->time_this_measurement, 5))
		{
			robot->att_sweep_motor_power = bound(robot->att_sweep_motor_power + 0.05,0,1);
		}
		break;
	case PHASE_ACTION_COMPLETE:
		action_continue = false; shutdownAllDocks(robot, msgArray);
		break;
	default:
		shutdownAllDocks(robot, msgArray);
		Serial.print(F("z"));
		robot->cb_phase_action = PHASE_ACTION_COMPLETE;
		action_continue = false;
		break;
	}
	return action_continue;
}
bool action_flip(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray)
{
	bool action_continue = true;
	robot->step_direction = robot->flip_direction;
	switch (robot->cb_phase_action)
	{
	case PHASE_ACTION_BEGIN:
		shutdownAllDocks(robot, msgArray);
		robot->stateTimer0 = robot->time_this_measurement;
		robot->cb_phase_action = PHASE_ACTION_FLP_FWD;
		break;
	case PHASE_ACTION_FLP_FWD:
		// Movement dock must flip until it hits something with some force
		if (fabs(robot->meas_fsr_force) < robot->desired_force)	{	dockFlip(robot, robot->movement_dock, 1.0, robot->flip_direction, msgArray);	}
		else													{	action_continue = false;	shutdownAllDocks(robot, msgArray);		}

		// Check if should back off (time increase critereon, don't revers if nearing the desired force (likely have made contact)
		if ((checkTime(robot->stateTimer0, robot->time_this_measurement, FLIP_FORWARD_TIME_WITHOUT_RECAL))
			&&(fabs(robot->meas_fsr_force) < (robot->desired_force* FLIP_FORWARD_FORCE_RATIO)))
		{
			robot->cb_phase_action = PHASE_ACTION_FLP_REV;
			shutdownAllDocks(robot, msgArray);
			robot->stateTimer0 = robot->time_this_measurement;
		}
		break;
	case PHASE_ACTION_FLP_REV:
		dockFlip(robot, robot->movement_dock, 1.0, (-1) * robot->flip_direction, msgArray);
		if (checkTime(robot->stateTimer0, robot->time_this_measurement, FLIP_REVERSE_TIME_BEFORE_RECAL))
		{
			shutdownAllDocks(robot, msgArray);
			robot->cb_phase_action = PHASE_ACTION_FLP_WAIT;
			robot->stateTimer0 = robot->time_this_measurement;
		}
		break;
	case PHASE_ACTION_FLP_WAIT:
		shutdownAllDocks(robot, msgArray);
		if (checkTime(robot->stateTimer0, robot->time_this_measurement, FLIP_RECAL_WAIT_DURATION))
		{	robot->cb_phase_action = PHASE_ACTION_FLP_RCAL;	}
		break;
	case PHASE_ACTION_FLP_RCAL:
		calibrateFSR(robot);
		robot->cb_phase_action = PHASE_ACTION_FLP_FWD;
		break;
	default:
		Serial.print(F("BFL"));
		shutdownAllDocks(robot, msgArray);
		robot->cb_phase_action = PHASE_ACTION_BEGIN;
		break;
	}
	return action_continue;
}
bool action_hoopMove(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray)
{
	bool action_continue = true;
	if (!checkTime(robot->timer_debug, robot->time_this_measurement, robot->dock_movement_timeout))
	{
		if (robot->desired_force == 1)		{	dockPuppet(robot, robot->action_dock, MOTORDIR_FWD, 1.0, MOTORDIR_IDLE, 0, msgArray);	}
		else								{	dockPuppet(robot, robot->action_dock, MOTORDIR_REV, 1.0, MOTORDIR_IDLE, 0, msgArray);	}
		// All other docks must do nothing
	}
	else
	{
		action_continue = false;
		dockStop(robot, robot->action_dock, msgArray);
		shutdownAllDocks(robot, msgArray);
	}
	return action_continue;
}
bool action_armMove(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray)
{
	bool action_continue = true;
	if (!checkTime(robot->timer_debug, robot->time_this_measurement, robot->dock_movement_timeout))
	{
		if (robot->desired_force == 1)		{	dockPuppet(robot, robot->action_dock, MOTORDIR_IDLE, 0, MOTORDIR_FWD, 1.0, msgArray);	}
		else								{	dockPuppet(robot, robot->action_dock, MOTORDIR_IDLE, 0, MOTORDIR_REV, 1.0, msgArray);	}
		// All other docks must do nothing
	}
	else
	{
		action_continue = false;
		dockStop(robot, robot->action_dock, msgArray);
		shutdownAllDocks(robot, msgArray);
	}
	return action_continue;
}
bool action_prepare(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray)
{
	bool action_continue = true;
	robot->step_direction = robot->flip_direction;
	dockPrepare(robot, robot->action_dock, robot->step_direction, msgArray);

	switch (robot->cb_phase_action)
	{
	case PHASE_ACTION_BEGIN:
		robot->cb_phase_action = PHASE_ACTION_PREP_CONTINUE;
		break;
	case PHASE_ACTION_PREP_CONTINUE:
		if ((robot->dockPhase_action == PHASE_COMPLETE))
		{
			robot->cb_phase_action = PHASE_ACTION_COMPLETE;
			shutdownAllDocks(robot, msgArray);
			action_continue = false;
		}
		break;
	case PHASE_ACTION_COMPLETE:
		shutdownAllDocks(robot, msgArray);
		action_continue = false;
		break;
	default:
		shutdownAllDocks(robot, msgArray);
		Serial.print(F("z"));
		robot->cb_phase_action = PHASE_ACTION_COMPLETE;
		action_continue = false;
		break;
	}

	if ((robot->dockPhase_action == PHASE_COMPLETE))
	{
		action_continue = false;
		shutdownAllDocks(robot, msgArray);
	}
	return action_continue;
}
bool action_information(robot_centerbody* robot, IMUpn* imu, uint8_t* msgArray)
{
	// Do nothing; All reports and exit conditions handled in the main loop	
	return true;
}

// Lower-level Functions //////////////////////////////////////////////////////////////////////
float getValFor1000g(uint8_t movement_dock, int8_t step_direction, robot_centerbody* robot)
{
#define NUMDOCKS 3
	uint8_t index;
	if (step_direction == MOTORDIR_FWD)	{ index = movement_dock				;}
	else								{ index = movement_dock + NUMDOCKS	;}
	switch (index)
	{
	case DOCK_0:
		return robot->fsr_cal_0f;	// Dock 0 Forward
	case DOCK_0 + NUMDOCKS:
		return robot->fsr_cal_0r;	// Dock 0 Reverse
	case DOCK_1:
		return robot->fsr_cal_1f;	// Dock 1 Forward 
	case DOCK_1 + NUMDOCKS:				
		return robot->fsr_cal_1r;	// Dock 1 Reverse
	case DOCK_2:
		return robot->fsr_cal_2f;	// Dock 2 Forward
	case DOCK_2 + NUMDOCKS:				
		return robot->fsr_cal_2r;	// Dock 2 Reverse
	default:
		return 0;
	}
	// Something has gone horribly wrong
	return 0;
}

void debug_sensorReport(robot_centerbody* robot)
{
	Serial.print(F("\t-- "));
	Serial.print(robot->meas_fsr_force);
	Serial.print(robot->meas_grav_x, 3); Serial.print(F(" "));
	Serial.print(robot->meas_grav_y, 3); Serial.print(F(" "));
	Serial.print(robot->meas_grav_z, 3); Serial.print(F(" "));
	Serial.print(robot->calc_flip_speed);
	Serial.println();
}

void debug_flashDockLED(uint8_t dockID, uint8_t* msgArray, uint8_t numFlashes, uint32_t duration_ms)
{
	for (uint8_t ii = 0; ii < numFlashes; ii++)
	{
		// Send message to turn on LED
		debug_setLED(dockID, msgArray, 1);
		delay(duration_ms);
		// Send Message to turn off LED
		debug_setLED(dockID, msgArray, 0);
		debug_setLED(dockID, msgArray, 0);
		delay(duration_ms);
	}
}

void debug_identifyDocks(uint8_t* msgArray)
{
	debug_flashDockLED(DOCK_0, msgArray, 1, 150);
	debug_flashDockLED(DOCK_1, msgArray, 2, 150);
	debug_flashDockLED(DOCK_2, msgArray, 3, 150);
}
void translateDockNumbersToProper(robot_centerbody* robot)
{
	robot->action_dock		= dock_num_to_I2C_addr(robot->pup_actionDock_number);
	robot->movement_dock	= dock_num_to_I2C_addr(robot->pup_moveDock_number);
	robot->passive_dock		= dock_num_to_I2C_addr(robot->pup_passiveDock_number);
}
uint8_t dock_num_to_I2C_addr(uint8_t num)
{
	switch (num)
	{
	case 0:		return DOCK_0;
	case 1:		return DOCK_1;
	case 2:		return DOCK_2;
	default:	return 0;
	}
}
void debug_setLED(uint8_t dockID, uint8_t* msgArray, uint8_t level)
{
	// Queue up the message array
	clearMsgArray(msgArray);
	msgArray[MSG_I2C_COMMAND_BYTE] = MSG_I2C_COMMAND_LED;
	msgArray[ARG_LED_LEVEL] = level;
	// Write the message to the desired dock
	sendArrayI2C(dockID, msgArray, COM_I2C_MAX_MSG_LENGTH, I2C_CHECK_BYTE_NUM);
}