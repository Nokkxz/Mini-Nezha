#ifndef __RESTSTATE_HPP__
#define __RESTSTATE_HPP__
#include "State.hpp"
#include "Robot.hpp"

class RestState : public State
{
public:
    RestState(){};
    ~RestState(){};

    void Enter(Robot* robot);
    void Execute(Robot* robot);
    void Exit(Robot* robot);
};

#endif