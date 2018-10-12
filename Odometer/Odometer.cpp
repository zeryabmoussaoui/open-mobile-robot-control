#include "Odometer.h"
#include "Arduino.h"

// Theory is on : http://manubatbat.free.fr/doc/positionning/node5.html


Odometer::Odometer(float _sampleTime, float _wheelDistance , float _radius) // Deprecated because radius useless

{
	this->sampleTime = _sampleTime ;
	this->wheelDistance = _wheelDistance ;
	this->radius = _radius; // Useless
	this->currentX = 0 ;
	this->currentY = 0 ;
	this->currentTheta = 0 ;
	
	this->lastRightWheelPosition = 0 ;
	this->lastLeftWheelPosition = 0 ;
	
	this->lastTime = millis() - this->sampleTime  ;
}


Odometer::Odometer(float _sampleTime, float _wheelDistance)

{
	this->sampleTime = _sampleTime ;
	this->wheelDistance = _wheelDistance ;
	this->radius = 5*0.01 ; // Useless
	this->currentX = 0 ;
	this->currentY = 0 ;
	this->currentTheta = 0 ;
	
	this->lastRightWheelPosition = 0 ;
	this->lastLeftWheelPosition = 0 ;
	
	this->lastTime = millis() - this->sampleTime  ;
}

void Odometer::setCoordinates(float _inputX , float _inputY , float _inputTheta)

{
	this->currentX 		= _inputX ;
	this->currentY 		= _inputY ;
	this->currentTheta 	= _inputTheta ;
}

void Odometer::process(float currentRightWheelPosition , float currentLeftWheelPosition)
	/**
	Based on linear approximation of the robot motion : 
	between to close position the robot is supposed doing a translation then a rotation
	**/
{

   unsigned long now = millis();
   unsigned long timeChange = ( now - this->lastTime );
   
   if( timeChange >= this->sampleTime )
   {
	
		float deltaRightWheel = ( currentRightWheelPosition - this->lastRightWheelPosition ); 
		float deltaLeftWheel  = ( currentLeftWheelPosition  - this->lastLeftWheelPosition );
	
		float deltaDistance = (deltaRightWheel + deltaLeftWheel) / 2.0 ;
		float deltaAngle = (deltaRightWheel - deltaLeftWheel) / this->wheelDistance;

		float deltaX = deltaDistance * cos ( this->currentTheta );
		float deltaY = deltaDistance * sin ( this->currentTheta ); 
		
		this->currentX += deltaX ;
		this->currentY += deltaY ;
		this->currentTheta += deltaAngle ;
		
		this->lastLeftWheelPosition  = currentLeftWheelPosition  ;
		this->lastRightWheelPosition = currentRightWheelPosition ;
		
		this->lastTime = now ;
  
   }

}

 float Odometer::getCurrentX() { return this->currentX;}
 float Odometer::getCurrentY() { return this->currentY;}
 
 float Odometer::getCurrentTheta()
 { 
	// Represent the angle between -PI and PI
	while( abs(this->currentTheta) > PI )
	{
		if (this->currentTheta > 0)
		{
			this->currentTheta = this->currentTheta - 2*PI;
		}
		else
		{
			this->currentTheta = this->currentTheta + 2*PI;
		}
	}
	
	return this->currentTheta;
 
 }
 
 
 void Odometer::resetOdometer() 
 { 
 
 this->lastLeftWheelPosition  = 0 ;
 this->lastRightWheelPosition = 0 ;
 
 }
 