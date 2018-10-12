//CAREFUL
// Digital pin 13 used by Encoder function
// PWM 11 12 13 cannot be used as PWM because TimerOne is Used

#include <Hbridge.h>
#include <Encoder.h>  // used in WheelEncoder class
#include <WheelEncoder.h>
#include <PID_v1.h>
#include <WheelControled.h>
#include <Odometer.h>
#include <RobotMotion.h>
#include <Robot.h>


// Sensor and actuator
Hbridge hbridgeRight(32,30,5); // input 1 and 2 = pin 4 and 5 & enable is the pin 2 
Hbridge hbridgeLeft (36,34,4); // input 1 and 2 = pin 6 and 7 & enable is the pin 3 

WheelEncoder wheelEncoderRight(18,19); // pin 18 and 19 to use high performance (interrupt mode) DO NOT USE PIN 13  
WheelEncoder wheelEncoderLeft(20,21); // pin 21 and 20 to use high performance (interrupt mode)

WheelControled wheelControledRight (&hbridgeRight , &wheelEncoderRight);
WheelControled wheelControledLeft  (&hbridgeLeft  , &wheelEncoderLeft);

RobotMotion robotMotion( &wheelControledRight , &wheelControledLeft);

Robot robot(&robotMotion);

 //Condition to stop moving loop
 float ERROR_POSITION_THRESHOLD = 0.5 ; 
 float ERROR_ANGLE_THRESHOLD = 0.5 ;
 float SPEED_STOP_THRESHOLD = 100000 ; //to make the speed condition on stop condition not applicable

// Send Command to Actuator Board


#define FILL_POPCORN  10
#define EMPTY_POPCORN 11
#define TAKE_CUP      12 
#define PUT_CUP       13 
#define VALVE_ON  14
#define VALVE_OFF 15
#define TAKE_GOBLET 16
#define PUT_GOBLET 17
int sharpPin = 2 ;

void setup()
{
  // Serial for communication with Actuators Board
// attachInterrupt(0, avoid , HIGH);

  Serial2.begin(9600);
  Serial.begin(9600);
  
 robotMotion.setup(22 , 3.1*0.01 , 0.01 , 5 , 2048 , CENTIMETER ); // wheelDistance , radius , deadband , sampleTime , encoderResolution , unit 
 
 robotMotion.setRegulatorParams(0.0085 , 0.024 , 0 ,RIGHT_WHEEL , SPEED_CONTROL , 1);
 robotMotion.setRegulatorParams(0.0085 , 0.024 , 0 , LEFT_WHEEL , SPEED_CONTROL  , 1);
 
 robotMotion.setControlerType(CONTROLER_TYPE1);
 robotMotion.setRegulatorParams(2 , 0 , 0.05 , ANY_WHEEL  , TRANSLATION_CONTROL , 30);
 robotMotion.setRegulatorParams(1.5 , 0.5 , 0.05 , ANY_WHEEL , ROTATION_CONTROL , 30 );
 
 robotMotion.setStopConditionParameter(ERROR_POSITION_THRESHOLD , ERROR_ANGLE_THRESHOLD , SPEED_STOP_THRESHOLD);
 
 //Debug display
 robotMotion.isDebugOn = false ;
 robotMotion.debugWheel = LEFT_WHEEL;
 
 robotMotion.debugType = POSITION_CONTROL;
 
 
}

void loop()
{
 
 robot.turnBy(90);
 delay(1000);
 robot.turnTo(0);
 delay(1000); 
 while(1);
}

int action(byte actionCmd)

{
   
  Serial2.write(actionCmd);
  
  while(Serial2.available()<=0);
  
  return Serial2.read();
    
 
 }
 void avoid()
{ 
  //Serial.println("Detection!");
  int cpt;
  while(cpt++<1000);
  if(digitalRead(sharpPin)==HIGH)
  {
    //Serial.println("Debut Evitement!");
    robotMotion.stop();
    while(cpt++<30000);
    //Serial.println("Fin Evitement!");
  }
  }
