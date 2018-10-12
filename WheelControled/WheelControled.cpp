

/* Diagramm describing purpose of the class

         e         pwm            voltage
Speed_D	---->|PID|------>|Hbrdige|------->|Motor|
	  |									     |
	  |   Speed_M			     ticks       |
	  |------------|Encoder|-----------------|

*/


#include <Arduino.h>
#include "WheelControled.h"


WheelControled::WheelControled() 
{  	
	// Initial conditions are equal to zero
	this->currentSpeed  = 0;
	this->outputCommand = 0;
	this->demandedSpeed = 0;
	
	// Create with default interface between Arduino and Encoder (see configuration file)
	this->myEncoder = new WheelEncoder(); 
	this->myEncoder->setup(DEFAULT_ENCODER_TICKS, DEFAULT_RADIUS);
	this->myEncoder->setUnit(DEFAULT_UNIT);
	this->myEncoder->setSampleTime(DEFAULT_SAMPLE_TIME);
	this->encoderType = this->myEncoder->getEncoderType();
	// Create default PID 
	this->myPID = new PID(& this->currentSpeed , & this->outputCommand , & this->demandedSpeed , DEFAULT_KP , DEFAULT_KI , DEFAULT_KD  , DIRECT);
	this->myPID->SetSampleTime(DEFAULT_SAMPLE_TIME); 
	this->myPID->SetMode(AUTOMATIC);
	this->myPID->SetOutputLimits(-DEFAULT_OUTPUT_LIMIT , DEFAULT_OUTPUT_LIMIT);
		
	// Create with default interface between Arduino and H bridge 
	this->myHbridge = new Hbridge(); 
	this->myHbridge->setup();
	
}


// Use this Constructor if you want to define yourself sensors and 
WheelControled::WheelControled(Hbridge* _hbridge , WheelEncoder* _wheelEncoder )
{
  
	this->myEncoder = _wheelEncoder;
	this->myEncoder->setup(DEFAULT_ENCODER_TICKS, DEFAULT_RADIUS , DEFAULT_UNIT , DEFAULT_SAMPLE_TIME);
	this->encoderType = this->myEncoder->getEncoderType();
	
	this->myPID = new PID(& this->currentSpeed , & this->outputCommand , & this->demandedSpeed , DEFAULT_KP , DEFAULT_KI , DEFAULT_KD  , DIRECT);
	this->myPID->SetSampleTime(DEFAULT_SAMPLE_TIME); 
	this->myPID->SetMode(AUTOMATIC);
	this->myPID->SetOutputLimits(-DEFAULT_OUTPUT_LIMIT , DEFAULT_OUTPUT_LIMIT);
	
	this->myHbridge = _hbridge;
	this->myHbridge->setup();
	
}

void WheelControled::setup(int _encoderRevolution , float _radius , float _unit , int _sampleTime , float _outputLimit)
{
	this->myEncoder->setup(_encoderRevolution, _radius , _unit , _sampleTime);
	
	this->myPID->SetSampleTime(_sampleTime); 
	this->myPID->SetOutputLimits(- _outputLimit , _outputLimit);

	this->myHbridge->setup();
	
}

void WheelControled::setup(int _encoderRevolution , float _radius , float _unit , int _sampleTime , float _outputLimit, int _encoderType) // TODO : to be deleted
{
	this->myEncoder->setup(_encoderRevolution, _radius , _unit , _sampleTime, _encoderType);
	
	this->myPID->SetSampleTime(_sampleTime); 
	this->myPID->SetOutputLimits(- _outputLimit , _outputLimit);

	this->myHbridge->setup();
	this->encoderType = _encoderType;
}


void WheelControled::setTuningParameter(double _kp , double _ki , double _kd )
{
	this->myPID->SetTunings(_kp , _ki , _kd);
}

void WheelControled::setOutputLimit(double outputMin , double outputMax)
{
	this->myPID->SetOutputLimits( outputMin , outputMax );
}

void WheelControled::setWindUpLimit(double windUpMin , double windUpMax)
{
	this->myPID->SetWindUpLimit(windUpMin , windUpMax);
}

void WheelControled::setSpeed(float _demandedSpeed)
{  
	this->demandedSpeed = _demandedSpeed;
}


void WheelControled::process() 
{  
	if(this->encoderType == SINGLE_ENCODER)
	{
		this->processWithSingleEncoder();
	}
	
	else
	{
		this->processWithQuadreEncoder();
	}
}


void WheelControled::processWithQuadreEncoder() // PATCH : to be able to manage single output Encoder 
{ 
 
 this->myEncoder->process();
 
 this->currentSpeed = this->myEncoder->getTranslationSpeed();
   
 this->myPID->Compute();
 
 this->myHbridge->setCmd(this->outputCommand);
 
 
}


void WheelControled::processWithSingleEncoder() // PATCH : to be able to manage single output Encoder 
{ 
  
 this->myEncoder->process();
 
 this->currentSpeed = (this->outputCommand >= 0) ? this->myEncoder->getTranslationSpeed() : -this->myEncoder->getTranslationSpeed() ;
  
 this->myPID->Compute();
 
 this->myHbridge->setCmd(this->outputCommand);

 
}

void WheelControled::displayPositionForMatlab() 
{	
	this->myEncoder->displayPositionForMatlab(); 
}

void WheelControled::displaySpeedForMatlab() 
{
	this->myEncoder->displaySpeedForMatlab(); 
}

void WheelControled::stop()
{
	this->myHbridge->setDirection(H_BRAKE);
	this->myPID->ResetIntegrator();
}

void WheelControled::setDeadBand(bool activateDeadBand , float deadBandValue)
{
	this->myHbridge->isDeadBand = activateDeadBand;
	this->myHbridge->setDeadBand(deadBandValue);
 
}

void WheelControled::setDeadBandValue(float deadBandValue)
{
	this->myHbridge->isDeadBand = true ;
	this->myHbridge->setDeadBand(deadBandValue);
}

void WheelControled::resetEncoder()
{
	this->myEncoder->resetEncoder();
}


float WheelControled::getPosition() { return this->myEncoder->getPosition()            ;}
float WheelControled::getSpeed()    { return this->myEncoder->getTranslationSpeed()    ;}