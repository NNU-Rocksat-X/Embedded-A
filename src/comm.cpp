
#include "comm.h"

static_assert(sizeof(CMDPacket) < BUFFER_SIZE, "Buffer too small.");

static uint32_t sequence = 0;
uint16_t byte_count = 0;
uint16_t rcv_crc = 0;
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

    hw_serial.clear();
    hw_serial.flush();
}

uint8_t get_cmd(int32_t* cmd, bool* led_value)
{
    char buffer[BUFFER_SIZE];
    int bytes_recieved = 0;
    CMDPacket rx;
    if(hw_serial.available())
    {
        while(hw_serial.available())
        {
            hw_serial.readBytes(&buffer[bytes_recieved], 1);
            bytes_recieved++;
            if (buffer[0] == 0X55)
            {
                if (bytes_recieved > 1)
                {
                    if (buffer[1] == 0x70)
                    {
                        if (bytes_recieved == sizeof(CMDPacket))
                        {
                            // check crc
                            memcopy(&rx, &buffer[0], sizeof(CMDPacket));
                            uint16_t crc = crc16((unsigned char*)&rx, (int)sizeof(CMDPacket) - 4); // trying 2 instead of 4 // Subtract 2 so the crc is not calculated over the crc
                            rcv_crc = bytes_recieved;
                            *led_value = (bool)rx.led;

                            if (crc != rx.crc)
                            {
                                return 1;
                            } else {
                                sequence = rx.seq;
                                memcopy(cmd, &rx.joint_velocity_cmd[0], sizeof(float) * NUM_JOINTS);
                            
                                return 2;
                            }
                        }
                    } else {
                        memset(buffer, 0, BUFFER_SIZE);
                        bytes_recieved = 0;
                        return 1;
                    }
                }
            } else {
                memset(buffer, 0, BUFFER_SIZE);
                bytes_recieved = 0;
            }

        }
        return 0;
    } else {
        return 0;
    }
}

void send_feedback(int32_t* enc_steps)
{
    RESPacket tx;
    tx.seq = sequence;
    memcopy(&tx.joint_step_position[0], enc_steps, sizeof(uint32_t) * NUM_JOINTS);
    tx.crc = crc16((unsigned char*)&tx, sizeof(RESPacket) - 4); // trying 4 instead of 2
    hw_serial.write((char*)&tx, sizeof(RESPacket)); 
}
