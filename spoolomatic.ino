// SPOOL-O-MATIC
//
// USING A STM32F103
//
// setup: defining the wished diameter: 1,75, 2,85, 3,00 .....
// and calibrating the led luminosity to display diametre (actually the calibration is totally independant of the STM)
// add Routine parkposition for trolley 

// MODULE DIAMETER:
// Use encoder to dial in the wished diameter and put it into a variable,
// push to confirm and pass to density, dial in the density of the plastic used and put it into a variable.
// ----------------------------------------------------------------
// 1st part: measuring luminosity with ldr and translate it into understandable values (map-function)
// Actually NO map-function: we will have one value on an analog input (0-1023) and we will use this value
// to compare with the value hardcoded for each diameter
// creating variable to display value


// 2nd part: reading the diameter value and tune the pull-motor with this value, once the switch is switched from "manual" to "auto"
// 
// MODULE SPEED CONTROL:
// We use a stepper motor to transport the filament, the speed is controlled by the value of an analog input
// which is either the readout from the ldr (auto) or the potentiometer of (manual) control. 
// We need four digital outs for the motor , two analog ins for the speed control and one digital in for 
// the choice of auto/manual.
// ----------------------------------------------------------------------
//
// 3rd part: turning the trolley motor in accordance to wished diameter
//We change the switch state to "start" to begin the counter, the spooling and the trolley-movement
//

// MODULE TROLLEY:
// A stepper motor moves the trolley forth and back between two limit-switches, the speed is controlled by the result of
// the speed-wheel (wheel with 8 holes on the axis and an optocoupleur) and the variable of the defined diameter.
// 
// Spool-Routine:
// M8 has a pitch of 1.25mm. For a diametre of 2.85 and a speedwheel with 8 holes that means
// 2.85 / 1.25 = 2.28 / 8 = 0;285 so every time a hole is detected, turn the axe 0.285 turns.
// which means 0,285÷0,015625(turns per step)= 18.24, so make 18 steps every time a hole is detected. 
// 
