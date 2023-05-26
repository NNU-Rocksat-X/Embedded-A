#ifndef COMM_H_
#define COMM_H_

#include "Arduino.h"
#include <apogee_robot_core/CommonComms.h>
#include <assert.h>

#define BAUD_RATE (9600)

// hw_serial == Serial: Communicate over micro-usb
// hw_serial == Serial1: Communicate over tx and rx pins
#define hw_serial (Serial)
#define BUFFER_SIZE (50)

/* @Brief - Performs required steps to setup the communication. Call during main setup
*/
void setup_comm(void);

/* @Brief - Get_data checks for a command from the jetson and places the command for each
*           joint into the cmd parameter
*
* @param[out] cmd - An array of size NUM_JOINTS (from CommonComms.h)
*                   get_data will place the cmd from the jetson for each joint in the array
*
* Returns 0 - if no data available
* Returns 1 - if there was a transmission error
* Returns 2 - if transmission was successful
*/
uint8_t get_cmd(int32_t* cmd, bool* led_value);

/* @Brief - Sends encoder data to the jetson
*
* @param[in] enc_steps - An array of size NUM_JOINTS (from CommonComms.h)
*                        containing the encoder step for each joint.
*                        The jetson calculates the degree of the joint
*
* Return N/A
*/
void send_feedback(int32_t* enc_steps);

#endif /* COMM_H_ */
