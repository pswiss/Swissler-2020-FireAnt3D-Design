/*////////////////////////////////////////////////////
 Name:		startup_functions.h
 Created:	3/25/2019
 Author:	Petras Swissler

 Purpose:	Functions to run before anything else
*/////////////////////////////////////////////////////

#include "startup_functions.h"

// Highest-Level startup function
void robot_startup(dock_robot* robot)
{
	// Configure the board
	boardConfig();
	shutdown_all(robot);

	for(uint8_t ii = 0; ii < 4; ii++)
	{
		digitalWrite(PIN_DO_DEBUG_LED, HIGH); delay(15); digitalWrite(PIN_DO_DEBUG_LED, LOW); delay(85);
	}
	
	// Set all outputs to off
	shutdown_all(robot);
	robot->command_behavior = MSG_I2C_COMMAND_STOP;
	// Explicitly put all motors at idle and put dock at high-z 
	hw_melt_hb(HBMELT_HI_Z,		0.0);
	hw_motor_hb(MOTORDIR_BRAKE, 0.0, pins_armMotor);
	hw_motor_hb(MOTORDIR_BRAKE, 0.0, pins_hoopMotor);
		
	// Initialize calculated values
#ifdef ENABLE_USE_OF_EEPROM
	if ((EEPROM.read(EEPROM_ADDR_INTEGRITY_0) == VAL_INTEGRITY_0) && (EEPROM.read(EEPROM_ADDR_INTEGRITY_1) == VAL_INTEGRITY_1))
	{
		robot->safety_forbidden_arm_direction	= u2s_int8(EEPROM.read(EEPROM_ADDR_SFAD));
		robot->safety_arm_backing_off_state		=			EEPROM.read(EEPROM_ADDR_SABOS );
		robot->safety_last_directional_command	= u2s_int8(EEPROM.read(EEPROM_ADDR_SLDC));
	}
	else
	{
		robot->safety_forbidden_arm_direction	= MOTORDIR_NONE;
		robot->safety_arm_backing_off_state		= SAFETY_ARM_OK;
		robot->safety_last_directional_command = MOTORDIR_IDLE;
	}
	EEPROM.update(EEPROM_ADDR_INTEGRITY_0, VAL_INTEGRITY_0);
	EEPROM.update(EEPROM_ADDR_INTEGRITY_1, VAL_INTEGRITY_1);
#else
	robot->safety_forbidden_arm_direction = MOTORDIR_NONE;
	robot->safety_arm_backing_off_state = SAFETY_ARM_OK;
	// Setup the safety code
	robot->safety_arm_backing_off_state = SAFETY_ARM_OK;
	robot->safety_last_directional_command = MOTORDIR_FWD;
	robot->safety_arm_timer = millis();
	robot->safety_forbidden_arm_direction = MOTORDIR_NONE
#endif
		
	// Calibrate sensors
	removeAnalogChargeBuildup();
	calibrateSensors(robot);
	robot->calc_integrated_current		= 0;
	robot->calc_max_current_detected	= 0;
	
	// Determine robot I2C ID
	get_i2c_ID(robot);

	
	for (uint8_t ii = 0; ii < 4; ii++)
	{
		digitalWrite(PIN_DO_DEBUG_LED, HIGH); delay(15); digitalWrite(PIN_DO_DEBUG_LED, LOW); delay(85);
	}
}

// Configure all pins as inputs and outputs
void boardConfig(void)
{
	// Analog Pins
	pinMode(PIN_ASEN_VBAT_MINUS					, INPUT);
	pinMode(PIN_ASEN_IDOCK_OUT_HIGH				, INPUT);
	pinMode(PIN_ASEN_IDOCK_INOUT_LOW			, INPUT);
	pinMode(PIN_ASEN_IHOOP_IN_LOW				, INPUT);
	pinMode(PIN_ASEN_VDOCK						, INPUT);

	// Digital Outputs
	pinMode(PIN_DO_ENABLE_SEN_VBAT_MINUS		, OUTPUT);
	pinMode(PIN_PWM_HOOP_MOTOR_A				, OUTPUT);
	pinMode(PIN_PWM_HOOP_MOTOR_B				, OUTPUT);
	pinMode(PIN_PWM_ARM_MOTOR_A					, OUTPUT);
	pinMode(PIN_PWM_ARM_MOTOR_B					, OUTPUT);
	pinMode(PIN_PWM_DOCK_MELT_DOCK				, OUTPUT);
	pinMode(PIN_PWM_DOCK_MELT_HOOP				, OUTPUT);
	pinMode(PIN_DO_DEBUG_LED					, OUTPUT);
	
	digitalWrite(PIN_DO_ENABLE_SEN_VBAT_MINUS	, LOW);
	digitalWrite(PIN_PWM_HOOP_MOTOR_A			, LOW);
	digitalWrite(PIN_PWM_HOOP_MOTOR_B			, LOW);
	digitalWrite(PIN_PWM_ARM_MOTOR_A			, LOW);
	digitalWrite(PIN_PWM_ARM_MOTOR_B			, LOW);
	digitalWrite(PIN_PWM_DOCK_MELT_DOCK			, LOW);
	digitalWrite(PIN_PWM_DOCK_MELT_HOOP			, LOW);
	digitalWrite(PIN_DO_DEBUG_LED				, LOW);

	// Digital Inputs
	pinMode( PIN_DI_HOOPLEVEL					, INPUT);
	pinMode( PIN_DI_DOCKLEVEL					, INPUT);
	pinMode( PIN_DI_ARMLIMIT					, INPUT_PULLUP);

	//***** DO NOT DO THIS WITH THE CURRENT MELT H-BRIDGE; POWER USE INCREASES WITH PWM FREQUENCY
	// Set PWM clocks for dock outputs (PWM for Pins 10 and 11 belong to timers 1 and 2 respectively)
	//TCCR1B = (TCCR1B & B11111000) | B00000001; // set timer 1 divisor to 1 for PWM frequency of 31372.55 Hz for D9 and D10
	//TCCR2B = (TCCR2B & B11111000) | B00000001; // for PWM frequency of 31372.55 Hz on D3 and D11
	// Timer 0 not modified since this would break the millis() and delay() functions
}

// Load calibrated zeros into the robot object
void calibrateSensors(dock_robot* robot)
{
	removeAnalogChargeBuildup();
	delay(5);
	// Get ADC zero offset for hoop current / dock current (1/2/3)
	robot->calib_adc_zero_Isense_dock_high_out	= getMaxofN(PIN_ASEN_IDOCK_OUT_HIGH,	10);	//getAvgReading(PIN_ASEN_IDOCK_OUT_HIGH, READS_FOR_CALIBRATION);
	robot->calib_adc_zero_Isense_dock_low_inout = getMaxofN(PIN_ASEN_IDOCK_INOUT_LOW,	10);	//getAvgReading(PIN_ASEN_IDOCK_INOUT_LOW, READS_FOR_CALIBRATION);
	robot->calib_adc_zero_Isense_hoop_low_in	= getMaxofN(PIN_ASEN_IHOOP_IN_LOW,		10);	//getAvgReading(PIN_ASEN_IHOOP_IN_LOW, READS_FOR_CALIBRATION);

	// Load robot calibration values
	loadCalib(robot);
}

// Load calibration values into robot object
void loadCalib(dock_robot* robot)
{
#ifdef HARDCODED_CALIBRATION_DATA
	robot->calib_cs_do_hi_a_per_adc_per_dc	= hc_calib_CS_DO_HI_A_per_ADC_perDC;
	robot->calib_cs_dio_lo_a_per_adc_per_dc	= hc_calib_CS_DIO_LO_A_per_ADC_perDC;
	robot->calib_cs_hi_lo_a_per_adc_per_dc	= hc_calib_CS_HI_LO_A_per_ADC_perDC;
	robot->calib_vbatminus_per_adc		= hc_calib_VBAT_minus_per_ADC;
	robot->calib_vdock_per_adc			= hc_calib_VDock_per_ADC;
	robot->calib_time_for_hoop_sweep	= hc_calib_timeForHoopSweep;
	robot->calib_time_for_arm_move		= hc_calib_timeForArmMove;
	robot->calib_min_hoop_move_pwm		= hc_calib_minHoopMovePWM;
	robot->calib_min_arm_move_pwm		= hc_calib_minArmMovePWM;
#else
	robot->calib_cs_do_hi_a_per_adc_per_dc	= readFloatFromEEPROM(	EEPROM_POSITION_CS_DO_HI_A_PER_ADC	);
	robot->calib_cs_dio_lo_a_per_adc_per_dc = readFloatFromEEPROM(	EEPROM_POSITION_CS_DIO_LO_A_PER_ADC	);
	robot->calib_cs_hi_lo_a_per_adc_per_dc	= readFloatFromEEPROM(	EEPROM_POSITION_CS_HI_LO_A_PER_ADC	);
	robot->calib_vbatminus_per_adc		= readFloatFromEEPROM(	EEPROM_POSITION_VBATMINUS_PER_ADC	);
	robot->calib_vdock_per_adc			= readFloatFromEEPROM(	EEPROM_POSITION_VDOCK_PER_ADC		);
	robot->calib_time_for_hoop_sweep	= readFloatFromEEPROM(	EEPROM_POSITION_TIME_FOR_HOOP_SWEEP	);
	robot->calib_time_for_arm_move		= readFloatFromEEPROM(	EEPROM_POSITION_TIME_FOR_ARM_MOVE	);
	robot->calib_min_hoop_move_pwm		= readFloatFromEEPROM(	EEPROM_POSITION_MIN_HOOP_MOVE_PWM	);	// todo: add related functionality
	robot->calib_min_arm_move_pwm		= readFloatFromEEPROM(	EEPROM_POSITION_MIN_ARM_MOVE_PWM	);	// todo: add related functionality
#endif // HARDCODED_CALIBRATION_DATA
}

// Functions to set the arm and hoop positions to known locations
void set_arm_to_known_position(dock_robot* robot,	int8_t mdir)
{
	// If at limit on startup, run until hit, then back off
	if (digitalRead(PIN_DI_ARMLIMIT))
	{
		while (digitalRead(PIN_DI_ARMLIMIT)&&(robot->command_behavior == MSG_I2C_COMMAND_PREPARE))
		{
#ifdef USE_WATCHDOG_TIMER
			wdt_reset();
#endif // USE_WATCHDOG_TIMER
			hw_motor_hb(mdir, 1.0, pins_armMotor);
		}
		hw_motor_hb(MOTORDIR_IDLE, 0.0, pins_armMotor);
	}
	// If not at limit on startup, run forward for some duration, check if still hit, then use this information to go to desired location
	else
	{
		hw_motor_hb(mdir, 1.0, pins_armMotor);
		delay(1000);
		hw_motor_hb(MOTORDIR_IDLE, 0.0, pins_armMotor);
		// If not at limit, know that is ok to keep going
		if (digitalRead(PIN_DI_ARMLIMIT))
		{
			while (digitalRead(PIN_DI_ARMLIMIT) && (robot->command_behavior == MSG_I2C_COMMAND_PREPARE))
			{
#ifdef USE_WATCHDOG_TIMER
				wdt_reset();
#endif // USE_WATCHDOG_TIMER
				hw_motor_hb(mdir, 1.0, pins_armMotor);
			}
			hw_motor_hb(MOTORDIR_IDLE, 0.0, pins_armMotor);
		}
		// Otherwise know that am at arm ilimit
	}
	// Back off until not at limit
	do 
	{ 
#ifdef USE_WATCHDOG_TIMER
		wdt_reset();
#endif // USE_WATCHDOG_TIMER
		hw_motor_hb((-1) * mdir, 1.0, pins_armMotor); 
	} while (!digitalRead(PIN_DI_ARMLIMIT));

#ifdef ENABLE_USE_OF_EEPROM
		EEPROM.update(EEPROM_ADDR_SFAD, s2u_int8(mdir));
		EEPROM.update(EEPROM_ADDR_SABOS, s2u_int8(SAFETY_ARM_OK));
		EEPROM.update(EEPROM_ADDR_SLDC, s2u_int8(mdir));
#endif

	hw_motor_hb((-1) * mdir, 1.0, pins_armMotor);
	delay(SAFETY_ARM_BACKOFF_TIME_S*MS_PER_S);
	hw_motor_hb(MOTORDIR_IDLE, 0.0, pins_armMotor);
	robot->safety_last_directional_command = mdir;
	robot->safety_forbidden_arm_direction = mdir;
	robot->calc_approx_arm_position = mdir;
}


void set_hoop_to_known_position(dock_robot* robot, int mdir)
{
	// Want to set to Position = 'mdir'
	// If at limit on startup, run forward until hit, then back off
	hw_melt_hb(HBMELT_HI_Z, 0.0);

	/*
	if (digitalRead(PIN_DI_HOOPLEVEL))
	{
		while (digitalRead(PIN_DI_HOOPLEVEL) && (robot->command_behavior == MSG_I2C_COMMAND_PREPARE))
		{
#ifdef USE_WATCHDOG_TIMER
			wdt_reset();
#endif // USE_WATCHDOG_TIMER
			hw_motor_hb(mdir, 1.0, pins_hoopMotor);
		}
		hw_motor_hb(MOTORDIR_IDLE, 0.0, pins_hoopMotor);
	}
	// If not at limit on startup, run forward for some duration, check if still hit, then use this information to go to desired location
	else
	{
		// Open-loop motion just to make sure that no longer at limit when start
		hw_motor_hb(mdir, 1.0, pins_hoopMotor);
		delay(1000);
		hw_motor_hb(MOTORDIR_IDLE, 0.0, pins_hoopMotor);
		// If not at limit, know that is ok to keep going
		if (digitalRead(PIN_DI_HOOPLEVEL))
		{
			while (digitalRead(PIN_DI_HOOPLEVEL) && (robot->command_behavior == MSG_I2C_COMMAND_PREPARE))
			{
#ifdef USE_WATCHDOG_TIMER
				wdt_reset();
#endif // USE_WATCHDOG_TIMER
				hw_motor_hb(mdir, 1.0, pins_hoopMotor);
			}
			hw_motor_hb(MOTORDIR_IDLE, 0.0, pins_hoopMotor);
		}
		// Otherwise know that am at hoop limit
	}
	*/

	// Just run the motor for a duration sufficient to guarantee hoop hitting its limit
	unsigned long startTime = millis();
	while (!checkTime(startTime, millis(), 5))
	{
#ifdef USE_WATCHDOG_TIMER
		wdt_reset();
#endif // USE_WATCHDOG_TIMER
		hw_motor_hb(mdir, 1.0, pins_hoopMotor);
	}

	hw_motor_hb(MOTORDIR_IDLE, 0.0, pins_hoopMotor);
	robot->calc_approx_hoop_position = mdir;
}
