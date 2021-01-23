#include "Motor_m2006.hpp"
#include <fstream>
#include <iostream>
#include <string>

static std::mutex uart_mut;
static uint8_t s_buf[19];
static uint8_t r_buf[19000];

Motor_m2006::Motor_m2006(){}

Motor_m2006::Motor_m2006(int uart, uint8_t can_port, uint32_t can_id)
{
    this->uart = uart;
    this->can_port = can_port;
    this->can_id = can_id;
}

Motor_m2006::~Motor_m2006(){}

//TODO
double Motor_m2006::get_torque()
{
    double torque;
    torque = this->current;
    return torque;
}

//TODO
double Motor_m2006::get_speed()
{
    double speed;
    speed = this->speed;
    speed *= 6;
    return speed;
}

double Motor_m2006::get_angle()
{
    return this->angle;
}

//TODO
int Motor_m2006::set_torque(double torque)
{
    int16_t current;
    current = torque;
    m2006_set_current(current, this);
}

void uart_send_thread(int uart)
{
    int s_len;
    uart_mut.lock();
    s_len = write(uart, s_buf, sizeof(s_buf));
    uart_mut.unlock();
}

void can_send(int uart, uint32_t can_id, uint8_t can_port, uint8_t can_data[8])
{
    s_buf[0] = 0x55;                // frame header
    s_buf[1] = 0x00;                // cmd
    s_buf[2] = 0x00;                // subcmd
    s_buf[3] = 0x13;                // frame length
    for(int i=0; i<4; i++)          // CAN id
    {
        s_buf[i+4] = (uint8_t)(can_id>>(i*8));
    }
    s_buf[8] = can_port;            // CAN port
    s_buf[9] = 0x08;                // data length
    for(int i=0; i<8; i++)          // data
    {
        s_buf[i+10] = can_data[i];
    }
    s_buf[18] = 0xaa;               // frame end

    std::thread uart_send_t(uart_send_thread, uart);
    uart_send_t.detach();
}

void m2006_set_current(int16_t c, Motor_m2006* m)
{
    int uart = m->uart;
    uint32_t can_id = 0x200;
    uint8_t can_port = m->can_port;
    uint8_t can_data[8] = {0};
    can_data[(m->can_id-1)*2+1] = (uint8_t)c;
    can_data[(m->can_id-1)*2] = (uint8_t)(c>>8);
    can_send(uart, can_id, can_port, can_data);
}

void m2006_set_current(int16_t c1, int16_t c2, Motor_m2006* m1, Motor_m2006* m2)
{
    int uart = m1->uart;
    uint32_t can_id = 0x200;
    uint8_t can_port = m1->can_port;
    uint8_t can_data[8] = {0};
    can_data[(m1->can_id-1)*2+1] = (uint8_t)c1;
    can_data[(m1->can_id-1)*2] = (uint8_t)(c1>>8);
    can_data[(m2->can_id-1)*2+1] = (uint8_t)c2;
    can_data[(m2->can_id-1)*2] = (uint8_t)(c2>>8);
    can_send(uart, can_id, can_port, can_data);
}

void can_recv(Motor_m2006* m1, Motor_m2006* m2)
{
    uint32_t can_id;
    uint8_t can_port;
    uint8_t can_data[8];
    if (0x55==r_buf[0])
    {
        switch (r_buf[1])
        {
        case 0x10:  // USB2CAN send back
            break;
        case 0x02:  // CAN receive
            can_port = r_buf[8];
            can_id = (uint32_t)r_buf[4]+((uint32_t)r_buf[5]<<8)
                        +((uint32_t)r_buf[6]<<16)+((uint32_t)r_buf[7]<<24);
            can_id -= 0x200;
            for(int i=0; i<8; i++)
            {
                can_data[i] = r_buf[i+10];
            }
            break;
        default:
            break;
        }
    }

    uint16_t angle = ((uint16_t)can_data[0]<<8) + can_data[1];
    int16_t speed = ((int16_t)can_data[2]<<8) + can_data[3];
    int16_t current = ((int16_t)can_data[4]<<8) + can_data[5];
    if(can_port == m1->can_port && can_id == m1->can_id)
    {
        m1->angle = (double)angle *360/8191 ;
        m1->speed = speed;
        m1->current = current;
    }
    else if(can_port == m2->can_port && can_id == m2->can_id)
    {
        m2->angle = (double)angle *360/8191 ;
        m2->speed = speed;
        m2->current = current;
    }
}

void uart_recv_thread(Motor_m2006* m1, Motor_m2006* m2)
{
    int uart = m1->uart;
    int r_len;

    while(1)
    {
        usleep(1);
        uart_mut.lock();
        r_len = read(uart, r_buf, sizeof(r_buf));
        uart_mut.unlock();
        if(r_len > 0)
        {
            can_recv(m1, m2);
        }
    }
}

void m2006_start_update(Motor_m2006* m1, Motor_m2006* m2)
{
    std::thread motor_recv_t(uart_recv_thread, m1, m2);
    motor_recv_t.detach();
}