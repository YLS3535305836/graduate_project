#ifndef __FILTER_H
#define __FILTER_H
#include "sys.h"

extern float angle, angle_dot;

void Kalman_Filter(float Accel,float Gyro);

#endif
