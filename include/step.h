#ifndef step_h
#define step_h

#include "Arduino.h"
//#include <Encoder.h>

class Stepper{
  public:
    Stepper(int _step_pin, int _dir_pin, int _enc_pin_A, int enc_pin_B);
    void step();
    int newFrequency(double velocity);
    int readEncoders();

  private:
    int step_pin;
    int dir_pin;
    int enc_pin_A;
    int enc_pin_B;
    bool direction;
    int motorFrequency;
    bool highLow;
    
};

#endif