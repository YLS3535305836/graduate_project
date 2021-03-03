#include "encoder.h"
//void huoer_init()  //����Ԫ����ʼ��  ʹ��PA12io��
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //ʹ��PA�˿�ʱ��
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			    //PA12 �˿�����
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 	 // �������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
//  GPIO_Init(GPIOA, &GPIO_InitStructure);			     //��ʼ��GPIOA12
//  GPIO_SetBits(GPIOA,GPIO_Pin_12);					//PA12�����
//}
///**************************************************************************
//�������ܣ���TIM2��ʼ��Ϊ�������ӿ�ģʽ
//��ڲ�������
//����  ֵ����
//**************************************************************************/
//void Encoder_Init_TIM2(void)
//{
//	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseStructure;  						//��ʼ������TIM x
//  TIM_ICInitTypeDef 				TIM_ICInitStructure;  							//���벶��
//  GPIO_InitTypeDef 					GPIO_InitStructure;
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);					//ʹ�ܶ�ʱ��4��ʱ��
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);					//ʹ��PB�˿�ʱ��

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;					//�˿�����PA0(TIM1_CH1) PA1(TIM4_CH2)
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 				//��������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
//  GPIO_Init(GPIOA, &GPIO_InitStructure);					      				//�����趨������ʼ��GPIOB
//	GPIO_SetBits(GPIOA,GPIO_Pin_12);					//PA12�����
//  
//  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);								//����ȱʡֵ
//  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 										//Ԥ��Ƶ��
//  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; 				//�趨�������Զ���װֵ #define ENCODER_TIM_PERIOD (u16)(65535)   //���ɴ���65535 ��ΪF103�Ķ�ʱ����16λ�ġ�
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;				//ѡ��ʱ�ӷ�Ƶ������Ƶ
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM���ϼ���
//  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);								//���趨ֵ��ʼ����ʱ��
//  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
//  TIM_ICStructInit(&TIM_ICInitStructure);		//��ȱʡֵ����
//  TIM_ICInitStructure.TIM_ICFilter = 10;		//ѡ������Ƚ��˲���
//  TIM_ICInit(TIM2, &TIM_ICInitStructure);		//���벶��
//  TIM_ClearFlag(TIM2, TIM_FLAG_Update);			//���TIM�ĸ��±�־λ
//  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//ʹ��TIM2�����ж�

//  TIM_SetCounter(TIM2,0);										//��λ������
//  TIM_Cmd(TIM2, ENABLE); 										//ʹ��TIM2
//}

///**************************************************************************
//�������ܣ���λʱ���ȡ����������
//��ڲ�������ʱ��
//����  ֵ���ٶ�ֵ
//**************************************************************************/
//int Read_Encoder(u8 TIMX)
//{
//   int Encoder_TIM;
//   switch(TIMX)
//	 {
//	   case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;	//(short)TIM2 -> CNT Ϊ10ms����������������ۼ� TIM2 -> CNT=0 ÿ��һ������
//		 case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	//�����Ϳ��԰Ѵ�ֵ(Encoder_TIM)���Ƶ�������ٶȣ���xxx��/10ms
//		 default:  Encoder_TIM=0;																					//��������㣬��ֵ����һֱ�ۼӣ�������Ϊ���λ�ò���
//	 }
//		return Encoder_TIM;
//}


///**************************************************************************
//�������ܣ�TIM2�жϷ�����
//��ڲ�������
//����  ֵ����
//**************************************************************************/
//void TIM2_IRQHandler(void)
//{
//	if(TIM2->SR&0X0001)//����ж�
//	{
//	}
//	TIM2->SR&=~(1<<0);//����жϱ�־λ
//}
extern uint32_t r1,time;
void Hall_EXTI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	NVIC_Config();
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
}


void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;    //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           //��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;      //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;             //��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Period = (1000-1);
	TIM_TimeBaseInitStructure.TIM_Prescaler = 71;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);

	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_Config(void)
{
	NVIC_Config();
	TIM_Mode_Config();
}
/*
�����������ⲿ�ж�
���ã����������������岢����
*/
void EXTI0_IRQHandler(void)
{
  //ȷ���Ƿ������EXTI Line�ж�
	if(EXTI_GetITStatus(EXTI_Line12) != RESET) 
	{
		//Ȧ����һ
		r1++;
		EXTI_ClearITPendingBit(EXTI_Line12);     
	}  
}
/*
��������ʱ���ж�
���ã���ʱ
*/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		time++;
		TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
	}
}
