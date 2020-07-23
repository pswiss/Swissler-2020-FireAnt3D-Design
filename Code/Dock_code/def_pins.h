/*////////////////////////////////////////////////////
 Name:		def_pins.h
 Created:	3/25/2019
 Author:	Petras Swissler

 Purpose:	Define friendly names for pins
*/////////////////////////////////////////////////////
#pragma once

// Analog Inputs
#define PIN_ASEN_VBAT_MINUS				0
// A1 is no longer used (available)
#define PIN_ASEN_IDOCK_INOUT_LOW		2
#define PIN_ASEN_IDOCK_OUT_HIGH			3
// A4 is taken by SDA
// A5 is taken by SCL
#define PIN_ASEN_VDOCK					6
#define PIN_ASEN_IHOOP_IN_LOW			7

// Digital Inputs / Outputs
// D0 is taken by RX 
// D1 is taken by TX
#define PIN_DO_ENABLE_SEN_VBAT_MINUS	2

#define PIN_PWM_ARM_MOTOR_B				3
#define PIN_PWM_ARM_MOTOR_A				5

#define PIN_DI_ARMLIMIT					4
#define PIN_DI_DOCKLEVEL				7
#define PIN_DI_HOOPLEVEL				8

#define PIN_PWM_HOOP_MOTOR_B			9
#define PIN_PWM_HOOP_MOTOR_A			11

#define PIN_PWM_DOCK_MELT_DOCK			6
#define PIN_PWM_DOCK_MELT_HOOP			10

#define PIN_DO_DEBUG_LED				12
// D13 (SCK) not used (available)