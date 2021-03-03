#include "encoder.h"


extern u16 duty;

void KEY_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //ʹ��PB,PA�˿�ʱ��
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 	 //�������
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			    //LED0-->PA.12 �˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			    //LED0-->PA.11 �˿�����
   GPIO_Init(GPIOB, &GPIO_InitStructure);			     //��ʼ��GPIOA.12
	
 

}
void key1_scan(void)
{
	  static unsigned char state1;
	  switch(state1)
		{
			case 0: if(KEY1==1)
			        {
								state1=1;
								break;
							}
			case 1: if(KEY1==1)
			        {
								state1=2;
								duty-=20;
								break;
							}
							else
							{
								state1=0;
								break;
							}
			case 2: if(KEY1==0)
			        {
								state1=0;
								break;
							}
			default:   break;
		}
}

void key2_scan(void)
{
	  static unsigned char state2;
	  switch(state2)
		{
			case 0: if(KEY2==1)
			        {
								state2=1;
								break;
							}
			case 1: if(KEY2==1)
			        {
								state2=2;
								duty+=20;
								break;
							}
							else
							{
								state2=0;
								break;
							}
			case 2: if(KEY2==0)
			        {
								state2=0;
								break;
							}
			default:   break;
		}
}


void huoer_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //ʹ��PA�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			    //LED0-->PB.2 �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);			     //��ʼ��PA0
//  GPIO_SetBits(GPIOB,GPIO_Pin_2);					//PB.2 �����
}

void TIM4_Init(u16 arr,u16 psc)
{
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	//��ʱ��TIM4��ʼ��
		TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	 
		TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�
   	NVIC_TIM4_Init();  
	  TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4				
}
uint8_t timer;
//��ʱ��3�жϷ������
void TIM4_IRQHandler(void)   //TIM4�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
		{
		    TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		    timer++;
			 key1_scan();
		   key2_scan();
		}
}


void NVIC_TIM4_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
}



void EXIT_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	huoer_init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	NVIC_EXIT_Init();
	
}
uint16_t Rotating_speed,turns;
//�ⲿ�ж�0������� 
void EXTI4_IRQHandler(void)
{
//	delay_ms(10);//����
		if(EXTI_GetITStatus(EXTI_Line4) != RESET) 
	{
		turns++;
		EXTI_ClearITPendingBit(EXTI_Line4); //���LINE0�ϵ��жϱ�־λ 
	}
	 
}

void TIM2Counter_Init()
{
      RCC->APB1ENR|=1<<0;        //TIM2 ???? 
      RCC->APB2ENR|=1<<2;        //??PORTA??  
      
      GPIOB->CRL&=0XFFFFFFF0;    //PA0 ??????  
      GPIOB->CRL|=0X00000008;    //PA0 ??   
      GPIOB->ODR|=0<<0;          //PA0 ??
      
      TIM2->ARR=8000;          //??????????   
      TIM2->PSC=0;          //???? 
 
      TIM2->SMCR &= ~(0xf<<8);      //???
      TIM2->SMCR &= ~(3<<12);       //?????
      TIM2->SMCR |= 1<<15;          //ETR???,?????????
      TIM2->SMCR |= 1<<14;          //????????2

      TIM2->DIER |= 0<<0;           //??????
      TIM2->DIER |= 0<<6;           //??????

      TIM2->CNT = 0x0;              //?????
      TIM2->CR1 |= 1<<0;            //?????,?????
}


 u16 Get_TIMCount(TIM_TypeDef * TIMx)
 {
	 u16 count=0;
	 count = TIMx->CNT;
	 TIMx->CNT=0;
	 return count;
 }

void NVIC_EXIT_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�����ڵ��ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2�� 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure); 	
}

void PWM_init(void)  //����Ԫ����ʼ��  ʹ��PA12io��
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);	    //ʹ��PA�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			    //PA2�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);			     //��ʼ��GPIOA2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;			    //PC15�˿�����
	GPIO_Init(GPIOC, &GPIO_InitStructure);			     //��ʼ��GPIOC15
  GPIO_SetBits(GPIOC,GPIO_Pin_15);					//PC15�����
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);        //PA2�����
	
}

//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
		
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	PWM_init();

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	

}


