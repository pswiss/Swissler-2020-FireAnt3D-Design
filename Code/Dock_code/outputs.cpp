/*///////////////////////////////////////////////////
Name:		outputs.h
Created :	3 / 25 / 2019
Author :	Petras Swissler

Purpose :	Run all hardware outputs safely
*/////////////////////////////////////////////////////
#include "outputs.h"

// Main Output Function
void executeOutputs(dock_robot* robot)
{
	// Run safety checks and calculate safe values
	modifyOutputsForSafety(robot);
	// Run motors
	output_hoopMotor(robot);
	output_armMotor(robot);
	// Run melt1
	output_meltOutput(robot);
}

// Shutdown function
void shutdown_all(dock_robot* robot)
{
	robot->out_arm_power			= 0;
	robot->out_hoop_power			= 0;
	robot->out_arm_direction		= MOTORDIR_IDLE;
	robot->out_hoop_direction		= MOTORDIR_IDLE;
	robot->safe_arm_power			= 0;
	robot->safe_hoop_power			= 0;
	robot->safe_arm_direction		= MOTORDIR_IDLE;
	robot->safe_hoop_direction		= MOTORDIR_IDLE;
	robot->out_melt_dock_voltage	= 0;
	robot->out_dock_state			= HBMELT_HI_Z;
	
	executeOutputs(robot);
}

//////////////////////////////////////////////////////
// Safety Functions
void modifyOutputsForSafety(dock_robot* robot)
{
	// Default all safe values to the commanded values
	safety_valueInitialize(robot);
	
	// Modify safe values based on system conditions
	safety_armLimit(robot);
	//safety_hoopLimit(robot);
	//safety_hoopPushed(robot);
	//safety_overCurrent(robot);
}

void safety_valueInitialize(dock_robot* robot)
{
	robot->safe_hoop_power			= robot->out_hoop_power;
	robot->safe_hoop_direction		= robot->out_hoop_direction;
	robot->safe_arm_power			= robot->out_arm_power;
	robot->safe_arm_direction		= robot->out_arm_direction;
	robot->safe_melt_dock_voltage	= robot->out_melt_dock_voltage;
}

void safety_armLimit(dock_robot* robot)
{
	int8_t old_forbidden_direction = robot->safety_forbidden_arm_direction;
	int8_t old_backing_off_state = robot->safety_arm_backing_off_state;
	int8_t old_last_directional_command = robot->safety_last_directional_command;
	// Require check for instances where limit is hit during same frame as stop command recieved or when contact happens after motion
	if (((robot->out_arm_direction == MOTORDIR_FWD) 
			|| (robot->out_arm_direction == MOTORDIR_REV))
		&& ( robot->safety_forbidden_arm_direction == MOTORDIR_NONE))
	{	robot->safety_last_directional_command = robot->out_arm_direction;	}

	// Condition: Arm is trying to push past its limit
	if (robot->sen_armLim)
	{
#ifdef SERIAL_ENABLE
		Serial.println(); Serial.println("Hit");
#endif
		// Need to back off slightly (should help greatly with between-run issues)
		robot->safety_arm_timer = robot->time_this_measurement;
		robot->safety_arm_limit_timeout = robot->time_this_measurement;
		robot->safety_arm_backing_off_state = SAFETY_ARM_BACKOFF;
		if (robot->safety_forbidden_arm_direction == MOTORDIR_NONE)
		{
#ifdef SERIAL_ENABLE
			Serial.println("UpdateDirection");
#endif
			robot->safety_forbidden_arm_direction = robot->safety_last_directional_command;
		}
	}

	// Back off if configured
	if (robot->safety_arm_backing_off_state == SAFETY_ARM_BACKOFF)
	{
#ifdef SERIAL_ENABLE
		Serial.print("BKoff");
#endif
		if (!checkTime(robot->safety_arm_timer, robot->time_this_measurement, SAFETY_ARM_BACKOFF_TIME_S))
		{
#ifdef SERIAL_ENABLE
			Serial.println("ON");
#endif
			// If I'm backing off for too long, then something has gone horribly wrong
			if (!checkTime(robot->safety_arm_limit_timeout, robot->time_this_measurement, SAFETY_ARM_MAX_BACKOFF_TIME_S))
			{
				robot->safe_arm_direction	= ((-1) * (robot->safety_forbidden_arm_direction));
				robot->safe_arm_power		= 1.0;
			}
			else
			{
				shutdown_all(robot);
				robot->safe_arm_direction = MOTORDIR_IDLE;
				robot->safe_arm_power = 0.0;
#ifdef SERIAL_ENABLE
				Serial.println("Arm Limit backoff override")
#endif // SERIAL_ENABLE
			}

		}
		else
		{
			robot->safety_arm_backing_off_state = SAFETY_ARM_OK;
			robot->safe_arm_direction			= MOTORDIR_IDLE;
		}
	}
	// otherwise, need to check if need to stop moving
	else
	{
		if (robot->out_arm_direction == robot->safety_forbidden_arm_direction)
		{
			robot->safe_arm_direction	= MOTORDIR_IDLE;
			robot->safe_arm_power		= 0.0;
		}
		// If moving away now, there is now no forbidden direction
		if (robot->out_arm_direction	== ((-1) * robot->safety_forbidden_arm_direction))
		{
			robot->safety_forbidden_arm_direction = MOTORDIR_NONE;
		}
	}

	// Store safety values to EEPROM to maintain values between reset
	// Only do this when value changes to preserve integrity of EEPROM
#ifdef ENABLE_USE_OF_EEPROM
	if (old_forbidden_direction != robot->safety_forbidden_arm_direction)
	{
		EEPROM.update(EEPROM_ADDR_SFAD, s2u_int8(robot->safety_forbidden_arm_direction));
	}
	else	{	}
	if (old_backing_off_state != robot->safety_arm_backing_off_state)
	{
		EEPROM.update(EEPROM_ADDR_SABOS, s2u_int8(robot->safety_arm_backing_off_state));
	}
	else	{	}
	if (old_last_directional_command != robot->safety_last_directional_command)
	{
		EEPROM.update(EEPROM_ADDR_SLDC, s2u_int8(robot->safety_last_directional_command));
	}
	else	{	}
#endif
}

void safety_hoopLimit(	dock_robot* robot)
{
	// Condition: Hoop is trying to push past its limit
	if ((robot->sen_hoop_level==0)&&(robot->out_dock_state==HBMELT_HI_Z))
	{
		// Only need to stop if moving into the limit (ok if moving away)
		if (getIntSign(robot->out_hoop_direction) == getFloatSign(robot->calc_approx_hoop_position))
		{
			robot->safe_hoop_power = 0;
			robot->safe_hoop_direction = MOTORDIR_IDLE;
		}
	}
}

void safety_hoopPushed(	dock_robot* robot)
{
	// Condition: Arm is pushing in such a way that it would force the hoop to move
	if ((robot->sen_hoop_level==0)&&(robot->safe_arm_direction != MOTORDIR_BRAKE)&&(robot->safe_arm_direction !=MOTORDIR_IDLE))
	{
		// Only a problem if arm is moving towards hoop
		if (getIntSign(robot->out_arm_direction) == getFloatSign(robot->calc_approx_hoop_position))
		{
			robot->safe_hoop_power = 0.05;
			robot->safe_hoop_direction = (int)(-1 * getIntSign(robot->out_arm_direction));
		}
	}
}

void safety_overCurrent(	dock_robot* robot)
{
	// Condition: Current is at limit of what is
	// allowable for the H-bridge
	if (robot->meas_current_dock >= DESIRED_MAX_CURRENT)
	{	robot->safe_melt_dock_voltage = (robot->out_melt_dock_voltage) * (DESIRED_MAX_CURRENT / robot->meas_current_dock) * 0.75;	}
}

//////////////////////////////////////////////////////
// Output Functions
void output_hoopMotor(		dock_robot* robot)
{
	// All low-level implementation of the h-bridge 
	// control is handled by the h-bridge function
#ifdef ENABLE_HOOP_MOTOR
	hw_hb_lv8548mc(robot->safe_hoop_direction, robot->safe_hoop_power, PIN_PWM_HOOP_MOTOR_A, PIN_PWM_HOOP_MOTOR_B);
#endif // ENABLE_HOOP_MOTOR
}

void output_armMotor(		dock_robot* robot)
{
	// All low-level implementation of the h-bridge 
	// control is handled by the h-bridge function

	// Override condition: Never want to move arm during attach
#ifdef OVERRIDE_ARM_MOTOR_DURING_ATT
	if(robot->command_behavior == MSG_I2C_COMMAND_ATTACH)
	{
#ifdef ENABLE_ARM_MOTOR
		hw_hb_lv8548mc(MOTORDIR_BRAKE, 0.0, PIN_PWM_ARM_MOTOR_A, PIN_PWM_ARM_MOTOR_B);
#endif
	}
	else
	{
#ifdef ENABLE_ARM_MOTOR
		hw_hb_lv8548mc(robot->safe_arm_direction, robot->safe_arm_power, PIN_PWM_ARM_MOTOR_A, PIN_PWM_ARM_MOTOR_B);
#endif // ENABLE_ARM_MOTOR
	}
#else
#ifdef ENABLE_ARM_MOTOR
	hw_hb_lv8548mc(robot->safe_arm_direction, robot->safe_arm_power, PIN_PWM_ARM_MOTOR_A, PIN_PWM_ARM_MOTOR_B);
#endif // ENABLE_ARM_MOTOR
#endif

}

void output_meltOutput(		dock_robot*	robot)
{
	// todo: re-implement a method to target a specific current
	// All low-level implementation of the h-bridge control is handled by the h-bridge function
#ifdef ENABLE_MELT
	hw_hb_drv8871ddar(robot->out_dock_state, bound((robot->out_melt_dock_voltage) / NOM_BATTERY_VOLTAGE, 0, 1));
#endif // ENABLE_MELT
}

//////////////////////////////////////////////////////
// Output Logic Functions
// Pulse Logic
float pulseLogic(float baseValue, float lowScale, float highScale, unsigned long time, unsigned long pulsePeriod)
{
	if (time%pulsePeriod > (pulsePeriod / 2))
	{	return baseValue * highScale;	}
	else
	{	return baseValue * lowScale;	}
}

// TwitchLogic
int twitchLogic(unsigned long nowTime, unsigned long twitchPeriod)
{
	if (nowTime%twitchPeriod > (twitchPeriod / 2))
	{	return MOTORDIR_FWD;	}
	else
	{	return MOTORDIR_REV;	}
}

// LED Functions
void ledON(void)
{
#ifdef ENABLE_LED
	digitalWrite(PIN_DO_DEBUG_LED, HIGH);
#endif // ENABLE_LED
}

void ledOFF(void)
{
#ifdef ENABLE_LED
	digitalWrite(PIN_DO_DEBUG_LED, LOW);
#endif // ENABLE_LED
}