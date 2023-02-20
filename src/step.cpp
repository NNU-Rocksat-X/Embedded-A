#include "step.h"
//#include <Encoder.h>

Stepper::Stepper(int _step_pin, int _dir_pin, int _enc_pin_A, int _enc_pin_B){

  step_pin = _step_pin;
  dir_pin = _dir_pin;
  enc_pin_A = _enc_pin_A;
  enc_pin_B = _enc_pin_B;

  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
}

void Stepper::step(){
  if(!highLow){
    if(direction == HIGH){
      digitalWrite(dir_pin, HIGH);
      digitalWrite(step_pin, HIGH);
    }
    
    else{
      digitalWrite(dir_pin, LOW);
      digitalWrite(step_pin, HIGH);
    }
    
    highLow = HIGH;
  }
  
  else{
    digitalWrite(step_pin, LOW);
    highLow = LOW;
    
  }
}

int Stepper::newFrequency(double velocity) {

  if(velocity > 0) {
    direction = HIGH;
  }
  else {
    direction = LOW;
  }

  velocity = abs(velocity);

  motorFrequency = velocity; // this just needs to invert the velocity values

  //take steps/s convert to s/step and convert to microseconds 
  
  return motorFrequency;
  
}


/*Encoder encoder(enc_pin_A, enc_pin_B)

int readEncoders() {
  return encoder.read();
}*/