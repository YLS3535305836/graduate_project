#ifndef __ADC_H
#define __ADC_H	
#include "headfile.h"

/* ������ľ�ֵ�˲��������뽫���������Ԫ������Ϊ������ADCͨ�������Ļ� */
extern volatile u16 ADCConvertedValue[30];//��·ADC�ڴ�

void dma_init(void);
u16 Get_ADC(u8 num);
u8 Get_X_Value(void);
u8 Get_Y_Value(void);
	
#endif 















