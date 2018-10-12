#ifndef Hbridge_H

#define Hbridge_H
#define H_STOP 0
#define H_DIRECT 1
#define H_INVERTED 2
#define H_BRAKE 3

#define H_DEADBAND 0.01
#define H_USELESS 0  
  
#define H_STANDARD_TYPE 0
#define H_NON_STANDARD_TYPE 1

class Hbridge
{
public:



  Hbridge();
  Hbridge(int selectedInput1Pin , int selectedInput2Pin , int selectedEnablePin);
  Hbridge(int selectedInputPin , int selectedEnablePin);
  
  void setup();  				   // setup pins of the H bridge as output of the Arduino
  int setDirection(int direction); // return 0 if OK else it returns -1
  int setPwmCmd(float dutyRatio);  // return 0 if OK else it returns -1
  void setCmd(float rateCmd);      
  int getState();
  
  bool isDeadBand;  
  void setDeadBand(float _deadBandValue);   

private:
	int input1Pin;
	int input2Pin;
	int enablePin;
	int directionState;
	int bridgeType; 
	float deadBandValue;
};

#endif