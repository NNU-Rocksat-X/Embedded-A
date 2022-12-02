#include <Arduino.h>
#include <comm.h> 

float velocity_cmds[NUM_JOINTS];
uint32_t encoder_positions[NUM_JOINTS];

void setup(void)
{
	pinMode(13, OUTPUT);
} 

void loop(void)
{
	
	get_cmd(&velocity_cmds[0]);



	send_feedback(&encoder_positions[0]);
	delay(100);
}

int main(void)
{
	while (1) {
		loop();
	}

}

