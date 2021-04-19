#include "pid.h"
#include "tim.h"

uint16_t arrayData[16];

void PIDParameterInit(void)
{
    PID = (PIDTydef)
    {
        100.0, 0.0, 0.0,
        0, 0, 0, 0, 0, 
        800, -800, 
        0, 0, 0, 0
    };
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


int16_t PIDCaculate_PositionTest(float p, float i, float d, PIDTydef *pid, int16_t TargetSpeed, int16_t NowSpeed)
{
    pid->Error = TargetSpeed - NowSpeed;

    pid->POut = p * pid->Error;
    pid->IOut = i * pid->ErrorSum;
    pid->DOut = d * (pid->Error - pid->LastError);

    pid->ErrorSum = pid->LastError + pid->Error;
    pid->LastError = pid->Error;

    pid->Output = pid->POut + pid->IOut + pid->DOut;

    if(pid->Output > pid->LimitMax)
        pid->Output = pid->LimitMax;
    else if(pid->Output < pid->LimitMin)
        pid->Output = pid->LimitMin;

    return (pid->Output);
}


int16_t PIDCaculate_IncrementTest(float P, float I, float D, PIDTydef *pid, int16_t TargetSpeed, int16_t NowSpeed)
{
    pid->Error = TargetSpeed - NowSpeed;

    pid->POut = P * (pid->Error - pid->LastError);
    pid->IOut = (I/100) * pid->Error;
    pid->DOut = D * (pid->PrevError - 2*pid->LastError + pid->Error);

    pid->ErrorSum = pid->LastError + pid->Error;
    pid->LastError = pid->Error;
    pid->PrevError = pid->LastError;

    pid->Output = pid->POut + pid->IOut + pid->DOut;
    pid->SumOutput += pid->Output;

    if(pid->SumOutput > pid->LimitMax)
        pid->SumOutput = pid->LimitMax;
    else if(pid->SumOutput < pid->LimitMin)
        pid->SumOutput = pid->LimitMin;

    return (pid->SumOutput);
}
