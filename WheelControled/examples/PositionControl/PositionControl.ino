//CAREFUL
// Digital pin 13 used by Encoder function
// PWM 11 12 13 cannot be used as PWM because TimerOne is Used

#include <Hbridge.h>
#include <Encoder.h>  // used in WheelEncoder class
#include <WheelEncoder.h>
#include <PID_v1.h>
#include <WheelControled.h>

WheelControled wheelControled;

double demandedPosition;
double currentPosition;
double speedCommand;

PID myPID = PID(&currentPosition , &speedCommand , &demandedPosition , 5 , 1 , 0 , DIRECT);

void setup()
{
 // Serial for debugging
 Serial.begin(9600);
 myPID.SetSampleTime(DEFAULT_SAMPLE_TIME); // see configuration file
 myPID.SetMode(AUTOMATIC);
 myPID.SetOutputLimits(-100,100);
 
 demandedPosition = 0;
 currentPosition = 0;
 speedCommand = 0;
 

 demandedPosition = 25 ;
 
}

void loop()
{

  currentPosition = wheelControled.getPosition();
  myPID.Compute();
  wheelControled.setSpeed(speedCommand);
  wheelControled.process(); 
  
 }
 

