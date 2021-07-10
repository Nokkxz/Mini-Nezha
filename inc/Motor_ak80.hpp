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
    double gear_ratio = 1;
public:
    int uart;
    uint8_t can_port;   // 0/1
    uint32_t can_id;    // 1/2/3/4

    double p=0;     // -95.5f~95.5f (rad)
    int16_t v=0;    // rad/s
    int16_t t=0;    // Nm

    Motor_ak80();
    Motor_ak80(int uart, uint8_t can_port, uint32_t can_id);
    ~Motor_ak80();

    double GetTorque();    // Nm
    double GetSpeed();     // rad/s
    double GetAngle();     // -95.5f~95.5f (rad)
    int SetTorque(double torque);  // Nm //TODO
};

void ak80_start_update(Motor_ak80* m1, Motor_ak80* m2);
// void m2006_set_current(int16_t c, Motor_m2006* m);
// void m2006_set_current(int16_t c1, int16_t c2, Motor_m2006* m1, Motor_m2006* m2);
// current: -10000~10000 (-10A~10A)

#endif