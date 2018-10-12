//TODO : Add a debug mode 
//TODO : Add identation
//TODO : Add documentation


#include <Arduino.h>
#include "WheelEncoder.h"
#include "Encoder.h" 


WheelEncoder::WheelEncoder() 
{  	
	
	this->canalAPin = DEFAULT_CANAL_A;
	this->canalBPin = DEFAULT_CANAL_B;
	this->encoder = new Encoder(this->canalAPin,this->canalBPin);
	
	//set default value in parameter
	this->ticksPerRevolution = DEFAULT_ENCODER_TICKS; // TODO : add a setter on this value on setup function 
	this->radius = DEFAULT_RADIUS ;				// TODO : add a setter on this value on setup function 
	this->previousPosition = 0;
	this->previousTranslationSpeed = 0;
	this->sampleTime = DEFAULT_SAMPLE_TIME;
	this->coeffUnit = METER;
	
	this->lastTime = millis() - this->sampleTime;
}

WheelEncoder::WheelEncoder(int selectedCanalAPin , int selectedCanalBPin)
{  
	this->canalAPin = selectedCanalAPin;
	this->canalBPin = selectedCanalBPin;
	
	this->encoder = new Encoder(this->canalAPin , this->canalBPin);
	
	//set default value in parameter
	this->ticksPerRevolution = DEFAULT_ENCODER_TICKS; // TODO : add a setter on this value on setup function 
	this->radius = DEFAULT_RADIUS ;				// TODO : add a setter on this value on setup function 
	this->previousPosition = 0;
	this->previousTranslationSpeed = 0;
	this->sampleTime = DEFAULT_SAMPLE_TIME;
	this->coeffUnit = METER;
	
	this->lastTime = millis() - this->sampleTime;	
}

WheelEncoder::WheelEncoder(int selectedCanalAPin)
{  
	this->canalAPin = selectedCanalAPin;
	this->canalBPin = USELESS_PIN;
	
	this->encoder = new Encoder(this->canalAPin);
	
	//set default value in parameter
	this->ticksPerRevolution = DEFAULT_ENCODER_TICKS; // TODO : add a setter on this value on setup function 
	this->radius = DEFAULT_RADIUS ;				// TODO : add a setter on this value on setup function 
	this->previousPosition = 0;
	this->previousTranslationSpeed = 0;
	this->sampleTime = DEFAULT_SAMPLE_TIME;
	this->coeffUnit = METER;
	
	this->lastTime = millis() - this->sampleTime;	
}


/** Definition of public methods **/

void WheelEncoder::setup()  // TODO : Obsolete to be deleted
{	
	//this->encoder->setup();
}

void WheelEncoder::setup(int inputTicksPerRevolution , float inputRadius)  // TODO : Obsolete to be deleted
{
	this->ticksPerRevolution = inputTicksPerRevolution ;
	this->radius = inputRadius;
}

void WheelEncoder::setup(int inputTicksPerRevolution , float inputRadius , float inputUnit , float inputSampleTime)
{

	this->ticksPerRevolution = inputTicksPerRevolution ;
	this->radius = inputRadius;
	this->coeffUnit = inputUnit;
	this->sampleTime = inputSampleTime ;
}

void WheelEncoder::setup(int inputTicksPerRevolution , float inputRadius , float inputUnit , float inputSampleTime , int inputEncoderType)
{

	this->ticksPerRevolution = inputTicksPerRevolution ;
	this->radius = inputRadius;
	this->coeffUnit = inputUnit;
	this->sampleTime = inputSampleTime ;
	this->encoder->setEncoderType(inputEncoderType);
}


int WheelEncoder::process() // process a new computation each sampleTime when called in a loop
{
   unsigned long now = millis();
   unsigned long timeChange = (now - this->lastTime);
   
   if(timeChange >= this->sampleTime)
	{
		computePosition();
		computeSpeed();
		this->previousPosition = this->currentPosition;
		this->lastTime = now;
	}

}

void WheelEncoder::setSampleTime(float inputSampleTime)
{
	this->sampleTime = inputSampleTime ;
}

void WheelEncoder::displayMeasure()
{
	Serial.print("Position : ");
	Serial.println(this->getPosition());
	Serial.print("Translation Speed : ");
	Serial.println(this->getTranslationSpeed());
}

void WheelEncoder::displaySpeedForMatlab()
{
	Serial.print(this->getTranslationSpeed());
	Serial.print(" ");
}
void WheelEncoder::displayPositionForMatlab()
{
	Serial.print(this->getPosition());
	Serial.print(" ");
}


float WheelEncoder::getPosition()
{
	return (this->currentPosition * this->coeffUnit) ; // TODO : use unit condition
}

float WheelEncoder::getAngle()
{
	return this->currentAngleRadian; // TODO : use unit condition
}

float WheelEncoder::getTranslationSpeed()
{
	return (this->currentTranslationSpeed * this->coeffUnit) ;
}

void WheelEncoder::setUnit(float unit)
{

this->coeffUnit = unit;
}

/** Definition of private methods **/

void WheelEncoder::computePosition()
{
	this->totalTicks = this->encoder->read();
	this->currentPosition = convertTicks(this->totalTicks);

}

void WheelEncoder::computeSpeed()
{
  this->currentTranslationSpeed = derivation(this->currentPosition,this->previousPosition);
}

void WheelEncoder::resetEncoder()
{
  this->encoder->write(0);
  this->currentPosition = 0 ;
  this->previousPosition =0;
  this->currentTranslationSpeed= 0;
  this->previousTranslationSpeed= 0;
}

float WheelEncoder::convertTicks(long measuredTicks)
{

 this->currentNbRevolution = (float) measuredTicks / ticksPerRevolution;
 this->currentAngleDegree = this->currentNbRevolution * REVOLUTION_TO_DEGREE;
 this->currentAngleRadian = (float) (this->currentAngleDegree * PI ) / RADIAN_TO_DEGREE ;
 
 return this->radius * this->currentAngleRadian;
  }
  

 float WheelEncoder::derivation(float currentValue , float previousValue ){
 
 return ( (currentValue - previousValue)  / (this->sampleTime * TO_SEC) );
 
 }
 
 int WheelEncoder::getEncoderType(){
	return this->encoder->getEncoderType();
 }
 
