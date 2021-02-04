#include "Robot.hpp"

Robot::Robot()
{
}

Robot::~Robot()
{
}

Robot::Robot(Imu imu, Motor hip_L, Motor hip_R,
            Motor knee_L, Motor knee_R,
            Motor wheel_L, Motor wheel_R):
            imu(imu), hip_L(hip_L), hip_R(hip_R), 
            knee_L(knee_L), knee_R(knee_R),
            wheel_L(wheel_L), wheel_R(wheel_R),
            pCurState(nullptr)
{
}

void Robot::SetCurState(State* pState)
{
    pCurState = pState;
}

void Robot::Update()
{
    pCurState->Execute(this);
}

void Robot::ChangeState(State* pNewState)
{
    pCurState->Exit(this);
    pNewState->Enter(this);
    delete pCurState;
    pCurState = pNewState;
}