/*////////////////////////////////////////////////////
 Name:		def_config.h
 Created:	3/27/2019
 Author:	Petras Swissler

 Purpose:	Define all configuration constants to
			tweak dock behaviors
*/////////////////////////////////////////////////////
#pragma once

// Input Configuration Parameters
#define FILTER_ALPHA_HOOP_CURRENT		0.4
#define ADC_CHARGE_DISPERSE_COUNT		5
#define THRESH_INT_I_MIN				0.2

// Output Melt Configuration
#define CONFIG_MIN_MELT_VOLTAGE_RATIO	(float)0.1
#define CONFIG_MAX_MELT_VOLTAGE_RATIO	(float)1.1

// ATTACHMENT PARAMETERS
#define CONFIG_TIME_TO_ALLOW_COOL_BEFORE_HOOP_RETRACT	120
#define CONFIG_TIME_TO_TRY_TO_PREPARE_HOOP 5
#define CONFIG_TIME_TO_PREPARE_HOOP_IF_PREPARE_FAILED	1.5
#define CONFIG_SWEEP_INIT_PULSE_LOW		0.15
#define CONFIG_SWEEP_INIT_PULSE_HIGH	0.65

// DETACH PARAMETERS
#define CONFIG_DETACH_DT1_MIN_T			3
#define CONFIG_DETACH_DT1_MAX_T			20
#define CONFIG_DEGRADATION_OF_MAX_CURRENT	0.995
#define CONFIG_DETACH_CUR_BREAK_RATIO	0.4
#define CONFIG_DETACH_MIN_CURRENT		0.05
#define CONFIG_DETACH_BUFFER_TIMEOUT	5.0
#define CONGIG_DETACH_NUM_CURRENT_BELOW_THRESHOLD	50
#define CONFIG_DETACH_BUFFER_TIME		0.75
#define CONFIG_REQ_CURRENT_DT1			0.5

// SAFETY PARAMETERS
#define SAFETY_ARM_BACKOFF_TIME_S		1.7
#define SAFETY_ARM_MAX_BACKOFF_TIME_S	10