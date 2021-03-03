#ifndef __LED_H
#define __LED_H
#include "headfile.h"

#define  LED1  PBout(3)		//LED1
#define  LED2  PBout(4)		//LED2
#define  LED3  PBout(5)		//LED3

#define  imitate   0   //模拟输入
#define  input_h   8   //上下拉输入
#define  Float     4   //浮空输入

#define  Push10     1   //推挽输出10
#define  Push20     2   //推挽输出20
#define  Push50     3   //推挽输出50

#define  O_dr10     5   //开漏输出10
#define  O_dr20     6   //开漏输出20
#define  O_dr50     7   //开漏输出50

#define  M_Push10     9    //推挽输出10   //复用
#define  M_Push20     10   //推挽输出20
#define  M_Push50     11   //推挽输出50

#define  MO_dr10     13   //开漏输出10      //复用
#define  MO_dr20     14   //开漏输出20
#define  MO_dr50     15   //开漏输出50



void GPIO_init(GPIO_TypeDef* GPIOx,u8 pin,u8 mode);

void GPIO_SET(GPIO_TypeDef* GPIOx,u8 pin,u8 level);

void LED_Init(void);

u8 GPIO_GET(GPIO_TypeDef* GPIOx,u8 pin);

#endif


