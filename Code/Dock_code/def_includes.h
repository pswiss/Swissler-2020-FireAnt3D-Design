/*////////////////////////////////////////////////////
 Name:		def_includes.h
 Created:	3/25/2019
 Author:	Petras Swissler

 Purpose:	Include all necessary files
*/////////////////////////////////////////////////////
#pragma once
// Structs, Classes, and Constants
#include "def_friendly_names.h"
#include "def_pins.h"
#include "def_friendly_names.h"

// Basic Libraries
#include <stdint.h>
#include <Wire.h>

// Borrowed Libraries

// Helper Functions
#include "simple_helper_functions.h"
#include "EEPROM_functions.h"

// Hardware Functions
#include "startup_functions.h"
#include "hw_h_bridges.h"

// Output Functions
#include "outputs.h"

// Input Functions
#include "inputs.h"

// Behavior Functions
#include "communications.h"
#include "logic.h"