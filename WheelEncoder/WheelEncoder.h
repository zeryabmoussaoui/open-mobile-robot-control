#ifndef WheelEncoder_H
#define WheelEncoder_H

#include "Encoder.h" 

// TODO move all constants in a header file

#define DEFAULT_CANAL_A 19 
#define DEFAULT_CANAL_B 18
#define DEFAULT_SAMPLE_TIME 41
#define DEFAULT_ENCODER_TICKS 360
#define DEFAULT_RADIUS 2.86*0.01
#define USELESS_PIN 0




// TODO : re order and regroup by functionnality
// TODO : implement remained methods
// TODO : Add comment 


class WheelEncoder
{
public:

  //Some of the constants used in the class
  
  #define RADIAN_TO_DEGREE 180
  #define REVOLUTION_TO_DEGREE 360
  #define TO_SEC 0.001
  #define METER 1
  #define CENTIMETER 100 

  WheelEncoder();
  WheelEncoder(int canalAPin , int canalBPin);
  WheelEncoder(int canalAPin);
  void setup();
  void setup(int precision , float radius);
  void setup(int precision , float radius , float unit , float inputSampleTime);
  void setup(int precision , float radius , float unit , float inputSampleTime ,int inputEncoderType);
  //void launch();
  void resetEncoder();
  //void stop();
  void setUnit(float unit);
  void setSampleTime(float inputSampleTime); // use integer instead
  //void setSpeedUnit(int unit);
  //void setTimeUnit(int unit);
  float getPosition();
  float getAngle();
  float getTranslationSpeed();
  int getEncoderType();
  //float getAcceleration();
  //float getAngularSpeed();
  //int getState();
  //int getDirection();
  //void setPositionPrecision(float positionPrecision);
  void displayMeasure();
  void displayPositionForMatlab();
  void displaySpeedForMatlab();
  int process();


private:

	//Encoder encoder;
	int canalAPin;
	int canalBPin;
	int ticksPerRevolution;
	//const float deadBandFrequency;
    float radius  ; // TODO : add a setup 	
	float sampleTime;
	long totalTicks;
	float coeffUnit;
	
	float currentAngleDegree;
	float currentAngleRadian;
	
	float currentTranslationSpeed;
	float previousTranslationSpeed;
	
	float convertTicks(long ticks);
	float derivation(float currentValue , float previousValue);
	
	unsigned long lastTime;

	void computeSpeed();
	void computePosition();
	float currentNbRevolution;
	float previousPosition;
	float currentPosition;
	Encoder * encoder;  // encoder object counting the impulsion
	
	
};

#endif