/*////////////////////////////////////////////////////
 Name:		def_calibration.h
 Created:	3/25/2019
 Author:	Petras Swissler

 Purpose:	Define calibration constants for sensors
*/////////////////////////////////////////////////////
#pragma once
// Current Sensor Calibrations
//#define CALIB_DOCK_AMPS_PER_ADC_HI		robot->calib_cs_do_hi_a_per_adc
//#define CALIB_DOCK_AMPS_PER_ADC_LO		robot->calib_cs_dio_lo_a_per_adc
//#define CALIB_HOOP_AMPS_PER_ADC			robot->calib_cs_hi_lo_a_per_adc

#define CONFIG_IDO_LOW_ADC_CUTOFF		200
//This is the amount above the neutral value that is required to change current sensors

// Voltage Calibration
//#define CALIB_VDOCK_VOLTS_PER_ADC		robot->calib_vdock_per_adc
//#define CALIB_VBATMINUS_VOLTS_PER_ADC	robot->calib_vbatminus_per_adc

// Motor Speed Properties (Zero to limit / second at full speed)
//#define HOOP_SPEED						robot->calib_time_for_hoop_sweep
//#define ARM_SPEED						robot->calib_time_for_arm_move

#define HOOP_MIN_PWM					robot->calib_min_hoop_move_pwm
#define ARM_MIN_PWM						robot->calib_min_arm_move_pwm