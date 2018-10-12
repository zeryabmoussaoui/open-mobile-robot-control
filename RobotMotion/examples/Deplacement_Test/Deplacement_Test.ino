//CAREFUL
// Digital pin 13 used by Encoder function
// PWM 11 12 13 cannot be used as PWM because TimerOne is Used

#include <Hbridge.h>
#include <Encoder.h>  // used in WheelEncoder class
#include <WheelEncoder.h>
#include <PID_v1.h>
#include <WheelControled.h>
#include <RobotMotion.h>


// Sensor and actuator
Hbridge hbridgeRight(4,5,9); // input 1 and 2 = pin 4 and 5 & enable is the pin 2 
Hbridge hbridgeLeft (6,7,10); // input 1 and 2 = pin 6 and 7 & enable is the pin 3 

WheelEncoder wheelEncoderRight(19,18); // pin 18 and 19 to use high performance (interrupt mode) DO NOT USE PIN 13  
WheelEncoder wheelEncoderLeft(21,20); // pin 21 and 20 to use high performance (interrupt mode)

WheelControled wheelControledRight (&hbridgeRight , &wheelEncoderRight);
WheelControled wheelControledLeft  (&hbridgeLeft  , &wheelEncoderLeft);

RobotMotion robotMotion( &wheelControledRight , &wheelControledLeft);

 //Condition to stop moving loop
 float ERROR_POSITION_THRESHOLD = 1 ; 
 float ERROR_ANGLE_THRESHOLD = 3 ;
 float SPEED_STOP_THRESHOLD = 100000 ; //to make the speed condition on stop condition not applicable


void setup()
{
 
  
 robotMotion.setup(10 , 2.86*0.01 , 0.01 , 41 , 360 , CENTIMETER ); // wheelDistance , radius , deadband , sampleTime , encoderResolution , unit 
 
 robotMotion.setRegulatorParams(0.5952 , 0.001 , 0.0 , RIGHT_WHEEL , SPEED_CONTROL , 1);
 robotMotion.setRegulatorParams(0.5952 , 0.001 , 0.0 , LEFT_WHEEL , SPEED_CONTROL  , 1 );
 
 robotMotion.setControlerType(CONTROLER_TYPE1);
 robotMotion.setRegulatorParams(5 , 1 , 0 , LEFT_WHEEL  , POSITION_CONTROL , 80);
 robotMotion.setRegulatorParams(5 , 1 , 0 , RIGHT_WHEEL , POSITION_CONTROL , 80 );
 
 robotMotion.setStopConditionParameter(ERROR_POSITION_THRESHOLD , ERROR_ANGLE_THRESHOLD , SPEED_STOP_THRESHOLD);
 
 //Debug display
 robotMotion.isDebugOn = true ;
 robotMotion.debugWheel = RIGHT_WHEEL;
 robotMotion.debugType = POSITION_CONTROL;
 
 robotMotion.init();
 
}

void loop()
{
  


  delay(1000);
  //robotMotion.turn(360);
  robotMotion.translation(200);
  robotMotion.translation(-10);
  delay(10000);


}
