//CAREFUL
// Digital pin 13 used by Encoder function
// PWM 11 12 13 cannot be used as PWM because TimerOne is Used

#include <Hbridge.h>
#include <Encoder.h>  // used in WheelEncoder class
#include <WheelEncoder.h>
#include <PID_v1.h>
#include <WheelControled.h>

WheelControled wheelControled;

void setup()
{
 // Serial for debugging
 Serial.begin(9600);
 wheelControled.setup(180 , DEFAULT_RADIUS , CENTIMETER , DEFAULT_SAMPLE_TIME, DEFAULT_OUTPUT_LIMIT, SINGLE_ENCODER); 
 wheelControled.setSpeed(-10);
}

void loop()
{

  wheelControled.process(); 
  wheelControled.displaySpeedForMatlab(); //Attention : Affiche toujours les valeurs positives
 }
 

