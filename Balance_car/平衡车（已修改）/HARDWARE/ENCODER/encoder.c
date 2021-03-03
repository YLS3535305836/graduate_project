#include "encoder.h"

/**************************************************************************
�������ܣ���TIM2��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseStructure;  						//��ʼ������TIM x
  TIM_ICInitTypeDef 				TIM_ICInitStructure;  							//���벶��
  GPIO_InitTypeDef 					GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);					//ʹ�ܶ�ʱ��4��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);					//ʹ��PB�˿�ʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;					//�˿�����PA0(TIM1_CH1) PA1(TIM4_CH2)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 				//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      				//�����趨������ʼ��GPIOB

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);								//����ȱʡֵ
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 										//Ԥ��Ƶ��
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; 				//�趨�������Զ���װֵ #define ENCODER_TIM_PERIOD (u16)(65535)   //���ɴ���65535 ��ΪF103�Ķ�ʱ����16λ�ġ�
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;				//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM���ϼ���
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);								//���趨ֵ��ʼ����ʱ��
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
  TIM_ICStructInit(&TIM_ICInitStructure);		//��ȱʡֵ����
  TIM_ICInitStructure.TIM_ICFilter = 10;		//ѡ������Ƚ��˲���
  TIM_ICInit(TIM2, &TIM_ICInitStructure);		//���벶��
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);			//���TIM�ĸ��±�־λ
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//ʹ��TIM2�����ж�

  TIM_SetCounter(TIM2,0);										//��λ������
  TIM_Cmd(TIM2, ENABLE); 										//ʹ��TIM2
}
/**************************************************************************
�������ܣ���TIM4��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM4(void)
{
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseStructure;  						//��ʼ������TIM x
  TIM_ICInitTypeDef 				TIM_ICInitStructure;  							//���벶��
  GPIO_InitTypeDef 					GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);					//ʹ�ܶ�ʱ��4��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);					//ʹ��PB�˿�ʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;					//�˿����� PB6(TIM4_CH1) PB7(TIM4_CH2)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 				//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      				//�����趨������ʼ��GPIOB

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);								//����ȱʡֵ
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 										//Ԥ��Ƶ��
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; 				//�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;				//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM���ϼ���
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);								//���趨ֵ��ʼ����ʱ��
  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
  TIM_ICStructInit(&TIM_ICInitStructure);		//��ȱʡֵ����
  TIM_ICInitStructure.TIM_ICFilter = 10;		//ѡ������Ƚ��˲���
  TIM_ICInit(TIM4, &TIM_ICInitStructure);		//���벶��
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);			//���TIM�ĸ��±�־λ
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//ʹ��TIM4�����ж�

  TIM_SetCounter(TIM4,0);										//��λ������
  TIM_Cmd(TIM4, ENABLE);  									//ʹ��TIM4
}
/**************************************************************************
�������ܣ���λʱ���ȡ����������
��ڲ�������ʱ��
����  ֵ���ٶ�ֵ
**************************************************************************/
int Read_Encoder(u8 TIMX)
{
   int Encoder_TIM;
   switch(TIMX)
	 {
	   case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;	//(short)TIM2 -> CNT Ϊ10ms����������������ۼ� TIM2 -> CNT=0 ÿ��һ������
		 case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	//�����Ϳ��԰Ѵ�ֵ(Encoder_TIM)���Ƶ�������ٶȣ���xxx��/10ms
		 default:  Encoder_TIM=0;																					//��������㣬��ֵ����һֱ�ۼӣ�������Ϊ���λ�ò���
	 }
		return Encoder_TIM;
}
/**************************************************************************
�������ܣ�TIM4�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
void TIM4_IRQHandler(void)
{
	if(TIM4->SR&0X0001)//����ж�
	{
	}
	TIM4->SR&=~(1<<0);//����жϱ�־λ
}
/**************************************************************************
�������ܣ�TIM2�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
void TIM2_IRQHandler(void)
{
	if(TIM2->SR&0X0001)//����ж�
	{
	}
	TIM2->SR&=~(1<<0);//����жϱ�־λ
}
