#ifndef __MOTOR_RMD_HPP__
#define __MOTOR_RMD_HPP__
#include "Can.hpp"
#include "Motor.hpp"

class Motor_rmd : public Motor
{
private:
    Can* can;
    int gear_ratio = 6;     // x8
    int64_t angle_offset = 0;
public:
    uint8_t angle_kp = 0;
    uint8_t angle_ki = 0;
    uint8_t speed_kp = 0;
    uint8_t speed_ki = 0;
    uint8_t current_kp = 0;
    uint8_t current_ki = 0;

    uint16_t encoder_offset = 0;

    int64_t angle_multiloop = 0;   // +: clockwise (0.01 degree/LSB)
    uint16_t angle_singleloop = 0;  // 0 ~ 35999 (0.01 degree/LSB)

    int16_t current = 0;    // -2048 ~ 2048 : -33A ~ 33A
    int16_t speed = 0;      // 1dps/LSB
    uint16_t encoder = 0;   // 0 ~ 16383 (14bit)

    int8_t temperature = 0;     // 1 C/LSB
    uint16_t voltage = 0;       // 0.1 V/LSB
    uint8_t error_state = 0;

    Motor_rmd();
    Motor_rmd(int uart, uint8_t can_port, uint32_t can_id);
    ~Motor_rmd();

    double GetTorque();    // Nm //TODO
    double GetSpeed();     // dps
    double GetAngle();     // d
    int SetTorque(double torque);      // Nm //TODO
    int SetSpeed(double speed);        // dps
    int SetAngle(double angle, double maxSpeed);  // d,dps
    int SetAngle(double angle);        // d

    int get_pid();
    int set_pid(uint8_t angle_kp_set, uint8_t angle_ki_set,
                    uint8_t speed_kp_set, uint8_t speed_ki_set,
                    uint8_t current_kp_set, uint8_t current_ki_set);
    int set_pid_rom(uint8_t angle_kp_set, uint8_t angle_ki_set,
                    uint8_t speed_kp_set, uint8_t speed_ki_set,
                    uint8_t current_kp_set, uint8_t current_ki_set);
    int set_encoder_offset_rom();

    int get_angle_multiloop();
    int get_angle_singleloop();
    int get_error_state();
    int get_motor_state();

    int shut_motor();
    int pause_motor();
    int resume_motor();

    int current_control(int16_t current_set);   // -32A ~ 32A
    int speed_control(int32_t speed_set);
    int angle_multiloop_control(int32_t angle_multiloop_set);
    int angle_multiloop_control(uint16_t max_speed, int32_t angle_multiloop_set);
    int angle_singleloop_control(uint8_t direction, uint16_t angle_singleloop_set);
    int angle_singleloop_control(uint8_t direction, uint16_t max_speed, uint16_t angle_singleloop_set);
    int multimotor_current_control(int16_t current1, int16_t current2, int16_t current3, int16_t current4);
};

#endif