#ifndef __PID_HPP__
#define __PID_HPP__

class Pid
{
public:
    float kp, ki, kd;
    float e_buf[3];
    float p_out;
    float i_out;
    float d_out;
    float out;
    float maxout;

    Pid();
    Pid(float kp, float ki, float kd, float maxout);
    float update(float ref, float fdb);
    void clear();
};

#endif