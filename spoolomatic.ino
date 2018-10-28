// SPOOL-O-MATIC
// setup: defining the wished diameter: 1,75, 2,85, 3,00 .....
// and calibrating the led luminosity to display diametre (actually the calibration is totally independant of the arduino)
// add Routine parkposition for trolley 


// 1st part: measuring luminosity with ldr and translate it into understandable values (map-function)
// Actually NO map-function: we will have one value on an analog input (0-1023) and we will use this value
//to compare with the value hardcoded for each diameter
// creating variable to display value


// 2nd part: reading the diameter value and tune the pull-motor with this value, once the switch is switched from "manual" to "auto"


// 3rd part: turning the trolley motor in accordance to wished diameter
//We change the switch state to "start" to begin the counter, the spooling and the trolley-movement
//
// Spool-Routine:
// M8 has a pitch of 1.25mm. For a diametre of 2.85 and a speedwheel with 8 holes that means
// 2.85 / 1.25 = 2.28 / 8 = 0;285 so every time a hole is detected, turn the axe 0.285 turns.
// which means 0,285÷0,015625(turns per step)= 18.24, so make 18 steps every time a hole is detected. 
// 
// We need a startbutton to start movement of the trolley, once the diameter is established
