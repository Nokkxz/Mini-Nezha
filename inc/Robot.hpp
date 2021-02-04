#ifndef __ROBOT_HPP__
#define __ROBOT_HPP__
#include "Motor.hpp"
#include "Imu.hpp"
#include "State.hpp"

class Robot
{
private:
    State* pCurState;

public:
    Imu imu;
    Motor hip_L;
    Motor hip_R;
    Motor knee_L;
    Motor knee_R;
    Motor wheel_L;
    Motor wheel_R;
    
    Robot();
    Robot(Imu imu, Motor hip_L, Motor hip_R,
            Motor knee_L, Motor knee_R,
            Motor wheel_L, Motor wheel_R);
    ~Robot();

    void SetCurState(State* pState);
    void Update();
    void ChangeState(State* pNewState);
};

#endif