//CAREFUL
// Digital pin 13 used by Encoder function
// PWM 11 12 13 cannot be used as PWM because TimerOne is Used

#include <Hbridge.h>
#include <Encoder.h>  // used in WheelEncoder class
#include <WheelEncoder.h>
#include <PID_v1.h>
#include <WheelControled.h>


// Sensor and actuator
Hbridge hbridgeRight(4,5,2); // input 1 and 2 = pin 4 and 5 & enable is the pin 2 
Hbridge hbridgeLeft (6,7,3); // input 1 and 2 = pin 6 and 7 & enable is the pin 3 

WheelEncoder wheelEncoderRight(19,18); // pin 18 and 19 to use high performance (interrupt mode) DO NOT USE PIN 13  
WheelEncoder wheelEncoderLeft(21,20); // pin 21 and 20 to use high performance (interrupt mode)

WheelControled wheelControledRight (&hbridgeRight , &wheelEncoderRight);
WheelControled wheelControledLeft  (&hbridgeLeft  , &wheelEncoderLeft);

// closed loop variable
double demandedPosition = 0;
double currentPosition = 0;
double speedCommand = 0;

double demandedAngle = 0 ;
double wheelDistance = 10; // 10 cm by default

// the same PID is applied for the two wheel 
PID myPID = PID(&currentPosition , &speedCommand , &demandedPosition , 5 , 1 , 0 , DIRECT); // set tuning parameter of PID Position

 //Condition to stop moving loop
 float ERROR_POSITION_THRESHOLD = 2 ; 
 float ERROR_ANGLE_THRESHOLD = 1 ;
 float SPEED_STOP_THRESHOLD = 100000 ; //to make the speed condition on stop condition not applicable
 int count = 0;

void setup()
{
 // Serial for debugging
 Serial.begin(9600);
 
 // Setup H bridge Dead band , below this value pwm is not applied
 hbridgeRight.isDeadBand = true ;
 hbridgeRight.setDeadBand(0.01);
 
 hbridgeLeft.isDeadBand = true ;
 hbridgeLeft.setDeadBand(0.01);
  
 // Setup the speed control on wheels 
 wheelControledRight.setup(360 , 2.86*0.01 , CENTIMETER , 41 , 1) ; // setup encoder resolution , radius of wheel  , unit , sample time in ms , max pwm rate
 wheelControledRight.setTuningParameter(0.5952 , 0.001 , 0.0) ; // set Kp , KI and KD of speed regulation
 
 wheelControledLeft.setup(360 , 2.86*0.01 , CENTIMETER , 41 , 1) ; // setup encoder resolution , radius of wheel  , unit , sample time in ms , max pwm rate
 wheelControledLeft.setTuningParameter(0.5952 , 0.001 , 0.0) ; // set Kp , KI and KD of speed regulation
  
 // Setup Position PID
 myPID.SetSampleTime(41); 
 myPID.SetMode(AUTOMATIC);
 myPID.SetOutputLimits(-80,80);
 
 
 // Setpoint
 demandedPosition = 0 ;
 

 
}

void loop()
{

  turn(-40);
  //delay(1000);
  //translation(40);
  delay(100000);
  // translation (-200); 


}


void translation(double distance)
{
  
  demandedPosition = demandedPosition + distance ; // add the translation distance
  count = 0 ; 
  
  do
  {
  
  currentPosition = (wheelControledRight.getPosition() + wheelControledLeft.getPosition()) / 2.0 ;
  
  myPID.Compute();
  
  wheelControledRight.setSpeed(speedCommand);
  wheelControledLeft .setSpeed(speedCommand);
 
  wheelControledRight.process(); 
  wheelControledLeft .process(); 
  
  wheelControledRight.displayPositionForMatlab();
  count ++ ;
  } while(isNotStop()) ;
  
  wheelControledRight.stop();
  wheelControledLeft .stop();
  
}

void turn(double demandedAngle)
{
  
  
  demandedPosition = demandedPosition + toDistance(demandedAngle)  ; 
 
  Serial.print("Demanded angle : " )    ; Serial.println(demandedAngle);
  Serial.print("Demanded position : " ) ; Serial.println(demandedPosition);
 
  count = 0 ; 
  
  do
  {
  
  currentPosition = (wheelControledRight.getPosition() - wheelControledLeft.getPosition()) / 2.0 ;
  
  myPID.Compute();
  
  wheelControledRight.setSpeed(speedCommand);
  wheelControledLeft .setSpeed(-speedCommand);
 
  wheelControledRight.process(); 
  wheelControledLeft .process(); 
  
  wheelControledRight.displayPositionForMatlab();
  count ++ ;
  } while(isNotStopAngle()) ;
  
  wheelControledRight.stop();
  wheelControledLeft .stop();
  
  Serial.println();
  Serial.print("current angle : " )    ; Serial.println(toAngle(currentPosition));
  Serial.print("current position : " ) ; Serial.println(currentPosition);
 
  
}



boolean isNotStop()
{
  
return  (count < 50 ||
           abs(demandedPosition - currentPosition) > ERROR_POSITION_THRESHOLD  
          || abs(wheelControledRight.getSpeed()) > SPEED_STOP_THRESHOLD 
          || abs(wheelControledLeft.getSpeed ()) > SPEED_STOP_THRESHOLD 
           ) ;
}

boolean isNotStopAngle()
{
  
return  (count < 50 ||
           abs(toAngle(demandedPosition) - toAngle(currentPosition)) > ERROR_ANGLE_THRESHOLD || 
           abs(wheelControledRight.getSpeed()) > SPEED_STOP_THRESHOLD ||
           abs(wheelControledLeft.getSpeed ()) > SPEED_STOP_THRESHOLD 
            );
}
float toDistance(float angle)
{
  
 return (angle*PI/180.00) * (wheelDistance / 2.0) ;
  
}

float toAngle(float distance) //degree
{
  return (distance * (2.0/wheelDistance) * (180.00/PI) );
}
 

