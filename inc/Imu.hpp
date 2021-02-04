#ifndef __IMU_HPP__
#define __IMU_HPP__
#include "uart.hpp"
#include <thread>
#include <mutex>
#include <fstream>
#include <iostream>
#include <string>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<assert.h>
#include<termios.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
#include<sys/types.h>
#include<errno.h>

class Imu
{
public:
    int uart;
    double angle_X, angle_Y, angle_Z,
        angularSpeed_X, angularSpeed_Y, angularSpeed_Z,
        acceleration_X, acceleration_Y, acceleration_Z;

    Imu();
    Imu(int uart);
    ~Imu();

    double GetAngle_X(){return angle_X;};     // degree
    double GetAngle_Y(){return angle_Y;};
    double GetAngle_Z(){return angle_Z;};
    double GetAngularSpeed_X(){return angularSpeed_X;}; // degree/s
    double GetAngularSpeed_Y(){return angularSpeed_Y;};
    double GetAngularSpeed_Z(){return angularSpeed_Z;};
    double GetAcceleration_X(){return acceleration_X;}; // m/s^2
    double GetAcceleration_Y(){return acceleration_Y;};
    double GetAcceleration_Z(){return acceleration_Z;};
};

void imu_start_update(Imu* imu);

#endif