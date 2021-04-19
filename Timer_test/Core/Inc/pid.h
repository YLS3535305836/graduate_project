#ifndef __PID_H__
#define __PID_H__

#include "main.h"

#define   Motor_Forward()      HAL_GPIO_WritePin(Direcion_Pin_GPIO_Port, Direcion_Pin_Pin, GPIO_PIN_SET)
#define   Motor_Backward()     HAL_GPIO_WritePin(Direcion_Pin_GPIO_Port, Direcion_Pin_Pin, GPIO_PIN_RESET)

#define PID (*((PIDTydef*)(&arrayData[0])))

typedef struct pid
{
    float P;
    float I;
    float D;
    float POut;
    float IOut;
    float DOut;
    int16_t Output;
    int16_t SumOutput;
    int16_t LimitMax;
    int16_t LimitMin;
    int16_t Error;
    int16_t LastError;
    int16_t PrevError;
    int16_t ErrorSum;
	
}PIDTydef;

extern uint16_t arrayData[16];

//PIDTydef *pidPara
void PIDParameterInit(void);
int16_t PIDCaculate_Position(PIDTydef *pid, int16_t TargetSpeed, int16_t NowSpeed);
int16_t PIDCaculate_Increment(PIDTydef *pid, int16_t TargetSpeed, int16_t NowSpeed);
void PIDSetTim1Compare(int16_t PIdOut);
int16_t PIDCaculate_PositionTest(float p, float i, float d, PIDTydef *pid, int16_t TargetSpeed, int16_t NowSpeed);
int16_t PIDCaculate_IncrementTest(float P, float I, float D, PIDTydef *pid, int16_t TargetSpeed, int16_t NowSpeed);






























#endif

