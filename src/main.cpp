#include <Arduino.h>
#include <comm.h> 

Packet packet;

void setup(void)
{
	pinMode(13, OUTPUT);
	packet.led = false;
} 

void loop(void)
{
	delay(100);
	
	get_data(&packet);
	digitalWrite(13, packet.led);

	packet.cmd += 1;

	write_data(&packet);
}

int main(void)
{
	while (1) {
		loop();
	}

}

