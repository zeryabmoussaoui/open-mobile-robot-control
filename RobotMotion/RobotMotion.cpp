#include <Arduino.h> 

#include "RobotMotion.h"


//TODO : use default value on #define 
RobotMotion::RobotMotion(WheelControled* _wheelRight , WheelControled* _wheelLeft)
{

	this->wheelRight =  _wheelRight ;
	this->wheelLeft  =  _wheelLeft ;

	this->isDebugOn = false ; 
	this->debugWheel = RIGHT_WHEEL;
	this->debugType  = SPEED_CONTROL;

	/*Initial conditions all set to 0 for Method 1 Position Control*/
	this->currentRightPosition = 0 ; 
	this->currentLeftPosition = 0 ; 
	this->speedRightCommand = 0;
	this->speedLeftCommand  = 0;	
	this->demandedRightPosition = 0; 
	this->demandedLeftPosition	= 0; 


	
	/*Initial conditions all set to 0 for Method 2 Position Control*/
	 
	this->currentAngle = 0; 
	this->demandedAngle = 0;	
	
	this->currentDistance = 0; 
	this->speedCommand = 0;
	this->demandedDistance = 0;  
 

	/* Physical constants*/
	this->wheelDistance = 20 ; // 

	/* Stop condition constants*/
	this->positionErrorLimit = 1 ;
	this->angleErrorLimit = 1; 
	this->speedStopLimit = 10000; 
	this->stopConditionUsed = true ; 
	
	this->deadBand = 0.1;
	this->deadBandTranslation = 0.1;
	this->deadBandRotation = 0.1;

	// Method 1 Polar regulator
	this->polarPID = new PID(& this->currentDistance , & this->speedCommand , & this->demandedDistance, 5, 1 , 0 , DIRECT); 
	
	this->translationPID = new PID(& this->currentDistance , & this->speedCommand , & this->demandedDistance, 5, 1 , 0 , DIRECT); 
	this->rotationPID    = new PID(& this->currentDistance , & this->speedCommand , & this->demandedDistance, 5, 1 , 0 , DIRECT); 

	// Method 2 Two closed loop
	this->positionPIDRight = new PID(& this->currentRightPosition , & this->speedRightCommand , & this->demandedRightPosition, 5 , 1 , 0 , DIRECT); 
	this->positionPIDLeft  = new PID(& this->currentLeftPosition  , & this->speedLeftCommand  , & this->demandedLeftPosition , 5 , 1 , 0 , DIRECT); 
	
	this->controlerType = CONTROLER_TYPE1;
	int _sampleTime = 41 ;
	float _radius = 5 * 0.01;
	
	this->odometer = new Odometer ( _sampleTime , this->wheelDistance , _radius * CENTIMETER);
}

void RobotMotion::init()
{
	Serial.begin(9600);
	//delay(1);
}

// deprecated setup to be deleted in V6
void RobotMotion::setup(double _wheelDistance , float _radius ,  double _deadBandValue, int _sampleTime , int _encoderRevolution , float _unit )
{
	this->wheelDistance = _wheelDistance;
	this->deadBand = _deadBandValue;
	
	if(this->deadBand == 0) // TODO : improve deadband setting by using some epsilon
	{
		this->wheelRight->setDeadBand( false , 0);
		this->wheelLeft->setDeadBand ( false , 0);
	}
	else
	{
		this->wheelRight->setDeadBand( true , this->deadBand);
		this->wheelLeft->setDeadBand( true , this->deadBand);	
	}
	
	 // Setup the speed control on wheels 
	this->wheelRight->setup( _encoderRevolution , _radius , _unit , _sampleTime , 1) ;
	this->wheelLeft->setup ( _encoderRevolution , _radius , _unit , _sampleTime , 1) ;
	
	// Setup sample time for Position Conntroler
	this->polarPID->SetSampleTime(_sampleTime);
	this->translationPID->SetSampleTime(_sampleTime);
	this->rotationPID->SetSampleTime(_sampleTime);
	
	//this->positionPIDLeft->SetSampleTime(_sampleTime);  // Not yet used
	//this->positionPIDRight->SetSampleTime(_sampleTime); // Not yet used
	
	//Setup Mode as Automatic
	
    this->polarPID->SetMode(AUTOMATIC);
	this->rotationPID->SetMode(AUTOMATIC);
	this->translationPID->SetMode(AUTOMATIC);
	
	//this->positionPIDLeft->SetMode(AUTOMATIC);  // Not yet used
	//this->positionPIDRight->SetMode(AUTOMATIC); // Not yet used
	
	this->odometer = new Odometer ( _sampleTime , this->wheelDistance , _radius * CENTIMETER);
 
}

void RobotMotion::setup(double _wheelDistance , float _radius , int _sampleTime , int _encoderRevolution , float _unit ,
							double _deadBandTranslation , double _deadBandRotation)
{
	this->wheelDistance = _wheelDistance;
	
	
	 // Setup the speed control on wheels 
	this->wheelRight->setup( _encoderRevolution , _radius , _unit , _sampleTime , 1) ;
	this->wheelLeft-> setup( _encoderRevolution , _radius , _unit , _sampleTime , 1) ;
	
	// Setup sample time for Position Conntroler
	this->polarPID->SetSampleTime(_sampleTime);
	this->translationPID->SetSampleTime(_sampleTime);
	this->rotationPID->SetSampleTime(_sampleTime);
	
	this->positionPIDLeft->SetSampleTime(_sampleTime);  // Not yet used
	this->positionPIDRight->SetSampleTime(_sampleTime); // Not yet used
	
	//Setup Mode as Automatic
	
    this->polarPID->SetMode(AUTOMATIC);
	this->rotationPID->SetMode(AUTOMATIC);
	this->translationPID->SetMode(AUTOMATIC);
	
	this->positionPIDLeft->SetMode(AUTOMATIC);  // Not yet used
	this->positionPIDRight->SetMode(AUTOMATIC); // Not yet used
	
	this->deadBandTranslation = _deadBandTranslation;
	this->deadBandRotation    = _deadBandRotation;
	
	this->odometer = new Odometer ( _sampleTime , this->wheelDistance , _radius * CENTIMETER);
 
}


void RobotMotion::setRegulatorParams(double _kp , double _ki , double _kd , int wheel , int kind , float limit) 
 // TODO : deprecated to be deleted in final version
 // wheel  = 1,2 for left or right wheel : kind = 1,2 for speed/position/polar controler
 {
 
  if(kind == SPEED_CONTROL) // speed regulation
  {
		if(wheel == LEFT_WHEEL) // speed controler for left wheel 
		{
			this->wheelLeft->setTuningParameter(_kp , _ki , _kd) ; 
			this->wheelLeft->setOutputLimit(-limit , limit);
		}
		else //speed controler for right wheel 
		{
			this->wheelRight->setTuningParameter(_kp , _ki , _kd) ;
			this->wheelRight->setOutputLimit(-limit , limit);			
		}
  
  }
  
  else if( kind == POSITION_CONTROL) 
  {
		this->polarPID->SetTunings(_kp , _ki , _kd);
		this->polarPID->SetOutputLimits(-limit,limit);
		
		if(wheel == LEFT_WHEEL) // position controler for left wheel 
		{	
			this->positionPIDLeft->SetTunings(_kp , _ki , _kd) ; 
			this->positionPIDLeft->SetOutputLimits(-limit,limit);
		}
		else // position controler for right wheel 
		{
			this->positionPIDRight->SetTunings(_kp , _ki , _kd) ; 
			this->positionPIDRight->SetOutputLimits(-limit,limit);
		}
  }
  
  else if(kind == TRANSLATION_CONTROL) 
  {
		this->translationPID->SetTunings(_kp , _ki , _kd); 
		this->translationPID->SetOutputLimits(-limit,limit); 
  }
  
   else if(kind == ROTATION_CONTROL) 
  {
		this->rotationPID->SetTunings(_kp , _ki , _kd); 
		this->rotationPID->SetOutputLimits(-limit,limit); 
  }
  
 }
 
 

 void RobotMotion::setRegulatorParams(double _kp , double _ki , double _kd , int wheel , int kind , float limit , float windUpLimit)

 {
 
  if(kind == SPEED_CONTROL) // speed regulation
  {
		if(wheel == LEFT_WHEEL) // speed controler for left wheel 
		{
			this->wheelLeft->setTuningParameter(_kp , _ki , _kd) ; 
			this->wheelLeft->setOutputLimit(-limit , limit);
			this->wheelLeft->setWindUpLimit(-windUpLimit , windUpLimit);
		}
		else //speed controler for right wheel 
		{
			this->wheelRight->setTuningParameter(_kp , _ki , _kd) ; 
			this->wheelRight->setOutputLimit(-limit , limit);
			this->wheelRight->setWindUpLimit(-windUpLimit , windUpLimit);
		}
  
  }
  else if( kind == POSITION_CONTROL)  // position regulation
  {
		this->polarPID->SetTunings(_kp , _ki , _kd);
		this->polarPID->SetOutputLimits(-limit,limit);
		this->polarPID->SetWindUpLimit(-windUpLimit,windUpLimit);
		
		if(wheel == LEFT_WHEEL) // position controler for left wheel 
		{	
			this->positionPIDLeft->SetTunings(_kp , _ki , _kd) ; 
			this->positionPIDLeft->SetOutputLimits(-limit,limit);
			this->positionPIDLeft->SetWindUpLimit(-windUpLimit,windUpLimit);
		}
		else // position controler for right wheel 
		{
			this->positionPIDRight->SetTunings(_kp , _ki , _kd) ; 
			this->positionPIDRight->SetOutputLimits(-limit,limit);
			this->positionPIDRight->SetWindUpLimit(-windUpLimit,windUpLimit);
		}
  }
  
   else if(kind == TRANSLATION_CONTROL)
  {
		this->translationPID->SetTunings(_kp , _ki , _kd); 
		this->translationPID->SetOutputLimits(-limit,limit); 
		this->translationPID->SetWindUpLimit(-windUpLimit,windUpLimit);
  }
    else if(kind == ROTATION_CONTROL)
  {
		this->rotationPID->SetTunings(_kp , _ki , _kd); 
		this->rotationPID->SetOutputLimits(-limit,limit); 
		this->rotationPID->SetWindUpLimit(-windUpLimit,windUpLimit);
  }
 }
  
 void RobotMotion::setControlerType(int type)
 {
	this->controlerType = type; 
 }
 
 void RobotMotion::setStopConditionParameter(float _positionErrorLimit , float _angleErrorLimit , float _speedStopLimit)
 {
	this->positionErrorLimit = _positionErrorLimit;
	this->angleErrorLimit    = _angleErrorLimit;
	this->speedStopLimit	 = _speedStopLimit;
 }
 
 
 void RobotMotion::stop()
 {
	this->wheelRight->stop();
	this->wheelLeft->stop(); 
 }
 
 void RobotMotion::translation(float distance) 
 {
  this->initTranslation(distance);
	
  do
  {
	this->currentDistance = (this->wheelRight->getPosition() + this->wheelLeft->getPosition()) / 2.0 ; // TYPE 1 : Used with a reset
   
	this->translationPID->Compute();
  
	//this->limitAcceleration
	
	this->wheelRight->setSpeed(speedCommand);
	this->wheelLeft->setSpeed(speedCommand);
 
	this->wheelRight->process(); 
	this->wheelLeft->process(); 
	
	this->odometer->process (this->wheelRight->getPosition() , this->wheelLeft->getPosition() );
	
	this->callDebug();
		
  } while(isNotStopDistance()) ;
  
  this->stop();
  delay(100); // time left for stop
  }

 void RobotMotion::turn(float angle) 
 {

  this->initTurn(angle);
  
  do
  {
	
	this->currentDistance = (this->wheelRight->getPosition() - this->wheelLeft->getPosition()) / 2.0 ; // TYPE 1 : Used with a reset
	
	this->rotationPID->Compute();
  
	this->wheelRight->setSpeed(speedCommand);
	this->wheelLeft->setSpeed(-speedCommand);
 
	this->wheelRight->process(); 
	this->wheelLeft->process(); 
	
	this->odometer->process (this->wheelRight->getPosition() , this->wheelLeft->getPosition() );
  
	this->callDebug();
	
  } while(isNotStopAngle()) ;
  
  this->stop();
  delay(100); // time left for stop
 }
 
 void RobotMotion::callDebug()
 {

	if(this->isDebugOn)
	{
		if(this->debugWheel == LEFT_WHEEL)
		{
			if(this->debugType == SPEED_CONTROL)
			{
				this->wheelLeft->displaySpeedForMatlab();
			}
			else
			{
				this->wheelLeft->displayPositionForMatlab();
			}
		}
		else
		{
			if(this->debugType == SPEED_CONTROL)
			{
				this->wheelRight->displaySpeedForMatlab();
			}
			else
			{
				this->wheelRight->displayPositionForMatlab();
			}
		
		}
	}
 
 }
 
 bool RobotMotion::isNotStopDistance()
 {
 
 return  (
           abs(this->demandedDistance - this->currentDistance) > this->positionErrorLimit  
          || abs(this->wheelRight->getSpeed()) > this->speedStopLimit 
          || abs(this->wheelLeft->getSpeed ()) > this->speedStopLimit 
           ) && this->stopConditionUsed ;
 
 }
 
 bool RobotMotion::isNotStopAngle()
 {
 return  (
           abs(this->toAngle(this->demandedDistance) - this->toAngle (this->currentDistance )) > this->angleErrorLimit  
          || abs(this->wheelRight->getSpeed()) > this->speedStopLimit 
          || abs(this->wheelLeft->getSpeed ()) > this->speedStopLimit 
           ) && this->stopConditionUsed ;
 
 }

 float RobotMotion::toDistance(float angle)
 {
	return (angle*PI/180.00) * (this->wheelDistance / 2.0) ;
 }
 
 float RobotMotion::toAngle(float distance)
 {
	 return (distance * (2.0/this->wheelDistance) * (180.00/PI) );
 }
 
 void RobotMotion::initTranslation(float distance) 
 {
  this->currentDistance = 0 ;
  this->wheelRight->resetEncoder();
  this->wheelLeft->resetEncoder();
  this->odometer->resetOdometer();
  
  this->wheelRight->setDeadBandValue(this->deadBandTranslation);
  this->wheelLeft-> setDeadBandValue(this->deadBandTranslation);
  
  this->demandedDistance = distance ; // add the translation distance
 }
 
 void RobotMotion::initTurn(float angle)
 {
  this->currentDistance = 0 ;
  
  this->wheelRight->resetEncoder();
  this->wheelLeft->resetEncoder();
  this->odometer->resetOdometer();
  
  this->wheelRight->setDeadBandValue(this->deadBandRotation);
  this->wheelLeft-> setDeadBandValue(this->deadBandRotation);
  
  this->demandedDistance = this->toDistance(angle) ; // convert the demanded angle to wheel distance
 }
 
double RobotMotion::getCurrentDistance() { return this->currentDistance;}
double RobotMotion::getCurrentAngle()    { return this->toAngle(this->currentDistance) ;}
double RobotMotion::getRemainedDistance() 
{ 
	return (this->demandedDistance - this->currentDistance );
}

