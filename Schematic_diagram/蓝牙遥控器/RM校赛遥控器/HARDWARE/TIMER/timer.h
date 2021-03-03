#ifndef __TIMER_H
#define __TIMER_H
#include "headfile.h"


#define  AIN1  PAout(1)
#define  AIN2  PAout(2)
#define  PWM   (TIM2->CCR1)


void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);


#endif

