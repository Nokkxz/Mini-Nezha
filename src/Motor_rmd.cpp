#include "Motor_rmd.hpp"
static uint8_t recv_data[8];

namespace rmd
{
    uint8_t CLOCKWISE = 0x00;
    uint8_t CTCLOCKWISE = 0x01;
}

Motor_rmd::Motor_rmd(){}

Motor_rmd::Motor_rmd(Can can)
{
    this->can = can;
    while(0==this->get_error_state());
    while(0==this->get_motor_state());
    while(0==this->get_pid());
    while(0==this->get_angle_singleloop());
    while(0==this->get_angle_multiloop());
    printf("motor ready\n\n");
}

Motor_rmd::~Motor_rmd(){}

// TODO
double Motor_rmd::get_torque()
{
    double torque;
    this->get_motor_state();
    torque = this->current;
    return torque;
}

double Motor_rmd::get_speed()
{
    double speed;
    this->get_motor_state();
    speed = this->speed;
    speed /= gear_ratio;
    return speed;
}

double Motor_rmd::get_angle()
{
    double angle;
    this->get_angle_multiloop();
    angle = this->angle_multiloop;
    angle /= gear_ratio*100;
    return angle;
}

// TODO
int Motor_rmd::set_torque(double torque)
{
    int16_t current;
    current = torque;
    this->current_control(current);
    return 1;
}

int Motor_rmd::set_speed(double speed)
{
    speed *= gear_ratio*100;
    this->speed_control(speed);
}

int Motor_rmd::set_angle(double angle, double speed)
{
    speed *= gear_ratio;
    angle *= gear_ratio*100;
    this->angle_multiloop_control(speed, angle);
    return 1;
}

int Motor_rmd::set_angle(double angle)
{
    angle *= gear_ratio*100;
    this->angle_multiloop_control(angle);
    return 1;
}

int Motor_rmd::get_pid()
{
    uint8_t can_cmd = 0x30;
    uint8_t can_data[8] = {can_cmd,0,0,0,0,0,0,0};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        angle_kp = recv_data[2];
        angle_ki = recv_data[3];
        speed_kp = recv_data[4];
        speed_ki = recv_data[5];
        current_kp = recv_data[6];
        current_ki = recv_data[7];
        return 1;
    }
    else
    {
        return 0;
    }
    
}

int Motor_rmd::set_pid(uint8_t angle_kp_set, uint8_t angle_ki_set,
                        uint8_t speed_kp_set, uint8_t speed_ki_set,
                        uint8_t current_kp_set, uint8_t current_ki_set)
{
    uint8_t can_cmd = 0x31;
    uint8_t can_data[8] = {can_cmd,0,angle_kp_set,angle_ki_set,
                                speed_kp_set,speed_ki_set,
                                current_kp_set,current_ki_set};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        angle_kp = angle_kp_set;
        angle_ki = angle_ki_set;
        speed_kp = speed_kp_set;
        speed_ki = speed_ki_set;
        current_kp = current_kp_set;
        current_ki = current_ki_set;
        return 1;
    }
    else
    {
        return 0;
    }
}

int Motor_rmd::set_pid_rom(uint8_t angle_kp_set, uint8_t angle_ki_set,
                        uint8_t speed_kp_set, uint8_t speed_ki_set,
                        uint8_t current_kp_set, uint8_t current_ki_set)
{
    uint8_t can_cmd = 0x32;
    uint8_t can_data[8] = {can_cmd,0,angle_kp_set,angle_ki_set,
                                speed_kp_set,speed_ki_set,
                                current_kp_set,current_ki_set};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int Motor_rmd::set_encoder_offset_rom()
{
    uint8_t can_cmd = 0x19;
    uint8_t can_data[8] = {can_cmd,0,0,0,0,0,0,0};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        encoder_offset = (uint16_t)recv_data[6]+((uint16_t)recv_data[7]<<8);
        return 1;
    }
    else
    {
        return 0;
    }
}

int Motor_rmd::get_angle_multiloop()
{
    uint8_t can_cmd = 0x92;
    uint8_t can_data[8] = {can_cmd,0,0,0,0,0,0,0};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        angle_multiloop = 0;
        for(int i=1; i<8; i++)
        {
            angle_multiloop += (uint64_t)recv_data[i]<<(i*8-8);
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

int Motor_rmd::get_angle_singleloop()
{
    uint8_t can_cmd = 0x94;
    uint8_t can_data[8] = {can_cmd,0,0,0,0,0,0,0};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        angle_singleloop = (uint16_t)recv_data[6]+((uint16_t)recv_data[7]<<8);
        return 1;
    }
    else
    {
        return 0;
    }
}

int Motor_rmd::get_error_state()
{
    uint8_t can_cmd = 0x9A;
    uint8_t can_data[8] = {can_cmd,0,0,0,0,0,0,0};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        temperature = recv_data[1];
        voltage = (uint16_t)recv_data[3]+((uint16_t)recv_data[4]<<8);
        error_state = recv_data[7];
        return 1;
    }
    else
    {
        return 0;
    }
}

int Motor_rmd::get_motor_state()
{
    uint8_t can_cmd = 0x9C;
    uint8_t can_data[8] = {can_cmd,0,0,0,0,0,0,0};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        temperature = recv_data[1];
        current = (uint16_t)recv_data[2]+((uint16_t)recv_data[3]<<8);
        speed = (uint16_t)recv_data[4]+((uint16_t)recv_data[5]<<8);
        encoder = (uint16_t)recv_data[6]+((uint16_t)recv_data[7]<<8);
        return 1;
    }
    else
    {
        return 0;
    }
}

int Motor_rmd::shut_motor()
{
    uint8_t can_cmd = 0x80;
    uint8_t can_data[8] = {can_cmd,0,0,0,0,0,0,0};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int Motor_rmd::pause_motor()
{
    uint8_t can_cmd = 0x81;
    uint8_t can_data[8] = {can_cmd,0,0,0,0,0,0,0};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int Motor_rmd::resume_motor()
{
    uint8_t can_cmd = 0x88;
    uint8_t can_data[8] = {can_cmd,0,0,0,0,0,0,0};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int Motor_rmd::current_control(int16_t current_set)
{
    // current_set = current_set*2000/32;
    uint8_t can_cmd = 0xA1;
    uint8_t can_data[8] = {can_cmd,0,0,0,
                        (uint8_t)current_set,(uint8_t)(current_set>>8),0,0};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        temperature = recv_data[1];
        current = (uint16_t)recv_data[2]+((uint16_t)recv_data[3]<<8);
        speed = (uint16_t)recv_data[4]+((uint16_t)recv_data[5]<<8);
        encoder = (uint16_t)recv_data[6]+((uint16_t)recv_data[7]<<8);
        return 1;
    }
    else
    {
        return 0;
    }
}

int Motor_rmd::speed_control(int32_t speed_set)
{
    uint8_t can_cmd = 0xA2;
    uint8_t can_data[8] = {can_cmd,0,0,0,
                        (uint8_t)speed_set,(uint8_t)(speed_set>>8),
                        (uint8_t)(speed_set>>16),(uint8_t)(speed_set>>24)};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        temperature = recv_data[1];
        current = (uint16_t)recv_data[2]+((uint16_t)recv_data[3]<<8);
        speed = (uint16_t)recv_data[4]+((uint16_t)recv_data[5]<<8);
        encoder = (uint16_t)recv_data[6]+((uint16_t)recv_data[7]<<8);
        return 1;
    }
    else
    {
        return 0;
    }
}

int Motor_rmd::angle_multiloop_control(int32_t angle_multiloop_set)
{
    uint8_t can_cmd = 0xA3;
    uint8_t can_data[8] = {can_cmd,0,0,0,
                        (uint8_t)angle_multiloop_set,(uint8_t)(angle_multiloop_set>>8),
                        (uint8_t)(angle_multiloop_set>>16),(uint8_t)(angle_multiloop_set>>24)};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        temperature = recv_data[1];
        current = (uint16_t)recv_data[2]+((uint16_t)recv_data[3]<<8);
        speed = (uint16_t)recv_data[4]+((uint16_t)recv_data[5]<<8);
        encoder = (uint16_t)recv_data[6]+((uint16_t)recv_data[7]<<8);
        return 1;
    }
    else
    {
        return 0;
    }
}

int Motor_rmd::angle_multiloop_control(uint16_t max_speed, int32_t angle_multiloop_set)
{
    uint8_t can_cmd = 0xA4;
    uint8_t can_data[8] = {can_cmd,0,(uint8_t)max_speed,(uint8_t)(max_speed>>8),
                        (uint8_t)angle_multiloop_set,(uint8_t)(angle_multiloop_set>>8),
                        (uint8_t)(angle_multiloop_set>>16),(uint8_t)(angle_multiloop_set>>24)};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        temperature = recv_data[1];
        current = (uint16_t)recv_data[2]+((uint16_t)recv_data[3]<<8);
        speed = (uint16_t)recv_data[4]+((uint16_t)recv_data[5]<<8);
        encoder = (uint16_t)recv_data[6]+((uint16_t)recv_data[7]<<8);
        return 1;
    }
    else
    {
        return 0;
    }
}

int Motor_rmd::angle_singleloop_control(uint8_t direction, uint16_t angle_singleloop_set)
{
    uint8_t can_cmd = 0xA5;
    uint8_t can_data[8] = {can_cmd,direction,0,0,
                        (uint8_t)angle_singleloop_set,(uint8_t)(angle_singleloop_set>>8),0,0};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        temperature = recv_data[1];
        current = (uint16_t)recv_data[2]+((uint16_t)recv_data[3]<<8);
        speed = (uint16_t)recv_data[4]+((uint16_t)recv_data[5]<<8);
        encoder = (uint16_t)recv_data[6]+((uint16_t)recv_data[7]<<8);
        return 1;
    }
    else
    {
        return 0;
    }
}

int Motor_rmd::angle_singleloop_control(uint8_t direction, uint16_t max_speed, uint16_t angle_singleloop_set)
{
    uint8_t can_cmd = 0xA6;
    
    uint8_t can_data[8] = {can_cmd,direction,(uint8_t)max_speed,(uint8_t)(max_speed>>8),
                        (uint8_t)angle_singleloop_set,(uint8_t)(angle_singleloop_set>>8),0,0};
    can.send(can_data);
    can.recv(recv_data);
    if(recv_data[0]==can_cmd)
    {
        temperature = recv_data[1];
        current = (uint16_t)recv_data[2]+((uint16_t)recv_data[3]<<8);
        speed = (uint16_t)recv_data[4]+((uint16_t)recv_data[5]<<8);
        encoder = (uint16_t)recv_data[6]+((uint16_t)recv_data[7]<<8);
        return 1;
    }
    else
    {
        return 0;
    }
}

int Motor_rmd::multimotor_current_control(int16_t current1, int16_t current2, int16_t current3, int16_t current4)
{

}