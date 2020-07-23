/*////////////////////////////////////////////////////
 Name:		inputs.cpp
 Created:	3/25/2019
 Author:	Petras Swissler

 Purpose:	Update measurements for all inputs
*/////////////////////////////////////////////////////
#include "inputs.h"

// Main Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void measureSensors(dock_robot* robot)
{
	updateSenseTimes(robot);
	// Current Sensors
	senseDockCurrent(robot);
	senseHoopCurrent(robot);
	// Dock Voltage
	senseDockVoltage(robot);
	// Hoop Limit
	checkHoopLevel(robot);
	// Arm Limit
	checkArmLimit(robot);
	// Dock Level
	checkDockLevel(robot);
}

// Low-Level (hardware-specific) code for checking sensors ~~~~~~~~~~~~~~~~
void updateSenseTimes(dock_robot* robot)
{
	robot->time_last_measurement = robot->time_this_measurement;
	robot->time_this_measurement = millis();
}

// Check Current using the four INA169 current sensors used on the board
void senseDockCurrent(dock_robot* robot)
{
	// Check for current inflow
	if (getMaxofN(PIN_ASEN_IDOCK_INOUT_LOW, 10) < 300)
	{
		// Assign an arbitrary negative current
		robot->meas_current_dock = -0.2;
	}
	else
	{
		// Current measurement depends on: Adc reading, calibration value, duty cycle of melt
		robot->meas_current_dock = (getMaxofN(PIN_ASEN_IDOCK_OUT_HIGH, READS_FOR_SENSING_DOH)-robot->calib_adc_zero_Isense_dock_high_out)
			* robot->calib_cs_do_hi_a_per_adc_per_dc * ((robot->out_melt_dock_voltage) / NOM_BATTERY_VOLTAGE);
	}
}

void senseHoopCurrent(dock_robot* robot)
{
	robot->meas_current_hoop = expFilter(
		(getMaxofN(PIN_ASEN_IHOOP_IN_LOW, READS_FOR_SENSING_HIL) - robot->calib_adc_zero_Isense_hoop_low_in) * robot->calib_cs_hi_lo_a_per_adc_per_dc * ((robot->out_melt_dock_voltage) / NOM_BATTERY_VOLTAGE),
		robot->meas_current_hoop,
		FILTER_ALPHA_HOOP_CURRENT);
		//0.6*robot->meas_current_hoop + 	0.4*(getMaxofN(PIN_ASEN_IHOOP_IN_LOW, READS_FOR_SENSING_HIL)-robot->calib_adc_zero_Isense_hoop_low_in) * 	robot->calib_cs_hi_lo_a_per_adc_per_dc * ((robot->out_melt_dock_voltage) / NOM_BATTERY_VOLTAGE);
}

// Check the dock voltage using the voltage divider
void senseDockVoltage(dock_robot* robot)
{
	robot->meas_dock_voltage = getAvgReading(PIN_ASEN_VDOCK, READS_FOR_SENSING) * robot->calib_vdock_per_adc;
}

// Check whether the hoop has hit the limit (pull-up resistor used)
void checkHoopLevel(dock_robot* robot)
{
	robot->sen_hoop_level = digitalRead(PIN_DI_HOOPLEVEL);
}

// Check whether the arm has hit the limit (pull-up resistor used)
// Hit limit? True. Not Hit limit? False
void checkArmLimit(dock_robot* robot)
{
	robot->sen_armLim = !digitalRead(PIN_DI_ARMLIMIT);
}

// Check whether the dock is excited to some threshold voltage (comparator used)
void checkDockLevel(dock_robot* robot)
{
	robot->sen_dockLevel = digitalRead(PIN_DI_DOCKLEVEL);
}

// Function to read all analog inputs some number of times to remove any charge buildup ont he ADC lines
void removeAnalogChargeBuildup(void)
{
	for (uint8_t ii = 0; ii < ADC_CHARGE_DISPERSE_COUNT; ii++)
	{
		analogRead(PIN_ASEN_VBAT_MINUS		);
		analogRead(PIN_ASEN_IDOCK_OUT_HIGH	);
		analogRead(PIN_ASEN_IDOCK_INOUT_LOW	);
		analogRead(PIN_ASEN_IHOOP_IN_LOW	);
		analogRead(PIN_ASEN_VDOCK			);
	}
}