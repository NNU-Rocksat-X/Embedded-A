#include <Arduino.h>
#include <comm.h> 
#include "step.h"
#include "Encoder.h"
#include <TimerOne.h>

#define enc_pin_1A    0
#define enc_pin_2A    0
#define enc_pin_3A    17
#define enc_pin_4A    4
#define enc_pin_5A    15
#define enc_pin_6A    40
#define enc_pin_7A    38

#define enc_pin_1B    0
#define enc_pin_2B    0
#define enc_pin_3B    18
#define enc_pin_4B    5
#define enc_pin_5B    16
#define enc_pin_6B    41
#define enc_pin_7B    39

#define step_pin_1    24
#define step_pin_2    28
#define step_pin_3    32
#define step_pin_4    34
#define step_pin_5    36
#define step_pin_6    30
#define step_pin_7    26

#define dir_pin_1     25
#define dir_pin_2     29
#define dir_pin_3     33
#define dir_pin_4     35
#define dir_pin_5     37
#define dir_pin_6     31
#define dir_pin_7     27

int32_t velocity_cmds[NUM_JOINTS];
int32_t encoder_positions[NUM_JOINTS];
int prevEncoderPos = 0;

typedef struct TaskScheduler{
  bool state;
  volatile int elapsedTime;
  int period;
  void (*function)();
} scheduledTasks;

scheduledTasks tasks[7];

void motorISR(void);

int tasksPerPeriod = 10;
int tasksNum = 7;

// how many ISR cycles to stay on
const int m1_OnTime = 1;
const int m2_OnTime = 1;
const int m3_OnTime = 1;
const int m4_OnTime = 1;
const int m5_OnTime = 1;
const int m6_OnTime = 1;

//period between the functions in 
const int m1_period = 0;
const int m2_period = 0;
const int m3_period = 0;
const int m4_period = 100000;
const int m5_period = 0;
const int m6_period = 0;
const int m7_period = 0;

double velocities[NUM_JOINTS];

uint32_t encoderValues[NUM_JOINTS];

Stepper myStepper[] = { Stepper(step_pin_1, dir_pin_1, enc_pin_1A, enc_pin_1B, 1000),
                        Stepper(step_pin_2, dir_pin_2, enc_pin_2A, enc_pin_2B, 1000), 
                        Stepper(step_pin_3, dir_pin_3, enc_pin_3A, enc_pin_3B, 300),
                        Stepper(step_pin_4, dir_pin_4, enc_pin_4A, enc_pin_4B, 300), 
                        Stepper(step_pin_5, dir_pin_5, enc_pin_5A, enc_pin_5B, 300), 
                        Stepper(step_pin_6, dir_pin_6, enc_pin_6A, enc_pin_6B, 300),
                        Stepper(step_pin_7, dir_pin_7, enc_pin_7A, enc_pin_7B, 300)}; 

Encoder myEncoder[] = { Encoder(enc_pin_1A, enc_pin_1B),
                        Encoder(enc_pin_2A, enc_pin_2B),
                        Encoder(enc_pin_3A, enc_pin_4B),
                        Encoder(enc_pin_4A, enc_pin_4B),
                        Encoder(enc_pin_5A, enc_pin_5B),
                        Encoder(enc_pin_6A, enc_pin_6B),
                        Encoder(enc_pin_7A, enc_pin_7B)}; 

int velocity[6];

/***************************************** -=+ Encoder Transmit Sequence +=- *****************************************/
void sendEncoderValues() {
  
  for(int ii = 0; ii < NUM_JOINTS; ++ii) { 
    encoder_positions[ii] = myEncoder[ii].read();
  }

  //send_feedback(&encoder_positions[0]);
 
}



void setup(void)
{

  noInterrupts();
  pinMode(13, OUTPUT);
  //setup_comm();
  
  int ii = 0;
  
  //init m1
  tasks[ii].state = false;
  tasks[ii].elapsedTime = 0;
  tasks[ii].period = m1_period;
  //pinMode(enc_pin_1, INPUT);

  ++ii;
  
  //init m2
  tasks[ii].state = false;
  tasks[ii].elapsedTime = 0;
  tasks[ii].period = m2_period;
  //pinMode(enc_pin_2, INPUT);

  ++ii;
  
  //init m3
  tasks[ii].state = false;
  tasks[ii].elapsedTime = 0;
  tasks[ii].period = m3_period;
  pinMode(enc_pin_3A, INPUT);
  pinMode(enc_pin_3B, INPUT);

  ++ii;
  
  //init m4
  tasks[ii].state = false;
  tasks[ii].elapsedTime = 0;
  tasks[ii].period = m4_period;
  pinMode(enc_pin_4A, INPUT);
  pinMode(enc_pin_4B, INPUT);

  ++ii;
  
  //init m5
  tasks[ii].state = false;
  tasks[ii].elapsedTime = 0;
  tasks[ii].period = m5_period;
  pinMode(enc_pin_5A, INPUT);
  pinMode(enc_pin_5B, INPUT);

  ++ii;
  
  //init m6
  tasks[ii].state = false;
  tasks[ii].elapsedTime = 0;
  tasks[ii].period = m6_period;
  pinMode(enc_pin_6A, INPUT);
  pinMode(enc_pin_6B, INPUT);

    ++ii;
  
  //init 76
  tasks[ii].state = false;
  tasks[ii].elapsedTime = 0;
  tasks[ii].period = m7_period;
  pinMode(enc_pin_7A, INPUT);
  pinMode(enc_pin_7B, INPUT);
  
  pinMode(13, OUTPUT);                    // led pin to output

  //interrupts();

  Timer1.initialize(10);                  // interrupt every 10 us
  Timer1.attachInterrupt(motorISR);       // motorISR is the ISR
  Timer1.start();

  interrupts();

  setup_comm();

	pinMode(13, OUTPUT);
} 

void loop(void)
{
	bool led_power;
	
	get_cmd(&velocity_cmds[0], &led_power);
/*
  for(int ii = 0; ii < NUM_JOINTS; ++ii) {                        // set the period of each motor
    tasks[ii].period = myStepper[ii].NewFrequency(velocity_cmds[ii]);
  }*/

  //digitalWrite(13, led_power);

  tasks[0].period = myStepper[0].newFrequency(velocity_cmds[0]);   // set the period of each motor based on the velocities recived from the jetson
  tasks[1].period = myStepper[1].newFrequency(velocity_cmds[1]);
  tasks[2].period = myStepper[2].newFrequency(velocity_cmds[2]);
  tasks[3].period = myStepper[3].newFrequency(velocity_cmds[3]);
  tasks[4].period = myStepper[4].newFrequency(velocity_cmds[4]);
  tasks[5].period = myStepper[5].newFrequency(velocity_cmds[5]);
  tasks[6].period = myStepper[6].newFrequency(velocity_cmds[6]);



	delay(10);
	//digitalWrite(13, led_power);
	
  sendEncoderValues();
  encoder_positions[0] = myStepper->current_velocity;
	send_feedback(&encoder_positions[0]);

  if ( myStepper->current_velocity == 0) {
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }

  //prevEncoderPos = encoder_positions[5];

  for (int jj = 0; jj < 7; ++jj) {
    myStepper[jj].currentVelocity(encoder_positions[jj]);
  }

}

void motorISR(void){
  //digitalWrite(13, HIGH);
  for(int ii = 0; ii < NUM_JOINTS; ++ii) {
    
    if (tasks[ii].elapsedTime >= tasks[ii].period) { // Ready
   
      myStepper[ii].step();               //call the step function
      tasks[ii].elapsedTime = 0;          //reset the elapsed time
    
    }
    tasks[ii].elapsedTime += 10;          //increase the elapsed time since the last time the function was called
  }

  //digitalWrite(13, LOW);
}

int main(void)
{
  setup();
	while (1) {
		loop();
	}

}



/*
double pid_controller(int joint_id, double position_cmd){
  static double error_sum[15];
  static std::vector<double> prev_error[15];
  static std::vector<double> prev_time[15];
  static int d_cnt = 0;

  // proportional component
  double error = position_cmd - pos[joint_id];
  double v = error * gains[joint_id]["p"];

  // integral component
  error_sum[joint_id] += error * gains[joint_id]["i"];

  // clegg integrator (but if close to zero set make it equal to 0)
  if (error < 0.001 && error > -0.001) {
    error_sum[joint_id] = 0;
  }
  if (error_sum[joint_id] > gains[joint_id]["i_clamp"]) {
    error_sum[joint_id] = gains[joint_id]["i_clamp"]
  }
  else if (error_sum[joint_id] < -1 * gains[joint_id]["i_clamp"]) {
    error_sum[joint_id] = -1 * gains[joint_id]["i_clamp"]
  }

  v += error_sum[joint_id];

  // derivative component
  double now = ; // we need to get the current time as ros sees it

  double d = 0;
  if (prev_error[joint_id].size() > 0) {
    d = (error - prev_error[joint_id].back()) / (now - prev_time[joint_id].back());
  }

  prev_error[joint_id].push_back(error);
  prev_time[joint_id].push_back(now);

  if ( prev_error[joint_id].size() > 10) {
    prev_error[joint_id].pop_back();
    prev_time[joint_id].pop_back();
  }

  v += d * gains[joint_id]["d"];

  return v;
}*/