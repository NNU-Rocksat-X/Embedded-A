
#include "comm.h"

static_assert(sizeof(CMDPacket) < BUFFER_SIZE, "Buffer too small.");

static uint32_t sequence = 0;
/////////////////////////////////////////////////////////////
//                      STATIC FUNCTIONS
/////////////////////////////////////////////////////////////
static void memcopy(void* dest, const void* src, int n)
{
    char *cdest = (char*)dest;
    char *csrc = (char*)src;

    for (int i = 0; i < n; i++)
    {
        cdest[i] = csrc[i];
    }
}

/////////////////////////////////////////////////////////////
//                     GLOBAL FUNCTIONS
/////////////////////////////////////////////////////////////

void setup_comm(void)
{
    hw_serial.begin(BAUD_RATE);
}

uint8_t get_cmd(float* cmd, bool* led_value)
{
    if(hw_serial.available())
    {
        char buffer[BUFFER_SIZE];
        CMDPacket rx;

        hw_serial.readBytes(buffer, sizeof(CMDPacket));

        // check crc
        memcopy(&rx, &buffer[0], sizeof(CMDPacket));
        uint16_t crc = crc16((unsigned char*)&rx, sizeof(CMDPacket) - 2); // // Subtract 2 so the crc is not calculated over the crc
	    *led_value = (bool)rx.led;

        if (crc != rx.crc)
        {
            return 1;
        } else {
            sequence = rx.seq;
            memcopy(cmd, &rx.joint_velocity_cmd[0], sizeof(float) * NUM_JOINTS);
	    
            return 2;
        }
    } else {
        return 0;
    }
}

void send_feedback(uint32_t* enc_steps)
{
    RESPacket tx;
    tx.seq = sequence;
    memcopy(&tx.joint_step_position[0], enc_steps, sizeof(uint32_t) * NUM_JOINTS);
    tx.crc = crc16((unsigned char*)&tx, sizeof(RESPacket) - 2);
    hw_serial.write((char*)&tx, sizeof(CMDPacket));
}
