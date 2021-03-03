#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"


extern uint8_t timer;
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void PWM_init(void);
void TIM2_PWM_Init(u16 arr,u16 psc);
#endif
