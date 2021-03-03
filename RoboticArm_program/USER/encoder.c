#include "encoder.h"
//void huoer_init()  //霍尔元件初始化  使用PA12io口
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //使能PA端口时钟
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			    //PA12 端口配置
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 	 // 上拉输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
//  GPIO_Init(GPIOA, &GPIO_InitStructure);			     //初始化GPIOA12
//  GPIO_SetBits(GPIOA,GPIO_Pin_12);					//PA12输出高
//}
///**************************************************************************
//函数功能：把TIM2初始化为编码器接口模式
//入口参数：无
//返回  值：无
//**************************************************************************/
//void Encoder_Init_TIM2(void)
//{
//	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseStructure;  						//初始化外设TIM x
//  TIM_ICInitTypeDef 				TIM_ICInitStructure;  							//输入捕获
//  GPIO_InitTypeDef 					GPIO_InitStructure;
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);					//使能定时器4的时钟
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);					//使能PB端口时钟

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;					//端口配置PA0(TIM1_CH1) PA1(TIM4_CH2)
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 				//浮空输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
//  GPIO_Init(GPIOA, &GPIO_InitStructure);					      				//根据设定参数初始化GPIOB
//	GPIO_SetBits(GPIOA,GPIO_Pin_12);					//PA12输出高
//  
//  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);								//填入缺省值
//  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 										//预分频器
//  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; 				//设定计数器自动重装值 #define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的。
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;				//选择时钟分频：不分频
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM向上计数
//  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);								//按设定值初始化定时器
//  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
//  TIM_ICStructInit(&TIM_ICInitStructure);		//按缺省值填入
//  TIM_ICInitStructure.TIM_ICFilter = 10;		//选择输入比较滤波器
//  TIM_ICInit(TIM2, &TIM_ICInitStructure);		//输入捕获
//  TIM_ClearFlag(TIM2, TIM_FLAG_Update);			//清除TIM的更新标志位
//  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//使能TIM2更新中断

//  TIM_SetCounter(TIM2,0);										//复位计数器
//  TIM_Cmd(TIM2, ENABLE); 										//使能TIM2
//}

///**************************************************************************
//函数功能：单位时间读取编码器计数
//入口参数：定时器
//返回  值：速度值
//**************************************************************************/
//int Read_Encoder(u8 TIMX)
//{
//   int Encoder_TIM;
//   switch(TIMX)
//	 {
//	   case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;	//(short)TIM2 -> CNT 为10ms编码器计数脉冲的累加 TIM2 -> CNT=0 每记一次清零
//		 case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	//这样就可以把此值(Encoder_TIM)近似当作电机速度，即xxx次/10ms
//		 default:  Encoder_TIM=0;																					//如果不清零，此值将会一直累加，即可作为电机位置参数
//	 }
//		return Encoder_TIM;
//}


///**************************************************************************
//函数功能：TIM2中断服务函数
//入口参数：无
//返回  值：无
//**************************************************************************/
//void TIM2_IRQHandler(void)
//{
//	if(TIM2->SR&0X0001)//溢出中断
//	{
//	}
//	TIM2->SR&=~(1<<0);//清除中断标志位
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;    //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           //响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;      //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;             //响应优先级
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
函数：霍尔外部中断
作用：检测霍尔传感器脉冲并计数
*/
void EXTI0_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(EXTI_Line12) != RESET) 
	{
		//圈数加一
		r1++;
		EXTI_ClearITPendingBit(EXTI_Line12);     
	}  
}
/*
函数：定时器中断
作用：定时
*/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		time++;
		TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
	}
}
