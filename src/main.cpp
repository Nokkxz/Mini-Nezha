#include <thread>
#include <mutex>
#include <time.h>
#include "uart.hpp"

int test_send_cnt = 0;
int test_recv_cnt = 0;
int test_send_len = 0;
int test_recv_len = 0;

int main(int argc, char const *argv[])
{
    int uart = uart_init(BAUD, UART_PATH);

    CAN_info info_recv, info_send;

    info_send.CAN_id = 0x141;
    info_send.CAN_port = 1;
    uint8_t data[8] = {0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    for(int i=0; i<8; i++)
        info_send.CAN_data[i] = data[i];

    std::thread uart_recv_t(uart_recv_thread, uart, info_recv);
    uart_recv_t.detach();
    
    while(1)
    {
        test_send_cnt++;
        uart_send(uart, info_send);

        usleep(1000);
    }
}
