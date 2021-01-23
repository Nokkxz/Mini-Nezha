#ifndef __CAN_HPP__
#define __CAN_HPP__
#include "uart.hpp"

#define CAN_PRINT 0
#define CAN_SENDBACK 0

class Can
{
private:
    uint8_t send_buf[19];
    uint8_t recv_buf[190];

    int recv_parse(uint8_t* recv_buf, uint8_t* recv_data);

public:
    int uart;
    uint8_t can_port;   // 0/1
    uint32_t can_id;

    Can();
    Can(int uart, uint8_t can_port, uint32_t can_id);
    ~Can();
    
    int send(uint8_t can_data[8]);
    int recv(uint8_t recv_data[8]);
};

#endif