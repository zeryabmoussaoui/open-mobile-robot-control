#include <Hbridge.h>
#include <Encoder.h> 
#include <WheelEncoder.h>

// Sensor and actuator
Hbridge hbridge;
WheelEncoder wheelEncoder(19,18);




void setup(){
  // initialize serial:
  Serial.begin(9600);

  //initialize the H Bridge on default value

  hbridge.setup();
  hbridge.setCmd(1);

  //intialize the wheel encoder on default value
  wheelEncoder.setup(180 , 2.86 * 0.01 , CENTIMETER , 40 , SINGLE_ENCODER ); // encoder ticks per revolution , radius of the wheel , unit , sample time , Encoder type

}


void loop(){

  wheelEncoder.process();       // compute position and speed 
  wheelEncoder.displaySpeedForMatlab(); // display unit in unit chosen
  
  // get the step response  from the serial port , then plot it in matlab to get response time and static gain 
  // Then , we can have the PI coeff : Kp/Ki = response time ; desired reponse time = 1/K0*Ki 
  
  
}


