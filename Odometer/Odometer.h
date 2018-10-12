#ifndef Odometer_H
#define Odometer_H

#define LINEAR 1;
#define CIRCLE 2;

// All unit shall the same for (radius , wheelDistance , current X , etc etc )

class Odometer{

public:

Odometer(float _sampleTime, float _wheelDistance , float _radius ); // Deprecated : to be deleted in V6
Odometer(float _sampleTime, float _wheelDistance);
void setCoordinates(float _inputX , float _inputY , float _inputTheta);

void process(float currentRightWheelPosition , float currentLeftWheelPosition);

void resetOdometer(void); // in case of new motion encoders are reset , so the odometer has to be reset

float getCurrentX();
float getCurrentY();
float getCurrentTheta();


private:

float wheelDistance; //  distance between the two wheels
float radius;		 //  radius of each wheel holding encoder (It is supposed that they have the same radius but it is easy to change the code)
float sampleTime; 	 // Sample time in ms

// Coordinates of the robot 
float currentX; 
float currentY; 
float currentTheta; // in degrees

float lastRightWheelPosition; 
float lastLeftWheelPosition;  

long lastTime;


};


#endif