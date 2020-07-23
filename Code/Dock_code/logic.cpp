/*///////////////////////////////////////////////////
Name:		logic.cpp
Created :	3 / 25 / 2019
Author :	Petras Swissler

Purpose :	Handle all logic functions. 
			All functions modify robot object
*/////////////////////////////////////////////////////
#include "logic.h"

//////////////////////////////////////////////////////
// Main Function
void runLogic(dock_robot* robot)
{
	// TODO: ADD TIMEOUT BEHAVIOR
	// Integrate all necessary values
	integrateValues(robot);

	// Switch based on commanded behavior
	switch (robot->command_behavior)
	{
	case MSG_I2C_COMMAND_ATTACH:
		behavior_attach(robot);
		break;
	case MSG_I2C_COMMAND_DETACH:
		behavior_detach(robot);
		break;
	case MSG_I2C_COMMAND_FLIP:
		behavior_flip(robot);
		break;
	case MSG_I2C_COMMAND_PREPARE:
		behavior_prepare(robot);
		break;
	case MSG_I2C_COMMAND_STOP:
		behavior_stop(robot);
		break;
	case MSG_I2C_COMMAND_PUPPET:
		behavior_puppet(robot);
		break;
	case CALIBRATION_STATE:
		// Do Nothing
		break;
	}
}

//////////////////////////////////////////////////////
// All logic functions for all commanded states
void behavior_attach(dock_robot* robot)
{
	switch (robot->state_phase)
	{
	case PHASE_INIT:
		robot->calc_integrated_current = 0;
		robot->state_phase = PHASE_ATT_SWP;
		// Initialize any values
		robot->scale_high	= CONFIG_SWEEP_INIT_PULSE_HIGH;
		robot->scale_low	= CONFIG_SWEEP_INIT_PULSE_LOW;
		shutdown_all(robot);
		robot->trackTimer0 = robot->time_this_measurement;
		break;

	case PHASE_ATT_SWP:
		if ((robot->meas_current_hoop >= robot->config_SWP_I_target)||(robot->meas_current_dock > THRESH_INT_I_MIN/2))
		{
			robot->state_phase = PHASE_ATT_MLT;
			// Go to next
			robot->out_max_melt_voltage = robot->config_ATT_V_start;
			robot->scale_high = 0.9;
			robot->scale_low = 0.15;
			robot->trackTimer0 = robot->time_this_measurement;
			robot->out_melt_dock_voltage = robot->config_SWP_voltage;
		}
		else
		{
			// If track timer 0 has expired, increase sweep force (bound to 0-1)
			if (checkTime(robot->trackTimer0, robot->time_this_measurement, robot->config_SWP_hoop_motor_pwr_growth_period))
			{
				robot->scale_high	= robot->scale_high*=1.1;
				robot->scale_low	= robot->scale_low*=1.1;
				robot->trackTimer0	= robot->time_this_measurement;
			}
			// Move towards other dock with pulse
			robot->out_hoop_power		= pulseLogic(robot->config_SWP_hoop_motor_power, robot->scale_low, robot->scale_high, robot->time_this_measurement, PERIOD_PULSE);
			robot->out_hoop_direction	= robot->config_ATT_sweep_direction;

			// Configure Melting
			robot->out_desired_current	 = 10 * robot->config_SWP_I_target;
			robot->out_max_melt_voltage  = robot->config_SWP_voltage;
			robot->out_melt_dock_voltage = robot->config_SWP_voltage;
			robot->out_dock_state = HBMELT_HI_V;
		}
		robot->out_arm_power = 0;
		break;

	case PHASE_ATT_MLT:
		if (robot->calc_integrated_current >= robot->config_ATT_integrated_I_target)
		{
			robot->out_dock_state = HBMELT_HI_Z;
			robot->out_melt_dock_voltage = 0;
			robot->out_max_melt_voltage = 0;
			// Go to next
			robot->state_phase = PHASE_ATT_COL;
			robot->trackTimer0 = robot->time_this_measurement;
			robot->scale_high = 1.0;
			robot->scale_low = 0.1;
		}
		else
		{
			// If below threshold current, apply some light pressure on the hoop
			robot->out_hoop_direction = robot->config_ATT_sweep_direction;
			if(robot->meas_current_dock < 0.25)
			{
				// Special behavior to pulse slightly more in case there are issues with the hoop actually touching
				robot->out_hoop_power = pulseLogic(robot->config_SWP_hoop_motor_power*0.5, 1.0, 0.5, robot->time_this_measurement, PERIOD_SHORT_PULSE);
			}
			else
			{
				robot->out_hoop_power = pulseLogic(robot->config_SWP_hoop_motor_power*0.05, 1.0, 0.5, robot->time_this_measurement, PERIOD_SHORT_PULSE);
			}
			// Check if need to increase dock voltage
			if (checkTime(robot->trackTimer0, robot->time_this_measurement, 1.0))//robot->config_ATT_V_growth_period))
			{
				robot->out_max_melt_voltage = robot->out_max_melt_voltage + 1;
				robot->out_max_melt_voltage = bound(robot->out_max_melt_voltage, robot->config_ATT_V_start, robot->config_ATT_V_max);
				robot->trackTimer0			= millis();
			}
			robot->out_dock_state			= HBMELT_HI_V;
			robot->out_melt_dock_voltage	= robot->out_max_melt_voltage;
			// Melt the dock
			// No need to update max melt voltage
			robot->out_desired_current		= robot->config_ATT_I_max;
		}
		robot->out_arm_power = 0;
		break;

	case PHASE_ATT_COL:
		if (checkTime(robot->trackTimer0, robot->time_this_measurement, CONFIG_TIME_TO_ALLOW_COOL_BEFORE_HOOP_RETRACT))
		{
			robot->state_phase = PHASE_ATT_HRN;
			// Move to next phase
			robot->trackTimer0 = robot->time_this_measurement;
		}
		// Do nothing until can retract the hoop
		robot->out_dock_state = HBMELT_HI_Z;
		robot->out_arm_power		= 0;
		robot->out_hoop_power		= 0;
		robot->out_desired_current	= 0;
		robot->out_max_melt_voltage = 0;
		break;

	case PHASE_ATT_HRN:
		if ((robot->sen_hoop_level == 0) || (checkTime(robot->trackTimer0, robot->time_this_measurement, 2)))
		{
			robot->state_phase = PHASE_COMPLETE;
			shutdown_all(robot);
		}
		else
		{
			robot->out_hoop_direction = (-1) * robot->config_ATT_sweep_direction;
			robot->out_hoop_power = robot->config_SWP_hoop_motor_power;
		}
		robot->out_dock_state = HBMELT_HI_Z;
		robot->out_arm_power = 0;
		robot->out_desired_current = 0;
		robot->out_max_melt_voltage = 0;
		

		// DEBUG ***
		// todo: reimplement
		/*// Continue until the hoop hits its limit
		if (robot->sen_hoop_level)
		{
			// Move hoop backwards at some speed
			robot->out_hoop_direction = (-1)*robot->config_ATT_sweep_direction;
			robot->out_hoop_power = robot->config_SWP_hoop_motor_power;
		}
		else
		{	robot->state_phase = PHASE_COMPLETE;	}*/
		
		// If Timeout, tuck the hoop back where it was
		/*if (checkTime(robot->trackTimer0, robot->time_this_measurement, CONFIG_TIME_TO_TRY_TO_PREPARE_HOOP))
		{
			if (checkTime(robot->trackTimer1, robot->time_this_measurement, CONFIG_TIME_TO_PREPARE_HOOP_IF_PREPARE_FAILED))
			{	robot->state_phase = PHASE_COMPLETE;	}
			robot->out_hoop_direction = (-1)*robot->config_ATT_sweep_direction;
			robot->out_hoop_power = robot->config_SWP_hoop_motor_power;
		}
		else
		{	robot->trackTimer1 = robot->time_this_measurement;		}
		robot->out_arm_power = 0;
		break;*/

	case PHASE_COMPLETE:
		// Do nothing 
		shutdown_all(robot);
		break;

	default:
		// Something has gone horribly wrong
		robot->state_phase = PHASE_COMPLETE;
		break;
	}
}

void behavior_detach(dock_robot* robot)
{
	// Variable declaration
	bool maxTimeMet;
	bool currentBreakDetect;
	bool intCurrMet;
	bool minTimeMet;
	// Init / DT1 / DT2 / BUFFER / COMPLETE
	switch (robot->state_phase)
	{
	case PHASE_INIT:
		// Initialize any values
		robot->out_hoop_direction = robot->config_DET_sweep_direction;
		robot->out_hoop_power = 1.0;
		robot->out_arm_power = 0;
		robot->out_dock_state = HBMELT_HI_Z;
		robot->out_max_melt_voltage = 0;
		robot->trackTimer0 = robot->time_this_measurement;
		robot->calc_max_current_detected = 0;
		robot->calc_integrated_current = 0;
		robot->count_numCurrentMeasBelowThreshold = 0;
		robot->state_phase = PHASE_DT_HF;
		break;
	case PHASE_DT_HF:
		// Let the hoop advance without melting for some short duration
		robot->out_hoop_direction = robot->config_DET_sweep_direction;
		robot->out_hoop_power = 1.0;
		if (checkTime(robot->trackTimer0, robot->time_this_measurement, 0.75))
		{
			robot->state_phase = PHASE_DT_DT1;
			robot->trackTimer0 = robot->time_this_measurement;
		}		
		break;
	case PHASE_DT_DT1:
		robot->out_hoop_direction = robot->config_DET_sweep_direction;
		robot->out_hoop_power = 0.75;
		robot->out_melt_dock_current_limit = 10;// robot->config_DT1_max_current;
		robot->out_dock_state = HBMELT_HI_V;
		robot->out_melt_dock_voltage = 50;// todo: re-add: robot->config_DT1_max_voltage;

		robot->calc_max_current_detected = 	
			max(robot->calc_max_current_detected*CONFIG_DEGRADATION_OF_MAX_CURRENT, robot->meas_current_dock);
		
		// Continue if: MaxTimeMet||CurBreakDetect||(intCurrMet&&minTimeMet)
		maxTimeMet = checkTime(robot->trackTimer0, robot->time_this_measurement, CONFIG_DETACH_DT1_MAX_T);
		intCurrMet = (robot->calc_integrated_current > robot->config_DET_DT1_integrated_I_target);
		minTimeMet = checkTime(robot->trackTimer0, robot->time_this_measurement, CONFIG_DETACH_DT1_MIN_T);

		if ((CONFIG_DETACH_CUR_BREAK_RATIO * robot->calc_max_current_detected) > robot->meas_current_dock)
		{	robot->count_numCurrentMeasBelowThreshold += 1;	}
		else
		{	robot->count_numCurrentMeasBelowThreshold = 0;	}
		
		if (	maxTimeMet 
			|| 	(robot->count_numCurrentMeasBelowThreshold > CONGIG_DETACH_NUM_CURRENT_BELOW_THRESHOLD) 
			||	(intCurrMet&&minTimeMet)		)
		{
			robot->count_numCurrentMeasBelowThreshold = 0;
			robot->state_phase = PHASE_DT_DT2;
		}
		break;
	case PHASE_DT_DT2:
		robot->out_hoop_direction	= robot->config_DET_sweep_direction;
		robot->out_hoop_power		= pulseLogic(robot->config_DET_hoop_motor_power, 0.65, 85, robot->time_this_measurement, PERIOD_SHORT_PULSE);
		robot->out_arm_direction	= twitchLogic(robot->time_this_measurement, PERIOD_PULSE);
		robot->out_arm_power		= robot->config_DET_arm_twitch_power;
		robot->out_melt_dock_current_limit = robot->config_DT1_max_current;
		robot->out_dock_state		= HBMELT_HI_V;
		robot->out_melt_dock_voltage = 50;// robot->config_DT1_max_voltage;

		//robot->calc_max_current_detected = max(robot->calc_max_current_detected * CONFIG_DEGRADATION_OF_MAX_CURRENT, robot->meas_current_dock);
		//currentBreakDetect =((CONFIG_DETACH_CUR_BREAK_RATIO*robot->calc_max_current_detected)< robot->meas_current_dock);
		if ((robot->meas_current_dock) <= CONFIG_DETACH_MIN_CURRENT)
		{	robot->count_numCurrentMeasBelowThreshold += 1;	}
		else
		{	robot->count_numCurrentMeasBelowThreshold = 0;	}
		
		if ((robot->count_numCurrentMeasBelowThreshold >= CONGIG_DETACH_NUM_CURRENT_BELOW_THRESHOLD))
		{
			robot->state_phase = PHASE_DT_BUF;
			robot->trackTimer0 = robot->time_this_measurement;
		}

		break;
	case PHASE_DT_BUF:
		robot->out_arm_direction = MOTORDIR_IDLE;
		robot->out_arm_power = 0;
		robot->out_hoop_direction = robot->config_PUP_hoop_direction;
		robot->out_hoop_power = 1.0;// robot->config_DET_hoop_motor_power;
		robot->out_melt_dock_voltage = 50;// robot->config_DT1_max_voltage;
		robot->out_dock_state = HBMELT_HI_V;

		if ((robot->meas_current_dock) >= CONFIG_DETACH_MIN_CURRENT)
		{
			// Exited DT2 too soon
			robot->trackTimer0 = robot->time_this_measurement;
			robot->count_numCurrentMeasBelowThreshold = 0;
			robot->state_phase = PHASE_DT_DT2;
		}

		// Continue melting until timeout occurs. Cannot detect contact due to how hoop contact detection works
		if (checkTime(robot->trackTimer0, robot->time_this_measurement, CONFIG_DETACH_BUFFER_TIME))
		{	robot->state_phase = PHASE_DT_RHP;	}
		break;
	case PHASE_DT_RHP:
		robot->out_arm_direction = MOTORDIR_IDLE;
		robot->out_arm_power = 0;

		robot->out_melt_dock_voltage = 0;
		robot->out_dock_state = HBMELT_HI_Z;

		robot->out_hoop_direction = (-1)*robot->config_DET_sweep_direction;
		robot->out_hoop_power = robot->config_DET_hoop_motor_power;

		executeOutputs(robot);
		delay(3000);
		robot->state_phase = PHASE_COMPLETE;
		shutdown_all(robot);
		break;
	case PHASE_COMPLETE:
		shutdown_all(robot);
		robot->out_dock_state = HBMELT_HI_Z;
		break;
	default:
		// Something has gone horribly wrong
		robot->state_phase = PHASE_COMPLETE;
		break;
	}
}

void behavior_flip(dock_robot*	 robot)
{
	switch (robot->state_phase)
	{
	case PHASE_INIT:
		robot->state_phase = PHASE_FLP_MAIN;
	case PHASE_FLP_MAIN:
		// check for timeout
		if (checkTime(robot->time_timeout_start, robot->time_this_measurement, robot->config_FLP_timeout))
		{
			shutdown_all(robot);
			robot->state_phase = PHASE_COMPLETE;
		}
		// Move according to command
		robot->out_hoop_direction = MOTORDIR_IDLE;
		robot->out_hoop_power = 0;
		/*switch (robot->config_FLP_type) // TODO: REMOVE ALL REFERENCES TO VESTIGIAL MODES OF FLIPPING
		{
		case MOVETYPE_NORMAL:*/
			robot->out_arm_direction = robot->config_FLP_direction;
			robot->out_arm_power = robot->config_FLP_power;
			/*break;
		case MOVETYPE_PULSED:
			robot->out_arm_direction = robot->config_FLP_direction;
			robot->out_arm_power = pulseLogic(robot->config_FLP_power, 0.2, 1.0, millis(), PERIOD_LONG_PULSE);
			break;
		case MOVETYPE_TWITCH:
			robot->out_arm_direction = twitchLogic(millis(), PERIOD_LONG_PULSE);
			robot->out_arm_power = robot->config_FLP_power;
			break;
		default:
			robot->out_arm_direction = MOTORDIR_IDLE;
			robot->out_arm_power = 0;
		}		*/
		break;
	case PHASE_COMPLETE:
		
		robot->state_phase = PHASE_FLP_MAIN;
		// Do nothing
		break;
	default:
		robot->state_phase = PHASE_FLP_MAIN;
		break;
	}
}

void behavior_prepare(dock_robot* robot)
{
	if (robot->state_phase == PHASE_INIT)
	{
		// Preparation behavior operates in a much more open-loop way than the ususal behaivors.
		// This is because the behavior typically does not require intervention and does not 
		// require the dock to react to any inputs other than those required to sense limits.
		// Consequently, the functions employed are less abstracted than those of other behaviors
		set_hoop_to_known_position	(robot,		  robot->config_PRP_direction	);
		set_arm_to_known_position	(robot,		  robot->config_PRP_direction	);
		set_hoop_to_known_position	(robot, (-1)*(robot->config_PRP_direction)	);
		robot->state_phase =	PHASE_COMPLETE;
	}
	else
	{
		// Do nothing
	}
}

void behavior_stop(dock_robot*	 robot)
{
	// Simply stop moving
	shutdown_all(robot);
}

void behavior_puppet(dock_robot* robot)
{
	switch (robot->state_phase)
	{
	case PHASE_INIT:
		robot->state_phase = PHASE_PUP_MAIN;
	case PHASE_PUP_MAIN:
		// check for timeout
		/*if (checkTime(robot->time_timeout_start, robot->time_this_measurement, robot->config_PUP_timeout))
		{
			shutdown_all(robot);
			robot->state_phase = PHASE_COMPLETE;
			break;
		}*/
		// Move according to command
		robot->out_hoop_direction	= robot->config_PUP_hoop_direction;
		robot->out_hoop_power		= bound(robot->config_PUP_hoop_power,0.0,1.0);
		robot->out_arm_direction	= robot->config_PUP_arm_direction;
		robot->out_arm_power		= bound(robot->config_PUP_arm_power,0.0,1.0);
		break;
	case PHASE_COMPLETE:
		shutdown_all(robot);
		// Do nothing
		break;
	default:
		robot->state_phase = PHASE_FLP_MAIN;
		break;
	}
}

//////////////////////////////////////////////////////
// Logic Helper Functions
void integrateValues(dock_robot* robot)
{
	// TODO: verify that the actual dt works
	float dt = ((float)(robot->time_this_measurement - robot->time_last_measurement)) * S_PER_MS;

	// Integrate the current IF the current is above some threshold
	if (robot->meas_current_dock > THRESH_INT_I_MIN)
	{	robot->calc_integrated_current += dt * robot->meas_current_dock;	}

	// Check if contact has been made (known position) If so, treat as going faster than usual
	// Approximate the hoop position
	if (!robot->sen_hoop_level && (robot->out_dock_state != HBMELT_GND))
	{
		robot->calc_approx_hoop_position += robot->calib_time_for_hoop_sweep * 10 * ((float)robot->safe_hoop_direction * (robot->safe_hoop_power)) * dt;
		robot->calc_approx_hoop_position = bound(robot->calc_approx_hoop_position, -1, 1);
	}
	else
	{
		robot->calc_approx_arm_position += (2/robot->calib_time_for_hoop_sweep ) * dt *
			((float)robot->safe_hoop_direction * (robot->safe_hoop_power - robot->calib_min_hoop_move_pwm / MAX_VALUE_PWM));

		robot->calc_approx_hoop_position = bound(robot->calc_approx_hoop_position, -1, 1);
	}

	// Approximate thearm position, 
	// then check if contact has been made (known position)
	if (robot->sen_armLim)
	{
		robot->calc_approx_arm_position = getFloatSign(robot->calc_approx_arm_position)*1.0;
	}
	else
	{
		robot->calc_approx_arm_position += (2/robot->calib_time_for_arm_move)* dt * ((float)robot->safe_arm_direction * 
			(robot->safe_arm_power - robot->calib_min_arm_move_pwm));
		robot->calc_approx_arm_position = bound(robot->calc_approx_arm_position, -1, 1);
	}
}