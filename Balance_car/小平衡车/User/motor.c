#include "motor.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
void MiniBalance_Motor_Init(void)
{
 
  /*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIO*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;

		/*调用库函数，初始化GPIO*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;

		/*调用库函数，初始化GPIOF*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
				/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;

		/*调用库函数，初始化GPIOF*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);

}
void MiniBalance_PWM_Init(u16 arr,u16 psc)
{		 					 
	MiniBalance_Motor_Init();  //初始化电机控制所需IO
	RCC->APB2ENR|=1<<11;       //使能TIM1时钟    
	RCC->APB2ENR|=1<<2;        //PORTA时钟使能     
	GPIOA->CRH&=0XFFFF0FF0;    //PORTA8 11复用输出
	GPIOA->CRH|=0X0000B00B;    //PORTA8 11复用输出
	TIM1->ARR=arr;             //设定计数器自动重装值 
	TIM1->PSC=psc;             //预分频器不分频
	TIM1->CCMR2|=6<<12;        //CH4 PWM1模式	
	TIM1->CCMR1|=6<<4;         //CH1 PWM1模式	
	TIM1->CCMR2|=1<<11;        //CH4预装载使能	 
	TIM1->CCMR1|=1<<3;         //CH1预装载使能	  
	TIM1->CCER|=1<<12;         //CH4输出使能	   
	TIM1->CCER|=1<<0;          //CH1输出使能	
	TIM1->BDTR |= 1<<15;       //TIM1必须要这句话才能输出PWM
	TIM1->CR1=0x8000;          //ARPE使能 
	TIM1->CR1|=0x01;          //使能定时器1 			
} 

