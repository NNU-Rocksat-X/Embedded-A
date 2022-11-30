#ifndef COMM_H_
#define COMM_H_

#include "Arduino.h"

#define BAUD_RATE (9600)
#define hw_serial Serial

void memcopy(void* dest, const void* src, int n)
{
    char *cdest = (char*)dest;
    char *csrc = (char*)src;

    for (int i = 0; i < n; i++)
    {
        cdest[i] = csrc[i];
    }
}

typedef struct packet
{
    uint8_t led;
    uint8_t reserved[3];
    float cmd;
} Packet;

void setup_comm()
{
    hw_serial.begin(BAUD_RATE);
}


void get_data(Packet* rx)
{
    char buffer[10];
    if(hw_serial.available())
    {
        hw_serial.readBytes(buffer, sizeof(Packet));
        memcopy(rx, &buffer[0], sizeof(Packet));
    }
}

void write_data(Packet* tx)
{
    hw_serial.write((char*)tx, sizeof(Packet));
}



#endif /* COMM_H_ */
