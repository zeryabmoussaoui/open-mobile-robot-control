#include <Hbridge.h>


Hbridge hbridge;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete



void setup(){
 // initialize serial:
 Serial.begin(9600);
 // reserve 200 bytes for the inputString:
 inputString.reserve(200);
 //initialize the H Bridge
 hbridge.setup();
 
 hbridge.setPwmCmd(1);
 hbridge.setDirection(H_DIRECT);
   
}


void loop(){
  
  if (stringComplete) {
    if(inputString.startsWith("DIRECT")){
     hbridge.setDirection(H_DIRECT); 
    }
    else if(inputString.startsWith("BRAKE")){
     hbridge.setDirection(H_BRAKE); 
    }
  }  
}


void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}

