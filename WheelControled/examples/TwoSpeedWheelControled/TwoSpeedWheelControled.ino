//CAREFUL
// Digital pin 13 used by Encoder function
// PWM 11 12 13 cannot be used as PWM because TimerOne is Used


#include <Hbridge.h>
#include <Encoder.h>  // used in WheelEncoder class
#include <WheelEncoder.h>
#include <PID_v1.h>


// Sensor and actuator
Hbridge hbridgeRight(4,5,2); // input 1 and 2 = pin 4 and 5 & enable is the pin 2 
Hbridge hbridgeLeft (6,7,3); // input 1 and 2 = pin 6 and 7 & enable is the pin 3 

WheelEncoder wheelEncoderRight(19,18); // pin 18 and 19 to use high performance (interrupt mode) DO NOT USE PIN 13  
WheelEncoder wheelEncoderLeft(21,20); // pin 21 and 20 to use high performance (interrupt mode)


//Define Variables we'll be connecting to
double desiredSpeedRight , desiredSpeedLeft ;
double measuredSpeedRight , measuredSpeedLeft;
double outputCommandRight , outputCommandLeft;

//Specify the links and initial tuning parameters
PID myPIDRight(&measuredSpeedRight, &outputCommandRight, &desiredSpeedRight, 0.5952 , 0 , 0 , DIRECT);
PID myPIDLeft (&measuredSpeedLeft , &outputCommandLeft , &desiredSpeedLeft, 0.5952 , 0 , 0 , DIRECT);

void setup()
{
  // Serial for debugging
  Serial.begin(9600);
  
  
  //initialize the H Bridges on default value
  hbridgeRight.setup();
  hbridgeLeft.setup();
   
  //intialize the wheel encoder on default value
 
  wheelEncoderRight.setup(360 , 2.86 * 0.01 ); // encoder ticks per revolution & radius of the wheel
  wheelEncoderRight.setUnit(CENTIMETER);
  wheelEncoderRight.setSampleTime(50); // sample time of encoder in ms => give the precision of velocity
  
  wheelEncoderLeft.setup(360 , 2.86 * 0.01 ); // encoder ticks per revolution & radius of the wheel
  wheelEncoderLeft.setUnit(CENTIMETER);
  wheelEncoderLeft.setSampleTime(50); // sample time of encoder in ms => give the precision of velocity
  
  
  // Setpoints in wheel encoder unit (init to 0)

  
  //turn the PIDs on
  myPIDRight.SetMode(AUTOMATIC);
  myPIDRight.SetSampleTime(50); // in ms
  myPIDRight.SetOutputLimits(-1,1); // correspond to the limite of pwm rating
  
  myPIDLeft.SetMode(AUTOMATIC);
  myPIDLeft.SetSampleTime(50); // in ms
  myPIDLeft.SetOutputLimits(-1,1); // correspond to the limite of pwm rating
}

void loop()
{
  desiredSpeedRight = 100 ; 
  desiredSpeedLeft  = 100 ;
  
  speedProcessing();
  
 }
 
 
 void speedProcessing(){
 
   
     // Inputs
  wheelEncoderRight.process();      // compute speed
  wheelEncoderLeft .process();
  
  wheelEncoderRight.displaySpeedForMatlab(); // display unit in unit chosen (be carefull , display only one of them for computation
  
  measuredSpeedRight = wheelEncoderRight.getTranslationSpeed();
  measuredSpeedLeft  = wheelEncoderLeft .getTranslationSpeed();
  
  // Commands computation
  myPIDRight.Compute(); 
  myPIDLeft .Compute(); 
  
  // 
  hbridgeRight.setCmd(outputCommandRight);
  hbridgeLeft .setCmd(outputCommandLeft);
     
 }


