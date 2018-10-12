#ifndef RobotMotion_H
#define RobotMotion_H

#include "Hbridge.h"
#include "WheelEncoder.h"
#include "WheelControled.h"
#include "PID_v1.h"
#include "Odometer.h"

#define LEFT_WHEEL 1
#define RIGHT_WHEEL 2
#define ANY_WHEEL 3

#define SPEED_CONTROL 1
#define POSITION_CONTROL 2
#define TRANSLATION_CONTROL 3
#define ROTATION_CONTROL 4

#define CONTROLER_TYPE1 1
#define CONTROLER_TYPE2 2



class RobotMotion{

public: // TODO : re order method

RobotMotion(WheelControled* _wheelRight , WheelControled* _wheelLeft); //TODO : move to private to implement

void setup(double _wheelDistance , float _radius ,  double deadBandValue, 
				int _sampleTime , int _encoderRevolution , float _unit ); // TODO : deprecated to be deleted in V6

void setup(double _wheelDistance , float _radius , int _sampleTime , int _encoderRevolution , 
			float _unit  , double _deadBandTranslation , double _deadBandRotation);


// wheel  = 1,2 for left or right wheel : kind = 1,2 for speed/position/polar controler

void setRegulatorParams(double _kp , double _ki , double _kd , int wheel , int kind , float limit); // TODO : deprecated to be deleted in V6

void setRegulatorParams(double _kp , double _ki , double _kd , int wheel , 
						int kind , float limit , float windUpLimit ); // method to set the windup limit

void setControlerType(int type); // type = 1,2 for TYPE1 , TYPE2 Position Controler

void setStopConditionParameter(float _positionErrorLimit , float _angleErrorLimit , float _speedStopLimit);

void init(); // add for exemple Serial.begin


void translation(float distance);

void turn(float angle);

void stop();

bool isDebugOn;
int debugWheel; // wheel = 1,2 for left,right
int debugType ; //type 1,2 = speed , position
bool stopConditionUsed  ; // to use stop condition , if false process shall be called in a loop() function 

double getCurrentDistance();
double getCurrentAngle();

double getRemainedDistance();

Odometer* odometer;


//void setAccelerationMax(float acc); // TODO : acceleration max to add


private:

WheelControled* wheelRight;
WheelControled* wheelLeft;

int controlerType; // type = 1,2 for TYPE1 , TYPE2 

// Position Controled loop 

/*Measures*/
double currentRightPosition ; 
double currentLeftPosition ; 	

/*Setpoints*/
double demandedRightPosition ; 
double demandedLeftPosition; 

/*Outputs*/
double speedRightCommand;
double speedLeftCommand;

// Position Controled loop 

double currentDistance; //  measure for translation control
double currentAngle; 	//  measure for angle control

double demandedDistance; // setpoint for translation	
double demandedAngle;	 // setoint for angle

double speedCommand; // output of Position Controler 



// Constants

/* Physical constants*/
double wheelDistance ; // default value for the robot in robot unit ( meter or centimeter)

/* Stop condition constants*/
float positionErrorLimit ; // default value for threshold on position error in robot unit (meter or centimeter)
float angleErrorLimit ; // in degree
float speedStopLimit ; // taken high to not be taken into account

double deadBand; // TODO  : to be deleted in next version
double deadBandTranslation ;
double deadBandRotation;

bool isNotStopDistance(); // TODO : replace in next version by deadband 
bool isNotStopAngle();

// Position regulator (use one of them)

// Standard PID ( Not yet used)
PID * positionPIDRight ;
PID * positionPIDLeft ; //enable to do one closed loop for each wheel

// Polar PID 
PID * translationPID ;
PID * rotationPID ;
PID * polarPID;

float toDistance(float angle);
float toAngle(float distance);

void callDebug();

void initTranslation(float distance);
void initTurn(float angle);

};



#endif