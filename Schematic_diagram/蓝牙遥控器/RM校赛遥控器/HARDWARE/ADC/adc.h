#ifndef __ADC_H
#define __ADC_H	
#include "headfile.h"

/* 若想更改均值滤波基数，请将这里的数组元素数改为基数与ADC通道个数的积 */
extern volatile u16 ADCConvertedValue[30];//三路ADC内存

void dma_init(void);
u16 Get_ADC(u8 num);
u8 Get_X_Value(void);
u8 Get_Y_Value(void);
	
#endif 















