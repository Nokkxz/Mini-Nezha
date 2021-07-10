#ifndef __MOTOR_HPP__
#define __MOTOR_HPP__

class Motor
{
public:
    virtual double GetTorque(){}
    virtual double GetSpeed(){}
    virtual double GetAngle(){}

    virtual int SetTorque(double torque){}
    virtual int SetSpeed(double speed){}
    virtual int SetAngle(double angle){}
    virtual int SetAngle(double angle, double maxSpeed){}
};

#endif