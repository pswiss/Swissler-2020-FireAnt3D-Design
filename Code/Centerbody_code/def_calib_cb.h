/*///////////////////////////////////////////////////////////////////////
 Name:		def_calib.h
 Created:	3/30/2019
 Author:	Petras Swissler

 Purpose:	Define calibration constants for sensors
*////////////////////////////////////////////////////////////////////////
#pragma once

// FSR
#define CALIB_GRAMS_PER_ADC			robot->calib_fsr

/*#define CAL_G_PER_ADC_0_POS			-2.483966667
#define CAL_G_PER_ADC_0_NEG			-10.0715
#define CAL_G_PER_ADC_1_POS			-12.29562
#define CAL_G_PER_ADC_1_NEG			-25.44142857
#define CAL_G_PER_ADC_2_POS			-4.930571429
#define CAL_G_PER_ADC_2_NEG			-19.8717*/
#define CAL_G_PER_ADC_0_POS			1.00
#define CAL_G_PER_ADC_0_NEG			1.00
#define CAL_G_PER_ADC_1_POS			1.00
#define CAL_G_PER_ADC_1_NEG			1.00
#define CAL_G_PER_ADC_2_POS			1.00
#define CAL_G_PER_ADC_2_NEG			1.00


// Filtering
#define FILT_EXP_ALPHA_FSR			0.3