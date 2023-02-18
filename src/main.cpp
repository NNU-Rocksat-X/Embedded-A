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
	bool led_power;
	
	get_cmd(&velocity_cmds[0], &led_power);


        //digitalWrite(13, HIGH);
	delay(100);
	digitalWrite(13, led_power);
	

	send_feedback(&encoder_positions[0]);
	delay(100);
}

int main(void)
{
	while (1) {
		loop();
	}

}

