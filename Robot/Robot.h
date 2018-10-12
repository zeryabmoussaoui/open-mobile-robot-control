#ifndef Robot_H
#define Robot_H

#include "RobotMotion.h"
#include "Hbridge.h"


class Robot{

public:

Robot(RobotMotion* _robotMotion);

void setInitialConditions(double x0 , double y0 , double theta0); 


// State
void finalStop(); // simple stop + inhibition of closed loop 
void stopRobot();
void restartRobot();


// Motion methods
void moveTo(double finalX , double finalY);
void translateBy(double distance);
void turnBy(double angle);
void turnTo(double finalAngle);

RobotMotion* robotMotion;

private:

// current coordinate of the robot 
double currentX;
double currentY;
double currentTheta; // angle corresponding to the heading 

};

#endif