#ifndef __RED_LED_H
#define __RED_LED_H
#include "sys.h"

#define RED_LED0 PAin(3)	//��ȡ����0
#define RED_LED1 PAin(4)	//��ȡ����1

void INFRARED_Init(void);		//IO��ʼ��
void INFRARED_Scan(void);  	//����ɨ�躯��

#endif

