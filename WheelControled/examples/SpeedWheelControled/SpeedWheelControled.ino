
#include <Hbridge.h>
#include <Encoder.h> 
#include <WheelEncoder.h>
#include <PID_v1.h>

// Sensor and actuator
Hbridge hbridge; // input 1 in pin 4 , input 2 in pin 5 , enable in pin 2
WheelEncoder wheelEncoder(19,18); // pin 18 and 19 used


//Define Variables we'll be connecting to
double desiredSpeed, measuredSpeed, outputCommand;

//Specify the links and initial tuning parameters
PID myPID(&measuredSpeed, &outputCommand, &desiredSpeed, 20 , 10 , 0 , DIRECT);

void setup()
{
  // Serial for debugging
  Serial.begin(9600);
  
  //initialize the H Bridge on default value
  hbridge.setup();
  hbridge.setCmd(1);
 
  //intialize the wheel encoder on default value
 
  wheelEncoder.setup(360 , 2.86 * 0.01 ); // encoder ticks per revolution & radius of the wheel
  wheelEncoder.setUnit(CENTIMETER);
  wheelEncoder.setSampleTime(20); // sample time of encoder in ms => give the precision of velocity
  
  
  //initialize the variables we're linked to
  desiredSpeed = 100 ; // in wheel encoder unit 

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(30); // in ms
  myPID.SetOutputLimits(-1,1); // correspond to the limite of pwm rating
}

void loop()
{
  // Input
  wheelEncoder.process();      // compute speed
  wheelEncoder.displaySpeedForMatlab(); // display unit in unit chosen
  measuredSpeed = wheelEncoder.getTranslationSpeed();
 
  // Command
  myPID.Compute(); 
  hbridge.setCmd(outputCommand);
 }


