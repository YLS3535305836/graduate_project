#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#define PWMA   TIM1->CCR1  //PA8
#define AIN2   PBout(1)
#define AIN1   PBout(10)
#define BIN1   PBout(11)
#define BIN2   PBout(14)
#define PWMB   TIM1->CCR4  //PA11
void MiniBalance_PWM_Init(u16 arr,u16 psc);
void MiniBalance_Motor_Init(void);
#endif
