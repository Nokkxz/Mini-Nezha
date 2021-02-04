#ifndef __STANDSTATE_HPP__
#define __STANDSTATE_HPP__
#include "State.hpp"
#include "Robot.hpp"

class StandState : public State
{
public:
    StandState(){};
    ~StandState(){};

    void Enter(Robot* robot);
    void Execute(Robot* robot);
    void Exit(Robot* robot);
};

#endif