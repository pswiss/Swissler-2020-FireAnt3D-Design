/*////////////////////////////////////////////////////
 Name:		dock_robot.cpp
 Created:	3/25/2019
 Author:	Petras Swissler

 Purpose:	Create a class for all robot parameters
			sensor values, and desired output
*/////////////////////////////////////////////////////
#pragma once
#include "def_friendly_names.h"
#include "def_commInterpretation.h"
#include "Arduino.h"
//////////////////////////////////////////////////////
class dock_robot
{
public:
	dock_robot();
	~dock_robot();
	// Logic Tracking Values ~~~~~~~~~~~~~~~~~~~~~~~~~
	uint8_t			state_phase;
	uint8_t			last_command_behavior;
	unsigned long	trackTimer0;
	unsigned long	trackTimer1;
	unsigned long	safety_arm_limit_timeout;

	// Measured Values ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Analog
	float			meas_current_dock;
	float			meas_current_hoop;
	float			meas_vbat_negative;
	float			meas_dock_voltage;
	// Digital
	uint8_t			sen_armLim;
	uint8_t			sen_dockLevel;
	uint8_t			sen_hoop_level;
	// Other
	unsigned long	time_this_measurement;
	unsigned long	time_last_measurement;
	unsigned long	time_timeout_start;

	// Communication Values ~~~~~~~~~~~~~~~~~~~~~~~~~~
	uint8_t			self_ID;
	uint8_t			status;
	uint8_t			com_dock_communication_in;
	uint8_t			com_dock_communication_out;

	// Output Values ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float			out_melt_dock_voltage;
	float			out_melt_dock_current_limit;
	float			out_desired_current;
	float			out_max_melt_voltage;
	int				out_dock_state;

	float			out_arm_power;
	int				out_arm_direction;

	float			out_hoop_power;
	int				out_hoop_direction;

	uint8_t			config_move_motor_type;
	uint8_t			com_msg_to_send;

	// Safety-Modified Output Values
	float			safe_hoop_power;
	int				safe_hoop_direction;
	float			safe_arm_power;
	int				safe_arm_direction;
	float			safe_melt_dock_voltage;

	// Commands from Centerbody ~~~~~~~~~~~~~~~~~~~~~~
	float			command_timeout;
	uint8_t			command_behavior;

	float			config_SWP_I_target;
	float			config_SWP_voltage;
	float			config_SWP_hoop_motor_power;
	float			config_SWP_hoop_motor_pwr_growth_period;

	float			config_ATT_I_max;
	float			config_ATT_V_start;
	float			config_ATT_V_growth_period;
	float			config_ATT_V_max;
	float			config_ATT_integrated_I_target;
	int8_t			config_ATT_sweep_direction;

	float			config_DET_DT1_integrated_I_target;
	float			config_DET_DT1_max_I;
	float			config_DET_DT1_max_V;
	float			config_DET_DT2_max_I;
	float			config_DET_DT2_max_V;
	float			config_DET_hoop_motor_power;
	float			config_DET_hoop_motor_mode;
	float			config_DET_arm_twitch_power;
	int8_t			config_DET_sweep_direction;

	float			config_DT1_integratedCurrent;
	float			config_DT1_max_current;
	float			config_DT1_max_voltage;
	float			config_DT2_max_current;

	int8_t			config_FLP_direction;
	float			config_FLP_power;
	float			config_FLP_timeout;
	uint8_t			config_FLP_type;

	int8_t			config_PRP_direction;

	int8_t			config_PUP_hoop_direction;
	float			config_PUP_hoop_power;
	int8_t			config_PUP_arm_direction;
	float			config_PUP_arm_power;
	float			config_PUP_timeout;

	uint8_t			config_RPT_whatToReport;

	uint8_t			config_RPT_whatToSend;

	// Calculated Values ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float			calc_integrated_current;
	float			calc_approx_hoop_position;
	float			calc_approx_arm_position;

	float			calc_max_current_detected;
	float			calc_min_current_detected;

	uint32_t		count_numCurrentMeasBelowThreshold;

	// Calibrated Values ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Calibration Zeroes
	float			calib_adc_zero_Isense_dock_high_out;
	float			calib_adc_zero_Isense_dock_low_inout;
	float			calib_adc_zero_Isense_dock_low_in;
	float			calib_adc_zero_Isense_hoop_low_in;
	// Linear Calibration Slopes
	
	float			calib_cs_do_hi_a_per_adc_per_dc	;
	float			calib_cs_dio_lo_a_per_adc_per_dc;
	float			calib_cs_hi_lo_a_per_adc_per_dc	;
	float			calib_vbatminus_per_adc		;
	float			calib_vdock_per_adc			;
	float			calib_time_for_hoop_sweep	;
	float			calib_time_for_arm_move		;
					
	uint8_t			calib_min_hoop_move_pwm	  	;
	uint8_t			calib_min_arm_move_pwm	  	;

	// Safety Tracking Values ~~~~~~~~~~~~~~~~~~~~~~~~
	int				safety_forbidden_arm_direction;
	int				safety_last_directional_command;
	uint8_t			safety_arm_backing_off_state;
	unsigned long	safety_arm_timer;

	// Scaling Values ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float			scale_low;
	float			scale_high;

	// Debug Scratch Values ~~~~~~~~~~~~~~~~~~~~~~~~~~
	int				debug_int;
	uint8_t			debug_uint8_1;
	uint8_t			debug_uint8_2;
	float			debug_float;
};

