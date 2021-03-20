//CAREFUL
// Digital pin 13 used by Encoder function
// PWM 11 12 13 cannot be used as PWM because TimerOne is Used

#include <Hbridge.h>
#include <Encoder.h>  // used in WheelEncoder class
#include <WheelEncoder.h>
#include <PID_v1.h>
#include <WheelControled.h>

Hbridge hbridge(4,5,9); // input 1 and 2 = pin 4 and 5 & enable is the pin 2 
WheelEncoder wheelEncoder(19,18); // pin 18 and 19 to use high performance (interrupt mode) DO NOT USE PIN 13  

WheelControled speedLoop(&hbridge, &wheelEncoder) ;

double demandedPosition = 0;
double currentPosition = 0 ;
double speedCommand = 0;

PID positionLoop = PID(&currentPosition , &speedCommand , &demandedPosition , 5 , 1 , 0 , DIRECT); // Position

int sampleTime = 40; // ms

void setup()
{
 // Serial for debugging
 Serial.begin(9600);

 speedLoop.setup(360, 2.86 * 0.01 , CENTIMETER, sampleTime , 1) ;// encoder ticks per revolution - radius of the wheel (m) - sampleTime(ms) - pwm Limit (%)
 speedLoop.setTuningParameter(0.5952 , 0.001 , 0.0) ; // set Kp , KI and KD of speed regulation

 positionLoop.SetSampleTime(sampleTime);
 positionLoop.SetMode(AUTOMATIC);
 positionLoop.SetOutputLimits(-100,100); // max speed command in unit/s
 
 demandedPosition = 25 ; // in radius unit
 
}

void loop()
{

  currentPosition = speedLoop.getPosition();
  positionLoop.Compute();
  speedLoop.setSpeed(speedCommand);
  speedLoop.process(); 
  
 }
 
