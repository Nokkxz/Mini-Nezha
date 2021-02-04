#include "RestState.hpp"

void RestState::Enter(Robot* robot)
{

}

void RestState::Execute(Robot* robot)
{
    robot->hip_L.SetTorque(0);
    robot->hip_R.SetTorque(0);
    robot->knee_L.SetTorque(0);
    robot->knee_R.SetTorque(0);
    robot->wheel_L.SetTorque(0);
    robot->wheel_R.SetTorque(0);
}

void RestState::Exit(Robot* robot)
{

}