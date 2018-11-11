// SPOOL-O-MATIC
//
// USING A Arduino Uno and MotorShield V2
//
// setup: defining the wished diameter: 1,75, 2,85, 3,00 .....
// and calibrating the led luminosity to display diametre (actually the calibration is totally independant of the STM)
// add Routine parkposition for trolley NO this will be done manually
//

#include <Wire.h>
#include <SPI.h>
#include <TFT.h>
#include <Encoder.h>
#include <Adafruit_MotorShield.h>

// Pindefinitions
// Pins for TFT Screen D11=MOSI D13=SCK
#define cs   10
#define dc   9
#define rst  8

// Creating Objects
// Create the motor shield object with the default I2C address
// I2C Pins are A4, A5
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_StepperMotor *pullMotor = AFMS.getStepper(64, 1);
Adafruit_StepperMotor *slideMotor = AFMS.getStepper(64, 2);

Encoder myEnc(2, 3);

TFT TFTscreen = TFT(cs, dc, rst);

// Declaring variables

int encSwitch = 4;
int leftLimit = 6;
int rightLimit = 7;
int speedWheel = A2;
int aInValue = A3;
int spool = 12;

int slideSpeed;
float diameterArray[4] = {0,  1.75, 2.85, 3.00};
int ldrValueArray[4] = {0, 102, 164, 198};   // Values need to be established
int stepsPerHoleArray[4] = {0, 20, 30, 40};    // Values need to be established
float density = 1;
int spoolSpeed;
//---------------------------------------------------------------
// Variables to toggle the direction of the slideMotor fw and bw

int state = LOW;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers
//--------------------------------------------------------------


  
void setup() {
  //start serial connection
Serial.begin(9600);
  TFTscreen.begin();
  TFTscreen.setRotation(3);
  TFTscreen.background(0, 0, 0);

  Serial.begin(9600);

  // Pin Modes
  pinMode(encSwitch, INPUT_PULLUP);  //Encoder Switch
  pinMode(limitSwitch, INPUT_PULLUP);  // 2 Limit Switches  parallel on Input7
  pinMode(speedWheel, INPUT_PULLUP); //Optocoupler speedwheel
  pinMode(spool, INPUT_PULLUP); //Start spooling

  pullMotor->setSpeed(10);

}

    void loop() {
  
// MODULE DIAMETER:
// Use encoder to dial in the wished diameter and put it into a variable,
// push to confirm and pass to density, dial in the density of the plastic used and put it into a variable.
// We need three digital inputs.
// ----------------------------------------------------------------


  // Choose diameter
  // Preparation of the encoder

  int oldPosition  = 1;
  int diameterIndex = 1;

  int newPosition = myEnc.read();
  if (newPosition > oldPosition) {
    oldPosition = oldPosition + 1;
    oldPosition = newPosition;
  }
  if (newPosition > 3) {
    newPosition = 3;
  }

  if (newPosition < oldPosition) {
    oldPosition = oldPosition - 1;
    oldPosition = newPosition;
  }
  if (newPosition < 1) {
    newPosition = 1;
  }
//
// 1st part: measuring luminosity with ldr and translate it into understandable values (map-function)
// Actually NO map-function: we will have one value on an analog input (0-1023) and we will use this value
// to compare with the value hardcoded for each diameter
// creating variable to display value
      
      


//
//2nd part: turning the trolley motor in accordance to wished diameter
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
 //*****************************************************************************//

  diameterIndex = newPosition;
  float stepsPerHole = stepsPerHoleArray[diameterIndex];
      
        if (digitalRead(spool) == LOW) {
    // pullSpeed = map(pullSpeed, 0, 1023, 0, 100);  // determines pullSpeed between 0 and 100 rpm's



    //------------------------------------------------------------
// Reading the switchstates for slidedirection

    reading = digitalRead(limitSwitch);
    
    if (reading == HIGH && previous == LOW && millis() - time > debounce) {
      if (state == HIGH)
        state = LOW;
      else
        state = HIGH;

      time = millis();
    }

    previous = reading;


  }
  //-------------------------------------------------------------

    int holeDetect = digitalRead (speedWheel);
      if (holeDetect == LOW && state == 0 ) {
        slideMotor->step(stepsPerHole, FORWARD, SINGLE);
      }
          if (holeDetect == LOW && state == 1 ) {
        slideMotor->step(stepsPerHole, BACKWARD, SINGLE);
      }


//3rd part: reading the diameter value and tune the pull-motor with this value, once the switch is switched from "manual" to "auto"
// 
// MODULE SPEED CONTROL:
// We use a stepper motor to transport the filament, the speed is controlled by the value of an analog input
// which is either the readout from the ldr (auto) or the potentiometer of (manual) control. 
  //*****************************************************************************//
  // Retreive the LDR Value or Pot-Value on A3

  int pullSpeed = analogRead(aInValue);
  int refValue = ldrValueArray[diameterIndex];    // reading from defined array

  if (pullSpeed > refValue) {
    pullSpeed = pullSpeed -1;
    }

 if (pullSpeed < refValue) {
    pullSpeed = pullSpeed +1;
    }

    } // Void End
