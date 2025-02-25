#pragma once
// Behavior Phases -----------------------------------
// Generic
#define PHASE_ACTION_BEGIN			0
#define PHASE_ACTION_COMPLETE		255
// Flip
#define PHASE_ACTION_FLP_FWD		10
#define PHASE_ACTION_FLP_REV		20
#define PHASE_ACTION_FLP_WAIT		30
#define PHASE_ACTION_FLP_RCAL		40
// Detach
#define PHASE_ACTION_DET_CONTINUE	10
#define PHASE_ACTION_DET_BUFFER		20
// Attach
#define PHASE_ACTION_ATT_CONTINUE	10
// Prepare
#define PHASE_ACTION_PREP_CONTINUE	10

// Automated Step Phases -----------------------------
#define AUTOSTEP_000_BEGIN							00
#define AUTOSTEP_010_DET_DOCK_PRESS					10
#define AUTOSTEP_020_SMALL_INITIAL_LIFT				20
#define AUTOSTEP_030_CALIB_FORCE_SENSORS_BEFORE_LIFT 30
#define AUTOSTEP_040_PULL_UPWARDS_BEFORE_DET		40
#define AUTOSTEP_050_DETACH_REAR_DOCK				50
#define AUTOSTEP_060_CONTINUE_MOVEMENT_AFTER_DETACH	60
#define AUTOSTEP_070_POST_DETACH_PREPARATION		70
#define AUTOSTEP_075_REVERSE_PREP					75
#define AUTOSTEP_077_PREP_HOOP_RETURN				77
#define AUTOSTEP_080_FLIP_FORWARD_UNTIL_CONTACT		80
#define AUTOSTEP_090_BACK_OFF_AFTER_CONTACT			90
#define AUTOSTEP_100_PRESS_INTO_SURFACE				100
#define AUTOSTEP_110_SELECT_NEXT_ATTACH_DOCK		110
#define AUTOSTEP_120_ATTACH_ON_SELECTED_DOCK		120
#define AUTOSTEP_130_FINAL_COOL						130