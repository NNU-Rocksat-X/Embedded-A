#include "step.h"
#include <Arduino.h>

Stepper::Stepper(int _step_pin, int _dir_pin, int _enc_pin_A, int _enc_pin_B, int encoderResolution){

  step_pin = _step_pin;
  dir_pin = _dir_pin;
  enc_pin_A = _enc_pin_A;
  enc_pin_B = _enc_pin_B;
  encoder_resolution = encoderResolution;

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

  //delaymicroseconds(10)
  
  else{
    digitalWrite(step_pin, LOW);
    highLow = LOW;
    
  }
}
/*
double Stepper::pidController(double goal_position) {

  double proportional_gain = 1;
  double integral_gain = 0.001;
  double derivative_gain = 0.05;
  double i_clamp = 0.01;

  double pid_velocity;

  int now_time = millis();

  double error = goal_position - current_position;                  // error is the desired velocity minus the current velocity
  elapsed_time = now_time - previous_time;

  double derivative = (error - previous_error) / (elapsed_time);   // acceleration in velocity per milisecond

  integral += error;

  if (integral > i_clamp) {
    integral = i_clamp;
  }

  if (error < 1 && error > -1) {
    integral = 0;
  }

  pid_velocity = proportional_gain * error; //+ integral_gain * integral + derivative_gain * derivative; // basic PID math

  previous_error = error;                                        // set last errors
  previous_time = now_time;                                       // set prev time to caclculate elapsedTime

  return pid_velocity;
}*/


double Stepper::pid_controller(double desired_angle, double current_angle) {
  double proportional_gain = 0.1;
  double integral_gain = 0.001;
  double derivative_gain = 0.1;
  double max_integral = 0.1;

  double now_time = millis();
  double delta_time = now_time - previous_time;
  previous_time = now_time;

  error = desired_angle - current_angle;
  integral += error;
  derivative = (error - previous_error) / delta_time;

  if (integral > max_integral) {         // stop the integral from getting out of control
    integral = max_integral;
  }
  else if (integral < -max_integral) {
    integral = -max_integral;
  }

  if (fabs(error) < 5) {              // reset integral if the error is enough
    integral = 0;
  }

  if (error > -10 && error < 10) {
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }
  

  previous_error = error;

  return velocity = error * proportional_gain;// + integral * integral_gain + derivative * derivative_gain;
}

int Stepper::newFrequency(double position, double desired_position) {
  int velocity; 

  velocity = pid_controller(desired_position, position);

  if(velocity > 0) {
    direction = HIGH;
  }
  else {
    direction = LOW;
  }

  if (fabs(velocity) < 0.001) {
    motorFrequency = 1000000;
    //digitalWrite(13, LOW);
  }
  else {
    motorFrequency = (1000 / abs(velocity)); // this just needs to invert the velocity values
    //digitalWrite(13, HIGH);
  }

  if(motorFrequency <= 15){
    motorFrequency = 15;
  }
  else{
  }

  //take steps/s convert to s/step and convert to microseconds 
  
  return motorFrequency;
  
}
/*
void Stepper::currentVelocity(int currentPosition){
  int counts_per_step = (encoder_resolution * 1.8)/ (360.0); // counts per step with a resolution of 

  current_vel_time = millis();

  delta_time = current_vel_time - previous_vel_time;

  current_velocity = ((1 / counts_per_step) * (currentPosition - previous_vel_position)) / delta_time; // steps per microsecond 

  previous_vel_time = current_vel_time;
  previous_vel_position = currentPosition;

  if((1 / current_velocity) < motorFrequency) {
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }
}*/


/*Encoder encoder(enc_pin_A, enc_pin_B)

int readEncoders() {
  return encoder.read();
}*/