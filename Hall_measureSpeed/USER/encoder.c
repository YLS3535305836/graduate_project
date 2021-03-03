#include "encoder.h"


extern u16 duty;

void KEY_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //使能PB,PA端口时钟
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 	 //下拉输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			    //LED0-->PA.12 端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			    //LED0-->PA.11 端口配置
   GPIO_Init(GPIOB, &GPIO_InitStructure);			     //初始化GPIOA.12
	
 

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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //使能PA端口时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			    //LED0-->PB.2 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	 //上拉输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);			     //初始化PA0
//  GPIO_SetBits(GPIOB,GPIO_Pin_2);					//PB.2 输出高
}

void TIM4_Init(u16 arr,u16 psc)
{
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	//定时器TIM4初始化
		TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	 
		TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断
   	NVIC_TIM4_Init();  
	  TIM_Cmd(TIM4, ENABLE);  //使能TIM4				
}
uint8_t timer;
//定时器3中断服务程序
void TIM4_IRQHandler(void)   //TIM4中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生与否
		{
		    TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志 
		    timer++;
			 key1_scan();
		   key2_scan();
		}
}


void NVIC_TIM4_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
}



void EXIT_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	huoer_init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	NVIC_EXIT_Init();
	
}
uint16_t Rotating_speed,turns;
//外部中断0服务程序 
void EXTI4_IRQHandler(void)
{
//	delay_ms(10);//消抖
		if(EXTI_GetITStatus(EXTI_Line4) != RESET) 
	{
		turns++;
		EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE0上的中断标志位 
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
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能所在的外部中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2， 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure); 	
}

void PWM_init(void)  //霍尔元件初始化  使用PA12io口
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);	    //使能PA端口时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			    //PA2端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);			     //初始化GPIOA2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;			    //PC15端口配置
	GPIO_Init(GPIOC, &GPIO_InitStructure);			     //初始化GPIOC15
  GPIO_SetBits(GPIOC,GPIO_Pin_15);					//PC15输出高
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);        //PA2输出低
	
}

//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
		
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	PWM_init();

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	

}


