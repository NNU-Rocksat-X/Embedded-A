#ifndef step_h
#define step_h

//#include "Arduino.h"

class Stepper{
  public:
    Stepper(int _step_pin, int _dir_pin, int _enc_pin_A, int enc_pin_B, int encoderResolution);
    void step();
    int newFrequency(double velocity);
    int readEncoders();
    void currentVelocity(int currentPosition);  // populates the current velocity values on all the instances of this class
    double current_velocity;

  private:
    int step_pin;
    int dir_pin;
    int enc_pin_A;
    int enc_pin_B;
    bool direction;
    int motorFrequency;
    bool highLow;
    

    double pidController(double des_velocity);
    int elapsed_time;
    int previous_time;
    int previous_error;

    int encoder_resolution; // 300 CPR for NEMA 11 and 14, 1000 PPR & 4000 CPR for NEMA 17 and flat 14
    int current_vel_time;
    int current_vel_position;
    int previous_vel_time;
    int previous_vel_position;
    int delta_time;

    double integral;
    
};

#endif