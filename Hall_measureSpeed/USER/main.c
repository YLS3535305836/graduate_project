#include "stm32f10x.h"
#include "stdlib.h"	 
#include "delay.h"
#include "oled.h"	 
#include "encoder.h"
#include "usart.h"	
u16 duty;

 

 int main(void)
 {	
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	  OLED_Init();    //OLED初始化
	  KEY_Init();     //按键初始化
//	  huoer_init();	  //霍尔初始化
//	  TIM4_Init(999,7199);  //定时器4，定时时间（999+1）*（7199+1）/72000=100ms
//	  TIM3_PWM_Init(899,0);	 //不分频。PWM频率=72000000/900=80Khz
//	  EXIT_Init();        //外部中断初始化  使用外部中断0
    duty=200;           //电机初始占空比 
//    uart_init(9600);

  while(1)
	{
		
		     key1_scan();
//		  TIM_SetCompare2(TIM3,duty);	  //电机占空比输出函数

//        duty=USART_ReceiveData(USART1);
		    OLED_ShowNum(0,0,duty,4,12);		 

//			OLED_ShowNum(0,4,duty,4,16);                       //显示占空比
			
			delay_ms(50);
	}
 }
