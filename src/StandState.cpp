#include "StandState.hpp"
#include "Pid.hpp"

static Pid pid = Pid(1, 0, 0, 10000);   //TODO

void StandState::Enter(Robot* robot)
{
    //TODO
    double hipAngle = 0;
    double kneeAngle = 0;
    robot->hip_L.SetAngle(hipAngle);
    robot->hip_R.SetAngle(hipAngle);
    robot->knee_L.SetAngle(kneeAngle);
    robot->knee_R.SetAngle(kneeAngle);
}

void StandState::Execute(Robot* robot)
{
    //TODO
    double angle_Y = robot->imu.GetAngle_Y();
    double torque;
    torque = pid.Update(0, angle_Y);
    robot->wheel_L.SetTorque(torque);
    robot->wheel_R.SetTorque(torque);
}

void StandState::Exit(Robot* robot)
{
    robot->wheel_L.SetTorque(0);
    robot->wheel_R.SetTorque(0);
}