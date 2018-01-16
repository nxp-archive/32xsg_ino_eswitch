#include <eSwitch.h>

float current = 0;

void setup() {
  //Init function initialize the eSwitch pins and SPI
  Init();

  //Sets the CSNS to Output3
  SetMonitoring(3);

  //Turn on Output3
  SetOutputState(3, 1);
}

void loop() {
  
  for(int i=0;i<255;i++){
    //Change the PWM value of Output2
    SetPWMDuty(2, i);
    delay(500);
  }
  //Read the current of output set by "SetMonitoring" function 
  //in the setup code
  current = GetOutputCurrent();
    
}
