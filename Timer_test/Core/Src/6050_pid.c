#include "6050_pid.h"

void IMUParameterInit(imuPIDTypedef *pidPara)
{
    pidPara->P = 40;
    pidPara->I = 30;
    pidPara->D = 0;
    pidPara->POut= 0;
    pidPara->IOut= 0;
    pidPara->DOut= 0;
    pidPara->Output= 0;
    pidPara->LastError = 0;
}

int16_t PIDCaculate_Position(imuPIDTypedef *pid, int16_t TargetSpeed, int16_t NowSpeed)
{
    pid->Error = TargetSpeed - NowSpeed;

    pid->POut = pid->P * pid->Error;
    pid->IOut = pid->I * pid->ErrorSum;
    pid->DOut = pid->D * (pid->LastError - pid->Error);

    pid->ErrorSum = pid->LastError + pid->Error;
    pid->LastError = pid->Error;

    pid->Output = pid->POut + pid->IOut + pid->DOut;

    if(pid->Output > Limitmax)
        pid->Output = Limitmax;
    else if(pid->Output < Limitmin)
        pid->Output = Limitmin;

    return (pid->Output);
}

int16_t PIDCaculate_PositionTest(float p, float i, float d, imuPIDTypedef *pid, int16_t TargetSpeed, int16_t NowSpeed)
{
    pid->Error = TargetSpeed - NowSpeed;

    pid->POut = p * pid->Error;
    pid->IOut = i * pid->ErrorSum;
    pid->DOut = d * (pid->Error - pid->LastError);

    pid->ErrorSum = pid->LastError + pid->Error;
    pid->LastError = pid->Error;

    pid->Output = pid->POut + pid->IOut + pid->DOut;

    if(pid->Output > Limitmax)
        pid->Output = Limitmax;
    else if(pid->Output < Limitmin)
        pid->Output = Limitmin;

    return (pid->Output);
}


