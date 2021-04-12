#include "pid.h"
#include "tim.h"

void PIDParameterInit(PIDTydef *pidPara)
{
    pidPara->P = 40;
    pidPara->I = 30;
    pidPara->D = 0;
    pidPara->POut= 0;
    pidPara->IOut= 0;
    pidPara->DOut= 0;
    pidPara->Output= 0;
    pidPara->SumOutput= 0;
    pidPara->LimitMax = 800;
    pidPara->LimitMin = -800;
    pidPara->LastError = 0;
}

int16_t PIDCaculate_Position(PIDTydef *pid, int16_t TargetSpeed, int16_t NowSpeed)
{
    pid->Error = TargetSpeed - NowSpeed;

    pid->POut = pid->P * pid->Error;
    pid->IOut = pid->I * pid->ErrorSum;
    pid->DOut = pid->D * (pid->LastError - pid->Error);

    pid->ErrorSum = pid->LastError + pid->Error;
    pid->LastError = pid->Error;

    pid->Output = pid->POut + pid->IOut + pid->DOut;

    if(pid->Output > pid->LimitMax)
        pid->Output = pid->LimitMax;
    else if(pid->Output < pid->LimitMin)
        pid->Output = pid->LimitMin;

    return (pid->Output+80);
}

int16_t PIDCaculate_Increment(PIDTydef *pid, int16_t TargetSpeed, int16_t NowSpeed)
{
    pid->Error = TargetSpeed - NowSpeed;

    pid->POut = pid->P * (pid->Error - pid->LastError);
    pid->IOut = pid->I * pid->Error;
    pid->DOut = pid->D * (pid->PrevError - 2*pid->LastError + pid->Error);

    pid->ErrorSum = pid->LastError + pid->Error;
    pid->LastError = pid->Error;

    pid->Output = pid->POut + pid->IOut + pid->DOut;
    pid->SumOutput += pid->Output;

    if(pid->SumOutput > pid->LimitMax)
        pid->SumOutput = pid->LimitMax;
    else if(pid->SumOutput < pid->LimitMin)
        pid->SumOutput = pid->LimitMin;

    return (pid->SumOutput+80);
}

void PIDSetTim1Compare(int16_t PIdOut)
{
    if(PIdOut > 0)
    {
        Motor_Forward();
        TIM_SetTIM1Compare1(PIdOut);
    }
    else
    {
        Motor_Backward();
        TIM_SetTIM1Compare1(-PIdOut);
    }
}



