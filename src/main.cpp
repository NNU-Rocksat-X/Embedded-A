#include <Arduino.h>
#include <comm.h> 
#include "step.h"
#include "Encoder.h"
#include <TimerOne.h>

#define enc_pin_1A    2
#define enc_pin_2A    6
#define enc_pin_3A    17
#define enc_pin_4A    4
#define enc_pin_5A    15
#define enc_pin_6A    40
#define enc_pin_7A    38
#define enc_pin_8A    10

#define enc_pin_1B    3
#define enc_pin_2B    7
#define enc_pin_3B    18
#define enc_pin_4B    5
#define enc_pin_5B    16
#define enc_pin_6B    41
#define enc_pin_7B    39
#define enc_pin_8B    11

#define step_pin_1    24
#define step_pin_2    28
#define step_pin_3    32
#define step_pin_4    34
#define step_pin_5    36
#define step_pin_6    30
#define step_pin_7    26
#define step_pin_8    8

#define dir_pin_1     25
#define dir_pin_2     29
#define dir_pin_3     33
#define dir_pin_4     35
#define dir_pin_5     37
#define dir_pin_6     31
#define dir_pin_7     27
#define dir_pin_8     9

int32_t position_cmds[NUM_JOINTS];
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
const int m4_period = 0;
const int m5_period = 0;
const int m6_period = 0;
const int m7_period = 0;

double velocities[NUM_JOINTS];

uint32_t encoderValues[NUM_JOINTS];

Stepper myStepper[] = { Stepper(step_pin_1, dir_pin_1, enc_pin_1A, enc_pin_1B, 1000, 1),
                        Stepper(step_pin_2, dir_pin_2, enc_pin_2A, enc_pin_2B, 1000, 2), 
                        Stepper(step_pin_3, dir_pin_3, enc_pin_3A, enc_pin_3B, 300, 3),
                        Stepper(step_pin_4, dir_pin_4, enc_pin_4A, enc_pin_4B, 300, 4), 
                        Stepper(step_pin_5, dir_pin_5, enc_pin_5A, enc_pin_5B, 300, 5), 
                        Stepper(step_pin_6, dir_pin_6, enc_pin_6A, enc_pin_6B, 300, 6),
                        Stepper(step_pin_7, dir_pin_7, enc_pin_7A, enc_pin_7B, 1000, 7)};
                        //Stepper(step_pin_8, dir_pin_8, enc_pin_8A, enc_pin_8B, 1000, 8)}; 

Encoder myEncoder[] = { Encoder(enc_pin_1A, enc_pin_1B),
                        Encoder(enc_pin_2A, enc_pin_2B),
                        Encoder(enc_pin_3A, enc_pin_3B),
                        Encoder(enc_pin_4A, enc_pin_4B),
                        Encoder(enc_pin_5A, enc_pin_5B),
                        Encoder(enc_pin_6A, enc_pin_6B),
                        Encoder(enc_pin_7A, enc_pin_7B)};
                        //Encoder(enc_pin_8A, enc_pin_8B)}; 

int velocity[6];

/*
**************************************** -=+ Encoder Transmit Sequence +=- *****************************************/
// - Populates Encoder values

void sendEncoderValues() {
  
  for(int ii = 0; ii < NUM_JOINTS; ++ii) { 
    encoder_positions[ii] = myEncoder[ii].read();
  }
 
}


/***************************************** -=+ Initialize Encoders +=- *****************************************/
void initEncoders() {
  for (int kk = 0; kk < NUM_JOINTS; ++kk) {
    myEncoder[kk].write(0);
  }
}

void setup(void)
{

  noInterrupts();
  pinMode(13, OUTPUT);
  //setup_comm();

  initEncoders();
  
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
  
  //init m7
  tasks[ii].state = false;
  tasks[ii].elapsedTime = 0;
  tasks[ii].period = m7_period;
  pinMode(enc_pin_7A, INPUT);
  pinMode(enc_pin_7B, INPUT);

  /*  ++ii;
  
  //init m8
  tasks[ii].state = false;
  tasks[ii].elapsedTime = 0;
  tasks[ii].period = m7_period;
  pinMode(enc_pin_8A, INPUT);
  pinMode(enc_pin_8B, INPUT);*/
  
  pinMode(13, OUTPUT);                    // led pin to output

  Timer1.initialize(10);                  // interrupt every 10 us
  Timer1.attachInterrupt(motorISR);       // motorISR is the ISR
  Timer1.start();

  interrupts();

  setup_comm();

  initEncoders();

	pinMode(13, OUTPUT);
} 

void loop(void)
{
	bool led_power;

  if (get_cmd(&position_cmds[0], &led_power) == 2) {
    //digitalWrite(13, HIGH);
  }

  else {
    //digitalWrite(13, LOW);
  }

  //int temp_array[NUM_JOINTS];

  tasks[0].period = myStepper[0].newFrequency(myEncoder[0].read(), position_cmds[0]);   // set the period of each motor based on the velocities recived from the jetson
  tasks[1].period = myStepper[1].newFrequency(myEncoder[1].read(), position_cmds[1]);
  tasks[2].period = myStepper[2].newFrequency(myEncoder[2].read(), position_cmds[2]);
  tasks[3].period = myStepper[3].newFrequency(myEncoder[3].read(), position_cmds[3]);
  tasks[4].period = myStepper[4].newFrequency(myEncoder[4].read(), position_cmds[4]);
  tasks[5].period = myStepper[5].newFrequency(myEncoder[5].read(), position_cmds[5]);
  tasks[6].period = myStepper[6].newFrequency(myEncoder[6].read(), position_cmds[6]);
  //tasks[7].period = myStepper[7].newFrequency(myEncoder[7].read(), position_cmds[7]);

	delay(5);
	
  sendEncoderValues();
	send_feedback(&encoder_positions[0]);
}

void motorISR(void){
  for(int ii = 0; ii < NUM_JOINTS; ++ii) {

    if (tasks[ii].elapsedTime >= tasks[ii].period) { 
      myStepper[ii].step();               //call the step function
      tasks[ii].elapsedTime = 0;          //reset the elapsed time
    }
    tasks[ii].elapsedTime += 10;          //increase the elapsed time since the last time the function was called
  }

}

int main(void)
{
  setup();
	while (1) {
		loop();
	}

}