#pragma once
#include <stdint.h>
#include "def_friendly_names.h"
#include "def_commInterpretation.h"
#include "Arduino.h"
class robot_centerbody
{
public:
	robot_centerbody();
	~robot_centerbody();

	// Sensed Values
	unsigned long time_this_measurement;
	unsigned long time_prev_measurement;

	float meas_fsr_force;

	float meas_grav_x;
	float meas_grav_y;
	float meas_grav_z;

	float heading;
	float meas_mag_x;
	float meas_mag_y;
	float meas_mag_z;

	// Calibration Values
	float calib_fsr;
	float calib_adc0_fsr_A;
	float calib_adc0_fsr_B;
	
	// Calculated Values
	float calib_gyro_x;
	float calib_gyro_y;
	float calib_gyro_z;
	float calc_flip_speed;

	// Logic Values
	uint8_t dock_A;
	uint8_t dock_B;
	uint8_t dock_C;

	uint8_t action_dock;
	uint8_t movement_dock;
	uint8_t passive_dock;

	uint8_t dockState_action ;
	uint8_t dockPhase_action ;
	uint8_t dockState_move 	 ;
	uint8_t dockPhase_move 	 ;
	uint8_t dockState_passive;
	uint8_t dockPhase_passive;
	uint8_t cb_phase_action;

	uint8_t step_direction;
	uint8_t step_phase;

	float  desired_force;
	int8_t sweep_direction;
	int8_t pull_direction;
	int8_t pressDirection;
	int8_t flip_direction;
	// State Values
	uint8_t state;
	unsigned long stateTimer0;

	// Received values
	float attachDock_dockCurrent;
	float attachDock_intCurrent;

	// Automated Step docks
	uint8_t PARAM_PARAM_ASTEP_dock_detach;
	uint8_t PARAM_PARAM_ASTEP_dock_move;
	uint8_t PARAM_PARAM_ASTEP_dock_attach;
	uint8_t PARAM_PARAM_ASTEP_direction;
	
	// DEBUG STUFF xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	uint8_t debug_state;
	float tempFloat;
	uint8_t tempUInt8;
	int tempInt;
	unsigned long timer_debug;
	// DEBUG STUFF xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

	// TEST RIG STUFF xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	float lastForce;
	float linacDisplacement;
	int zeroLinacDisplacement;
	int currentTrial;
	// TEST RIG STUFF xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

	// PUPPET STUFF xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	uint8_t pup_mode;
	uint8_t pup_moveDock_number;
	uint8_t pup_actionDock_number;
	uint8_t pup_passiveDock_number;
	float	dock_movement_timeout;

	// PUPPET STUFF xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

	// HARDWARE CONFIGURATION STUFF xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	float vec_d0_x;
	float vec_d0_y;
	float vec_d0_z;
	float vec_d1_x;
	float vec_d1_y;
	float vec_d1_z;
	float vec_d2_x;
	float vec_d2_y;
	float vec_d2_z;

	// HARDWARE CONFIGURATION STUFF xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	float attach_melt_integrated_current;

	// Automated Step Stuff xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	int8_t astep_direction;
	uint8_t astep_dock_attach;
	uint8_t astep_dock_detach;
	uint8_t astep_dock_move;
	uint8_t astep_dock_passive;
	int8_t astep_isForwardMotion;
	uint8_t astep_phase;
	float att_sweep_motor_power;

	// Easy Step Stuff xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	uint8_t ez_mode_active;
	uint8_t ez_movement_dock;
	uint8_t ez_attached_dock;
	uint8_t ez_dock_to_detach;

	// FSR Calibration Values xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	float fsr_cal_0f ;
	float fsr_cal_0r ;
	float fsr_cal_1f ;
	float fsr_cal_1r ;
	float fsr_cal_2f ;
	float fsr_cal_2r ;

	// TO DO: SORT THIS LATER xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	bool isReverseAttachment;
};

