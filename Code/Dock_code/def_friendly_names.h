/*////////////////////////////////////////////////////
 Name:		def_pins.h
 Created:	3/25/2019
 Author:	Petras Swissler

 Purpose:	Define friendly names various constants
*/////////////////////////////////////////////////////
#pragma once
// Motor Control constants 
// FWD/REV must be either +1 or -1 to allow other logic to function
#define MOTORDIR_REV			(int)(-1)
// FWD/REV must be either +1 or -1 to allow other logic to function
#define MOTORDIR_FWD			(int)(1)
#define MOTORDIR_IDLE			(int)(-2)
#define MOTORDIR_BRAKE			(int)(-3)
#define MOTORDIR_NONE			(int)(5)

#define MOTOR_POWER_FULL		1.0
#define MOTOR_POWER_OFF			0.0

#define ARMDIR_REV				MOTORDIR_REV	
#define ARMDIR_FWD				MOTORDIR_FWD	
#define ARMDIR_IDLE				MOTORDIR_IDLE	
#define ARMDIR_BRAKE			MOTORDIR_BRAKE	

#define HOOPDIR_REV				MOTORDIR_REV	
#define HOOPDIR_FWD				MOTORDIR_FWD	
#define HOOPDIR_IDLE			MOTORDIR_IDLE	
#define HOOPDIR_BRAKE			MOTORDIR_BRAKE	


#define MOVETYPE_NORMAL			0
#define MOVETYPE_TWITCH			1
#define MOVETYPE_PULSED			2

#define HBMELT_HI_V				MOTORDIR_FWD			
#define HBMELT_GND				MOTORDIR_BRAKE
#define HBMELT_HI_Z				MOTORDIR_IDLE

// Configuration Constants
#define READS_FOR_CALIBRATION	10
#define READS_FOR_SENSING_DOH	10
#define READS_FOR_SENSING_DIO	15
#define READS_FOR_SENSING_HIL	15
#define READS_FOR_SENSING		3

// Dock-Dock Comm constants
#define COM_DD_PULSE_SHORT		5
#define COM_DD_PULSE_LONG		10
#define COM_DD_PULSE_MSGSTART	20
#define COM_DD_MSG_LENGTH		32

// Communication Speeds
#define STD_SERIAL_SPEED_0		  9600
#define STD_SERIAL_SPEED_1		 19200
#define STD_SERIAL_SPEED_2		 38400
#define STD_SERIAL_SPEED_3		 57600
#define STD_SERIAL_SPEED_4		115200

#define STD_I2C_SPEED_0			 10000
#define STD_I2C_SPEED_1			 25000
#define STD_I2C_SPEED_2			 50000
#define STD_I2C_SPEED_3			100000


#define SERIAL_BAUD_RATE		STD_SERIAL_SPEED_2
#define I2C_CLOCK				STD_I2C_SPEED_1

// I2C Constants (7 bit addresses)
#define DOCK_0_I2C_ADDR			96
#define DOCK_1_I2C_ADDR			97
#define DOCK_2_I2C_ADDR			98

#define IMU_I2C_ADDR			0x6A

#define DOCK_I2C_ADDR_UNKNOWN	0x69

#define COM_I2C_CENTER_OUT_LENGTH	16
#define COM_I2C_DOCK_OUT_LENGTH		16
#define COM_I2C_MAX_MSG_LENGTH		COM_I2C_CENTER_OUT_LENGTH

// Friendly Dock Names
#define DOCK_0					DOCK_0_I2C_ADDR
#define DOCK_1					DOCK_1_I2C_ADDR
#define DOCK_2					DOCK_2_I2C_ADDR

// Unit Conversions
#define MS_PER_S				1000.0
#define S_PER_MS				0.001

// Pulse Periods (ms)
#define PERIOD_XTRA_PULSE		2200
#define PERIOD_LONG_PULSE		500
#define PERIOD_PULSE			250
#define PERIOD_SHORT_PULSE		100

// State Phases
#define PHASE_INIT				0
#define PHASE_COMPLETE			255

#define PHASE_ATT_SWP			1
#define PHASE_ATT_MLT			2
#define PHASE_ATT_COL			3
#define PHASE_ATT_HRN			4

#define PHASE_DT_DT1			1
#define PHASE_DT_DT2			2
#define PHASE_DT_BUF			3
#define PHASE_DT_RHP			4
#define PHASE_DT_HF				11

#define PHASE_FLP_MAIN			1
#define PHASE_PUP_MAIN			1

#define PHASE_PRP_ARM			1
#define PHASE_PRP_HOOP			2

// Centerbody States
#define CENTER_STATE_INITIALIZE	0
#define CENTER_STATE_DECIDE		10
#define CENTER_STATE_STEP		20
#define CENTER_STATE_IDLE		30

// Safety States xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#define SAFETY_ARM_OK			0
#define SAFETY_ARM_BACKOFF		1

// TestRig States xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#define TESTRIG_STATE_INIT		0
#define TESTRIG_STATE_TOUCH		10
#define TESTRIG_STATE_ATTACH	20
#define TESTRIG_STATE_COOL		30
#define TESTRIG_STATE_PULL		40
#define TESTRIG_STATE_HOLD		50
#define TESTRIG_STATE_PREPARE	60
#define TESTRIG_STATE_DETACH	70
#define TESTRIG_STATE_RETRACT	80
#define TESTRIG_STATE_PIVOT		85
#define TESTRIG_STATE_COOL2		90
#define TESTRIG_STATE_COMPLETE	100

// Step Phases 
#define STEP_TOP_UP				1
#define STEP_BOTTOM_UP			-1