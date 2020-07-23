/*////////////////////////////////////////////////////
 Name:		dock_robot.cpp
 Created:	3/25/2019
 Author:	Petras Swissler

 Purpose:	Create a class for all robot parameters
			sensor values, and desired output
*/////////////////////////////////////////////////////
#include "dock_robot.h"

dock_robot::dock_robot(void)
{	
	// Logic Tracking Values ~~~~~~~~~~~~~~~~~~~~~~~~~
	state_phase				= PHASE_INIT;
	last_command_behavior	= MSG_I2C_COMMAND_STOP;
	trackTimer0				= millis();
	trackTimer1				= millis();
	safety_arm_limit_timeout = 0;

	// Measured Values ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Analog
	meas_current_dock	= 0;
	meas_current_hoop	= 0;
	meas_vbat_negative	= 0;
	meas_dock_voltage	= 0;
	// Digital
	sen_armLim		= LOW;
	sen_dockLevel	= LOW;
	sen_hoop_level	= LOW;
	// Other
	time_this_measurement	= millis();
	time_last_measurement	= millis();
	time_timeout_start		= millis();

	// Communication Values ~~~~~~~~~~~~~~~~~~~~~~~~~~
	self_ID	= DOCK_I2C_ADDR_UNKNOWN;
	status						= 0;
	com_dock_communication_in	= 0;
	com_dock_communication_out	= 0;

	// Output Values ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	out_melt_dock_voltage		= 0;
	out_melt_dock_current_limit	= 0;
	out_desired_current			= 0;
	out_max_melt_voltage		= 0;
	out_dock_state				= HBMELT_HI_Z;

	out_arm_power				= 0;
	out_arm_direction			= MOTORDIR_IDLE;

	out_hoop_power				= 0;
	out_hoop_direction			= MOTORDIR_IDLE;

	config_move_motor_type		= 0;
	com_msg_to_send				= 0;

	// Safety-Modified Output Values
	safe_hoop_power				= 0;
	safe_hoop_direction			= 0;
	safe_arm_power				= 0;
	safe_arm_direction			= 0;
	safe_melt_dock_voltage		= 0;

	// Commands from Centerbody ~~~~~~~~~~~~~~~~~~~~~~
	command_timeout				= 500;
	command_behavior			= MSG_I2C_COMMAND_STOP;

	config_SWP_I_target			= 0;
	config_SWP_voltage			= 0;
	config_SWP_hoop_motor_power = 0;
	config_SWP_hoop_motor_pwr_growth_period = 0;

	config_ATT_I_max = 0;
	config_ATT_V_start = 0;
	config_ATT_V_growth_period = 0;
	config_ATT_V_max = 0;
	config_ATT_integrated_I_target = 0;
	config_ATT_sweep_direction = MOTORDIR_FWD;

	config_DET_DT1_integrated_I_target = 0;
	config_DET_DT1_max_I = 0;
	config_DET_DT1_max_V = 0;
	config_DET_DT2_max_I = 0;
	config_DET_DT2_max_V = 0;
	config_DET_hoop_motor_power = 0;
	config_DET_hoop_motor_mode = 0;
	config_DET_arm_twitch_power = 0;
	config_DET_sweep_direction = MOTORDIR_FWD;

	config_DT1_integratedCurrent = 0;
	config_DT1_max_current = 0;
	config_DT1_max_voltage = 0;
	config_DT2_max_current = 0;

	config_FLP_direction = 0;
	config_FLP_power = 0;
	config_FLP_timeout = 0;
	config_FLP_type = 0;

	config_PUP_hoop_direction = 0;
	config_PUP_hoop_power = 0;
	config_PUP_arm_direction = 0;
	config_PUP_arm_power = 0;
	config_PUP_timeout = 0;

	config_RPT_whatToReport = I2C_SEND_IDOCK;

	config_RPT_whatToSend = DOCK_MSG_NOTHING;

	// Calculated Values ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	calc_integrated_current = 0;
	calc_approx_hoop_position = 0;
	calc_approx_arm_position = 0;

	calc_max_current_detected = 0;

	// Calibrated Values ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	calib_adc_zero_Isense_dock_high_out = 0;
	calib_adc_zero_Isense_dock_low_inout = 0;
	calib_adc_zero_Isense_dock_low_in = 0;
	calib_adc_zero_Isense_hoop_low_in = 0;

	calib_cs_do_hi_a_per_adc_per_dc	= 1;
	calib_cs_dio_lo_a_per_adc_per_dc= 1;
	calib_cs_hi_lo_a_per_adc_per_dc = 1;
	calib_vbatminus_per_adc		= 1;
	calib_vdock_per_adc			= 1;
	calib_time_for_hoop_sweep	= 1;
	calib_time_for_arm_move		= 1;
	calib_min_hoop_move_pwm		= 1;
	calib_min_arm_move_pwm		= 1;

	// Scaling Values ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	scale_low = 1.0;
	scale_high = 1.0;

	// Debug Scratch Values ~~~~~~~~~~~~~~~~~~~~~~~~~~
	debug_int = 0;
	debug_uint8_1 = 0;
	debug_uint8_2 = 0;
	debug_float = 0;
}


dock_robot::~dock_robot()
{
}
