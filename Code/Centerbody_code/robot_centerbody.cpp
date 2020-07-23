#include "robot_centerbody.h"

robot_centerbody::robot_centerbody()
{
	calib_fsr = 0;

	meas_fsr_force = 0;
	meas_grav_x = 0;
	meas_grav_y = 0;
	meas_grav_z = 0;
	calc_flip_speed = 0;
	action_dock = DOCK_0;
	movement_dock = DOCK_1;
	passive_dock = DOCK_2;
	state = CENTER_STATE_INITIALIZE;
	stateTimer0 = millis();
	time_this_measurement = millis();
	time_prev_measurement = millis();
	attachDock_dockCurrent = 0;
	attachDock_intCurrent = 0;
	att_sweep_motor_power = 0.5;

	dockState_action  = MSG_I2C_COMMAND_STOP;
	dockState_move	  = MSG_I2C_COMMAND_STOP;
	dockState_passive = MSG_I2C_COMMAND_STOP;
	dockPhase_action  = PHASE_INIT;
	dockPhase_move	  = PHASE_INIT;
	dockPhase_passive = PHASE_INIT;

	step_direction = MOTORDIR_IDLE;

	attach_melt_integrated_current = 25;
}

robot_centerbody::~robot_centerbody()
{
}
