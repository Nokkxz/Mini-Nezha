#ifndef __UART_HPP__
#define __UART_HPP__
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <termios.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <stdint.h>

int uart_init(int baud, char* uart_path);

#endif