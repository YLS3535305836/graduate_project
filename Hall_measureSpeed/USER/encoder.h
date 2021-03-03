#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"

#define KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)
#define KEY2  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)

extern uint16_t Rotating_speed,turns;
extern uint8_t timer;

void KEY_Init(void);
void key1_scan(void);
void key2_scan(void);

void huoer_init(void);
void TIM4_Init(u16 arr,u16 psc);
void TIM4_IRQHandler(void);
void NVIC_TIM4_Init(void);

void TIM2Counter_Init(void);
u16 Get_TIMCount(TIM_TypeDef * TIMx);
 
void EXIT_Init(void);
void NVIC_EXIT_Init(void);
void EXTI4_IRQHandler(void);

void TIM3_PWM_Init(u16 arr,u16 psc);
void PWM_init(void);

#endif
