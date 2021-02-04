#include "Pid.hpp"
#include <stdlib.h>

Pid::Pid()
{

}

Pid::Pid(float kp, float ki, float kd, float maxout)
{
    Clear();
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
    this->maxout = maxout;
}

float Pid::Update(float ref, float fdb)
{
    e_buf[2] = e_buf[1];
    e_buf[1] = e_buf[0];
    e_buf[0] = ref - fdb;

    p_out = e_buf[0] * kp;
    i_out += e_buf[0] * ki;
    d_out = (e_buf[0] - e_buf[1]) * kd;

    out = p_out + i_out + d_out;
    if(out>maxout) out = maxout;
    else if(out<-maxout) out = -maxout;

    return out;
}

void Pid::Clear()
{
    e_buf[0] = e_buf[1] = e_buf[2] = 0.0f;
    p_out = i_out = d_out = out = 0.0f;
    kp = ki = kd = maxout = 0.0f;
}