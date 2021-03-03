#ifndef __LED_H
#define __LED_H
#include "headfile.h"

#define  LED1  PBout(3)		//LED1
#define  LED2  PBout(4)		//LED2
#define  LED3  PBout(5)		//LED3

#define  imitate   0   //ģ������
#define  input_h   8   //����������
#define  Float     4   //��������

#define  Push10     1   //�������10
#define  Push20     2   //�������20
#define  Push50     3   //�������50

#define  O_dr10     5   //��©���10
#define  O_dr20     6   //��©���20
#define  O_dr50     7   //��©���50

#define  M_Push10     9    //�������10   //����
#define  M_Push20     10   //�������20
#define  M_Push50     11   //�������50

#define  MO_dr10     13   //��©���10      //����
#define  MO_dr20     14   //��©���20
#define  MO_dr50     15   //��©���50



void GPIO_init(GPIO_TypeDef* GPIOx,u8 pin,u8 mode);

void GPIO_SET(GPIO_TypeDef* GPIOx,u8 pin,u8 level);

void LED_Init(void);

u8 GPIO_GET(GPIO_TypeDef* GPIOx,u8 pin);

#endif


