#include <Arduino.h> 

#include "Robot.h"

Robot::Robot(RobotMotion* _robotMotion)
{
	this->robotMotion = _robotMotion;
	this->currentX = 0;	
	this->currentY = 0;
	this->currentTheta = 0;
}


void Robot::setInitialConditions(double x0 , double y0 , double theta0)
{
	this->robotMotion->odometer->setCoordinates(x0, y0, theta0);
}


void Robot::moveTo(double finalX , double finalY)
/**
 Generate a rotation and a translation to reach the point (finalX , finalY)
*/
{
	this->currentX = this->robotMotion->odometer->getCurrentX();
	this->currentY = this->robotMotion->odometer->getCurrentY();
	this->currentTheta = this->robotMotion->odometer->getCurrentTheta();

	double distance = sqrt ( pow( finalX - this->currentX , 2) + pow ( finalY - this->currentY , 2 ) );  // pythagore : D^2 = x^2 + y^2 
	double angleRot = atan2( finalY - this->currentY , finalX - this->currentX ) - this->currentTheta ;  // compute the rotation to do
	
	angleRot = 180.0 * angleRot / PI;   // convertion to degree

	this->turnBy(angleRot);
	this->translateBy(distance);
}


void Robot::turnBy(double angleRot)
{
	this->robotMotion->turn(angleRot);
}


void Robot::translateBy(double distance) 
{
	this->robotMotion->translation(distance);
	
}

void Robot::turnTo(double finalAngle)
{
	this->currentTheta = this->robotMotion->odometer->getCurrentTheta();
	double angleRot = finalAngle - 180.0 * this->currentTheta / PI ;	
	this->robotMotion->turn(angleRot);
}

void Robot::finalStop()
{
	this->robotMotion->stopConditionUsed = false;
}

void Robot::stopRobot()
{
	this->robotMotion->stopConditionUsed = false; 
	this->robotMotion->stop();
	
}

void Robot::restartRobot()
{
	this->robotMotion->stopConditionUsed = true; 
	this->robotMotion->translation(this->robotMotion->getRemainedDistance());
}

