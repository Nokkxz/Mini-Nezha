#ifndef __UART_HPP__
#define __UART_HPP__
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <termios.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <errno.h>
#include <thread>
#include <mutex>

#define BAUD 115200
#define UART_PATH (char*)"/dev/ttyACM0"

typedef struct
{
    uint8_t CAN_cmd;
    uint8_t CAN_port;
    uint32_t CAN_id;
    uint8_t CAN_data[8];
}CAN_info;

int uart_init(int baud, char* uart_path);
void uart_recv_thread(int uart, CAN_info info);
void uart_send_thread(int uart, uint8_t* s_buf, int length);
void uart_send(int uart, CAN_info info);


#endif