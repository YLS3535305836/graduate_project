#include "beep.h"

/**************************************************************************
�������ܣ��������ӿڳ�ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void BEEP_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ�ܶ˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	          	//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOA
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);											//������ɹرշ�����
}
/**************************************************************************
�������ܣ���Ϸ���
��ڲ���������Ƶ�ʣ���Χ1~10 ����0����
����  ֵ����
**************************************************************************/
void BEEP_Flash(u16 time)
{
	  static int temp;
	  if(0==time) BEEP=1;
	  else		if(++temp==time)	BEEP=~BEEP,temp=0;
}
