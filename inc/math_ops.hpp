#ifndef _MATH_OPS_HPP_
#define _MATH_OPS_HPP_

#define PI 3.14159265359f

#include "math.h"

float fmaxf2(float x, float y);
float fminf2(float x, float y);
float fmaxf3(float x, float y, float z);
float fminf3(float x, float y, float z);
int float_to_uint(float x, float x_min, float x_max, int bits);
float uint_to_float(int x_int, float x_min, float x_max, int bits);

#endif
