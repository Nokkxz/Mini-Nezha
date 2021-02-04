#ifndef __STATE_HPP__
#define __STATE_HPP__

class Robot;

class State
{
public:
    virtual void Enter(Robot* robot) = 0;
    virtual void Execute(Robot* robot) = 0;
    virtual void Exit(Robot* robot) = 0;
};

#endif