#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"

#define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的。

int Read_Encoder(u8 TIMX);
void Encoder_Init_TIM2(void);
void TIM2_IRQHandler(void);
void Hall_EXTI_Config(void);
void TIM2_Config(void);
void NVIC_Config(void);
void EXTI0_IRQHandler(void);
void TIM2_IRQHandler(void);

#endif
