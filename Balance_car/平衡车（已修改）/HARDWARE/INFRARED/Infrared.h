#ifndef __RED_LED_H
#define __RED_LED_H
#include "sys.h"

#define RED_LED0 PAin(3)	//读取红外0
#define RED_LED1 PAin(4)	//读取红外1

void INFRARED_Init(void);		//IO初始化
void INFRARED_Scan(void);  	//红外扫描函数

#endif

