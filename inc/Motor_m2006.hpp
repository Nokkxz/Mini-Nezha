#ifndef __MOTOR_M2006_HPP__
#define __MOTOR_M2006_HPP__
#include "Can.hpp"
#include "Motor.hpp"
#include <thread>
#include <mutex>
#include <fstream>
#include <iostream>
#include <string>

class Motor_m2006 : public Motor
{
private:
    double gear_ratio = 1;
public:
    int uart;
    uint8_t can_port;   // 0/1
    uint32_t can_id;    // 1/2/3/4

    double angle=0;     // 0~360
    int16_t speed=0;    // rpm
    int16_t current=0;

    Motor_m2006();
    Motor_m2006(int uart, uint8_t can_port, uint32_t can_id);
    ~Motor_m2006();

    double GetTorque();    // Nm //TODO
    double GetSpeed();     // dps //TODO
    double GetAngle();     // 0~360
    int SetTorque(double torque);  // Nm //TODO
};

void m2006_start_update(Motor_m2006* m1, Motor_m2006* m2);
void m2006_set_current(int16_t c, Motor_m2006* m);
void m2006_set_current(int16_t c1, int16_t c2, Motor_m2006* m1, Motor_m2006* m2);
// current: -10000~10000 (-10A~10A)

#endif