
//This class defines a speed control of a wheel using Encoder and Motor

#ifndef WheelControled_H
#define WheelControled_H

#include "WheelEncoder.h"
#include "Hbridge.h"
#include "PID_v1.h"

// TODO move all constants in a header file

#define DEFAULT_KP 0.5952 
#define DEFAULT_KI 0.001
#define DEFAULT_KD 0.0

#define DEFAULT_UNIT CENTIMETER 
#define DEFAULT_OUTPUT_LIMIT 1  

class WheelControled{ //TODO : add documentation

public:

WheelControled();
WheelControled(Hbridge* hbridge , WheelEncoder* wheelEncoder);


void setTuningParameter(double _kp , double _ki , double _kd );
void setOutputLimit(double outputMin , double outputMax);
void setWindUpLimit(double windUpMin , double windUpMax);
void setSpeed(float _demandedSpeed);
void setup(int _encoderRevolution , float _radius , float _unit , int _sampleTime , float _outputLimit);
void setup(int _encoderRevolution , float _radius , float _unit , int _sampleTime , float _outputLimit , int _encoderType);
void setDeadBand(bool activateDeadBand , float deadBandValue);
void setDeadBandValue(float deadBandValue);
void process();
void processWithSingleEncoder(); // Patch for Encoder with only one output
void processWithQuadreEncoder();
void resetEncoder();
void displaySpeedForMatlab();
void displayPositionForMatlab();

void stop();

float getSpeed();
float getPosition();
//float getAccelelation();

private:

// Declaration of object used in control loop

WheelEncoder * myEncoder;
PID * myPID;
Hbridge * myHbridge;
int encoderType; // Patch for Encoder with only one output

// Outputs and inputs of closed loop

double currentSpeed;  
double outputCommand;
double demandedSpeed ;


};

#endif