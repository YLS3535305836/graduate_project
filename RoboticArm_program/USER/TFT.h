#ifndef __TFT_H
#define __TFT_H	

#include "sys.h"
#include "stdlib.h"	   

#define TFT_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_11)//CS GND
#define TFT_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_11)

#define TFT_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_6)//RES A6
#define TFT_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define TFT_DC_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//DC   A5
#define TFT_DC_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define TFT_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_3)//CLK A3
#define TFT_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_3)

#define TFT_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_4)//DIN A4
#define TFT_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_4)





























#endif

