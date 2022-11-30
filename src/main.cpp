
#include <Arduino.h>
#include <comm.h> 

int main(void)
{

	pinMode(13, OUTPUT);
	Packet packet;
	packet.led = false;
	while (1) {

		delay(100);
		
		get_data(&packet);
		digitalWrite(13, packet.led);

		packet.cmd += 1;

		write_data(&packet);
	}

}

