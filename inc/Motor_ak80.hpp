#ifndef __MOTOR_AK80_HPP__
#define __MOTOR_AK80_HPP__
#include "Can.hpp"
#include "Motor.hpp"
#include <thread>
#include <mutex>
#include <fstream>
#include <iostream>
#include <string>
#include "math_ops.hpp"

class Motor_ak80 : public Motor
{
private:
    float gear_ratio = 1;

public:
    int uart;
    uint8_t can_port;   // 0/1
    uint32_t can_id;    // 1/2/3/4

    float p=0;     // -95.5f~95.5f (rad)
    int16_t v=0;    // rad/s
    int16_t t=0;    // Nm

    Motor_ak80();
    Motor_ak80(int uart, uint8_t can_port, uint32_t can_id);
    ~Motor_ak80();

    void EnterMotorMode();
    void ExitMotorMode();
    double GetTorque();    // Nm
    double GetSpeed();     // rad/s
    double GetAngle();     // -95.5f~95.5f (rad)
    int SetTorque(double torque);   // Nm
    int SetSpeed(double speed);     // rad/s
    int SetAngle(double angle);     // -95.5f~95.5f (rad)
    void SendZeroCmd();
};

void Motor_ak80_start_update(Motor_ak80* m1, Motor_ak80* m2);


#endif