//#include <Arduino.h>
#include <comm.h> 
#include <step.h>
#include "Encoder.h"
#include "TimerOne.h"

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
#define step_pin_7    0

#define dir_pin_1     25
#define dir_pin_2     29
#define dir_pin_3     33
#define dir_pin_4     35
#define dir_pin_5     37
#define dir_pin_6     31
#define dir_pin_7     0

float velocity_cmds[NUM_JOINTS];
uint32_t encoder_positions[NUM_JOINTS];
/*
void setup(void)
{
	pinMode(13, OUTPUT);
} 

void loop(void)
{
	bool led_power;
	
	get_cmd(&velocity_cmds[0], &led_power);


        //digitalWrite(13, HIGH);
	delay(100);
	digitalWrite(13, led_power);
	

	send_feedback(&encoder_positions[0]);
	delay(100);
}
*/
typedef struct TaskScheduler{
  bool state;
  int elapsedTime;
  int period;
  void (*function)();
} scheduledTasks;

scheduledTasks tasks[6];

int tasksPerPeriod = 10;
int tasksNum = 6;

// how many ISR cycles to stay on
const int m1_OnTime = 1;
const int m2_OnTime = 1;
const int m3_OnTime = 1;
const int m4_OnTime = 1;
const int m5_OnTime = 1;
const int m6_OnTime = 1;

//period between the functions in 
const int m1_period = 10;
const int m2_period = 10;
const int m3_period = 10;
const int m4_period = 10;
const int m5_period = 10;
const int m6_period = 10;

uint32_t encoderValues[7];

Stepper myStepper[] = { Stepper(step_pin_1, dir_pin_1, enc_pin_1A, enc_pin_1B),
                        Stepper(step_pin_2, dir_pin_2, enc_pin_2A, enc_pin_2B), 
                        Stepper(step_pin_3, dir_pin_3, enc_pin_3A, enc_pin_3B),
                        Stepper(step_pin_4, dir_pin_4, enc_pin_4A, enc_pin_4B), 
                        Stepper(step_pin_5, dir_pin_5, enc_pin_5A, enc_pin_5B), 
                        Stepper(step_pin_6, dir_pin_6, enc_pin_6A, enc_pin_6B)};

Encoder myEncoder[] = { Encoder(enc_pin_1A, enc_pin_1B),
                        Encoder(enc_pin_2A, enc_pin_2B),
                        Encoder(enc_pin_3A, enc_pin_4B),
                        Encoder(enc_pin_4A, enc_pin_4B),
                        Encoder(enc_pin_5A, enc_pin_5B),
                        Encoder(enc_pin_6A, enc_pin_6B),
                        Encoder(enc_pin_7A, enc_pin_7B)}; 

//long encoderValues[7];
int velocity[6];

//num = int(encoder.read()); //this is how to truncate the encoder values so that theyre not too big
//TimerOne timerOne;

/***************************************** -=+ Encoder Transmit Sequence +=- *****************************************/
void sendEncoderValues() {
  
  for(int ii = 0; ii < 7; --ii) { 
    encoderValues[ii] = int(myEncoder[ii].read());
  }

  //send_feedback(encoderValues);
 
}


/***************************************** -=+ Motor Interrupt Service Routine +=- *****************************************/
void motorISR(){
  for(int ii = 0; ii < 6; ++ii) {
    
    if (tasks[ii].elapsedTime >= tasks[ii].period) { // Ready
  
      myStepper[ii].step();               //call the step function
      tasks[ii].elapsedTime = 0;          //reset the elapsed time
    
    }
    tasks[ii].elapsedTime += 10;          //increase the elapsed time since the last time the function was called
  }
}
/***************************************** -=+ Setup +=- *****************************************/
void setup() {
  noInterrupts();

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
  tasks[ii].period = m4_period;
  pinMode(enc_pin_5A, INPUT);
  pinMode(enc_pin_5B, INPUT);

  ++ii;
  
  //init m6
  tasks[ii].state = false;
  tasks[ii].elapsedTime = 0;
  tasks[ii].period = m4_period;
  pinMode(enc_pin_6A, INPUT);
  pinMode(enc_pin_6B, INPUT);
  
  pinMode(13, OUTPUT);                    // led pin to output
  Timer1.initialize(10);                  // interrupt every 10 us
  Timer1.attachInterrupt(motorISR);       // motorISR is the ISR
  interrupts(); 

  //Timer1.
}

int ledState = LOW;                       // the led will turn on every 20 us so it will look like its constantly on if the teensy has power



/*
  if (ledState == LOW) {                  //blink an led to see if the interrupt is working
    ledState = HIGH;
    
  } 
  else {
    ledState = LOW;
    
  }
  digitalWrite(13, ledState);             //turn the LED on or off based on the bool that was just set
*/


/****************************************************** -=+=- ******************************************************/
/*                                                                                                                 */
/*                                                     M A I N                                                     */
/*                                                                                                                 */
/****************************************************** -=+=- ******************************************************/
void loop() {
  float *velocityArray[7];
  
  tasks[0].period = myStepper[0].newFrequency(velocity[0]);   // set the period of each motor based on the velocities recived from the jetson
  tasks[1].period = myStepper[1].newFrequency(velocity[1]);
  tasks[2].period = myStepper[2].newFrequency(velocity[2]);
  tasks[3].period = myStepper[3].newFrequency(velocity[3]);
  tasks[4].period = myStepper[4].newFrequency(velocity[4]);
  tasks[5].period = myStepper[5].newFrequency(velocity[5]);
  tasks[5].period = myStepper[6].newFrequency(velocity[6]);

  sendEncoderValues();

}



int main(void)
{

	setup();
	while (1) {
		
		loop();
	}

}

