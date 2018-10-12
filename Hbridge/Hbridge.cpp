
#include <Arduino.h> 

#include "Hbridge.h"


Hbridge::Hbridge()
{  
	this->input1Pin = 4;
	this->input2Pin = 5;
	this->enablePin = 2;
	
	this->isDeadBand = false;
	this->deadBandValue = H_DEADBAND;
	this->bridgeType = H_STANDARD_TYPE;
}


Hbridge::Hbridge(int selectedInput1Pin , int selectedInput2Pin , int selectedEnablePin)
{  
	this->input1Pin = selectedInput1Pin;
	this->input2Pin = selectedInput2Pin;
	this->enablePin = selectedEnablePin;
	
	this->isDeadBand = false;
	this->deadBandValue = H_DEADBAND;
	this->bridgeType = H_STANDARD_TYPE;
}

Hbridge::Hbridge(int selectedInputPin , int selectedEnablePin)
{  
	this->input1Pin = selectedInputPin;
	this->input2Pin = H_USELESS;
	this->enablePin = selectedEnablePin;
	
	this->isDeadBand = false;
	this->deadBandValue = H_DEADBAND;
	this->bridgeType = H_NON_STANDARD_TYPE;
}

void Hbridge::setup()
{
	if (this->bridgeType == H_STANDARD_TYPE)
	{
		pinMode(this->input1Pin, OUTPUT);
		pinMode(this->input2Pin, OUTPUT);
		pinMode(this->enablePin, OUTPUT);
	}
	
	else if (this->bridgeType == H_NON_STANDARD_TYPE)
	{
		pinMode(this->input1Pin, OUTPUT);
		pinMode(this->enablePin, OUTPUT);
	}
}

int Hbridge::setDirection(int directionCmd)
{

if (this->bridgeType == H_STANDARD_TYPE){

	switch(directionCmd){
		case H_BRAKE:
			digitalWrite(this->input1Pin,HIGH);
			digitalWrite(this->input2Pin,HIGH);
			this->setPwmCmd(1);
			break;	
		case H_STOP:
			digitalWrite(this->input1Pin,LOW);
			digitalWrite(this->input2Pin,LOW);
			break;
		case H_DIRECT:
			digitalWrite(this->input1Pin,HIGH);
			digitalWrite(this->input2Pin,LOW);
			break;
		case H_INVERTED:
			digitalWrite(this->input1Pin,LOW);
			digitalWrite(this->input2Pin,HIGH);
			break;
		default :
			return -1;
}
}

else if (this->bridgeType == H_NON_STANDARD_TYPE){
	switch(directionCmd){
		case H_BRAKE:
			digitalWrite(this->input1Pin,LOW);
			this->setPwmCmd(0);
			break;	
		case H_STOP:
			digitalWrite(this->input1Pin,LOW);
			this->setPwmCmd(0);
			break;
		case H_DIRECT:
			digitalWrite(this->input1Pin,HIGH);
			break;
		case H_INVERTED:
			digitalWrite(this->input1Pin,LOW);
			break;
		default :
			return -1;
}
}


	this->directionState=directionCmd;
	return 0;
}

int Hbridge::setPwmCmd(float dutyRatio)
{
	if(abs(dutyRatio)>1){
		return -1;
	}

	else
	{
		analogWrite(this->enablePin , 255*dutyRatio);
		delay(1);
	}
}

void Hbridge::setCmd(float rateCmd)
{

	//Serial.print(rateCmd); //TODO : implement a debug function
	
	if( (abs(rateCmd) < this->deadBandValue ) && isDeadBand )
	{
		this->setDirection(H_BRAKE);
	}

	else 
	{
		if(rateCmd<0)
		{
			this->setDirection(H_INVERTED);
			this->setPwmCmd(-rateCmd);
		}
		else
		{
			this->setDirection(H_DIRECT);
			this->setPwmCmd(rateCmd);
		}

	}
}

void  Hbridge::setDeadBand(float _deadBandValue)
{
	if( abs(deadBandValue) > 1 )
	{
		this->deadBandValue = abs(H_DEADBAND);
	}
	else
	{
		this->deadBandValue = abs(_deadBandValue);
	}

}


int Hbridge::getState()
{
	return this->directionState;
}

