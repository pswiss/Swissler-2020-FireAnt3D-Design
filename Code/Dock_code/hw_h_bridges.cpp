/*////////////////////////////////////////////////////
 Name:		hw_h_bridges.cpp
 Created:	3/25/2019
 Author:	Petras Swissler

 Purpose:	Create functions to control h-bridges
*/////////////////////////////////////////////////////

#include "hw_h_bridges.h"

// H-Bridge used to control the melting
void hw_hb_drv8871ddar(int direction, float power)
{
	int applyPWM = (int)(ATMEGA328_MAX_PWM * power);
	switch (direction)
	{
	/*case HBMELT_HI_V: // This is the hi-gnd pwm signal
		analogWrite(PIN_PWM_DOCK_MELT_HOOP, applyPWM);
		digitalWrite(PIN_PWM_DOCK_MELT_DOCK, HIGH);
		break;*/
	case HBMELT_HI_V:	// This is the hi-hiz pwm signal
		digitalWrite(PIN_PWM_DOCK_MELT_HOOP, LOW);
		if (power > 0.95)
		{	digitalWrite(PIN_PWM_DOCK_MELT_DOCK, HIGH);		}
		else
		{	analogWrite(PIN_PWM_DOCK_MELT_DOCK, applyPWM);	}

		break;
	case HBMELT_GND:
		digitalWrite(PIN_PWM_DOCK_MELT_DOCK, HIGH);
		digitalWrite(PIN_PWM_DOCK_MELT_HOOP, HIGH);
		break;
	case HBMELT_HI_Z:
		digitalWrite(PIN_PWM_DOCK_MELT_DOCK, LOW);
		digitalWrite(PIN_PWM_DOCK_MELT_HOOP, LOW);
		break;
	default:
		// default to high-z state
		digitalWrite(PIN_PWM_DOCK_MELT_DOCK, LOW);
		digitalWrite(PIN_PWM_DOCK_MELT_HOOP, LOW);
		break;
	}
}

// H-Bridge used to control the hoop and arm motors
void hw_hb_lv8548mc(int direction, float power, int pin_in_A, int pin_in_B)
{
	int applyPWM = power * ATMEGA328_MAX_PWM;

	switch (direction) 
	{
	case MOTORDIR_FWD:
		analogWrite(pin_in_A, applyPWM);
		digitalWrite(pin_in_B, LOW);
		break;
	case MOTORDIR_REV:
		digitalWrite(pin_in_A, LOW);
		analogWrite(pin_in_B, applyPWM);
		break;
	case MOTORDIR_IDLE:
		digitalWrite(pin_in_A, LOW);
		digitalWrite(pin_in_B, LOW);
		break;
	case MOTORDIR_BRAKE:
		digitalWrite(pin_in_A, HIGH);
		digitalWrite(pin_in_B, HIGH);
		break;
	}
}