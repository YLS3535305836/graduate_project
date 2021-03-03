#include "infrared.h"

/**************************************************************************
�������ܣ������ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void INFRARED_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 				//���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);								//��ʼ��GPIOA 3 4
}
/**************************************************************************
�������ܣ�����ɨ�輰����
��ڲ�������
����  ֵ���������ҷ���ֵ
**************************************************************************/
//0��û���յ������ź�
//1��RED_LED0
//2��RED_LED1
//3��RED_LED0|RED_LED1
void INFRARED_Scan()
{
	if((RED_LED0==0)&&(RED_LED1==1))											//RED_LED0�յ������ź�
		Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;  	//��
	else if((RED_LED0==1)&&(RED_LED1==0))									//RED_LED1�յ������ź�
		Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0;		//��
	else	Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//ǰ
}
