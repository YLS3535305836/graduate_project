#ifndef __6050_PID_H__
#define __6050_PID_H__

#include "main.h"

#define Limitmax 0
#define Limitmin 0

typedef struct imuPid
{
    float P;
    float I;
    float D;
    float POut;
    float IOut;
    float DOut;
    int16_t Output;
    int16_t Error;
    int16_t LastError;
    int16_t PrevError;
    int16_t ErrorSum;

}imuPIDTypedef;






void IMUParameterInit(imuPIDTypedef *pidPara);
int16_t PIDCaculate_Position(imuPIDTypedef *pid, int16_t TargetSpeed, int16_t NowSpeed);
int16_t PIDCaculate_PositionTest(float p, float i, float d, imuPIDTypedef *pid, int16_t TargetSpeed, int16_t NowSpeed);

#endif
