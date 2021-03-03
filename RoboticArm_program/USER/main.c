#include "stm32f10x.h"
#include "stdlib.h"	 
#include "oled.h"	
#include "timer.h"
#include "encoder.h"
#include "usart.h"	
/************************************************
 ALIENTEK 战舰STM32F103开发板实验0
 工程模板
 注意，这是手册中的新建工程章节使用的main文件 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//水平舵机中值 183  左值 175   右值 192
//竖直舵机   90度 188   0度 178  
//1舵机      0度  185   90度  176
u8 duty_X,duty_Y,duty_Z,duty_S,xxy;
uint32_t time = 0,n = 0,r = 0,i = 0,r1 = 0,r2 = 0;  //ms
u8 key;

int main(void)
{
	OLED_Init();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  TIM3_PWM_Init(199,7199);	 //不分频。PWM频率=72000000/900=80Khz
	TIM2_PWM_Init(199,7199);
	uart_init(9600);
	duty_X=183;
	duty_Y=193;
	while(1)
	{
/**********舵机限幅**************************************/
					if(u_flag==1)
					{
								duty_X = ((float)port_X/250)*16+175;
								duty_Y = 178+((float)port_Y/255)*20;
								
										
					}

					if(duty_Y<178)
						 duty_Y=178;
					if(duty_Y>193)
						 duty_Y=193;
					if(key==1)
					{
							duty_S=176;
					}
					else
					{
							duty_S=183;
					}
			//xxy=(float)port_Y/15;
			duty_Z = 364 - duty_Y-1;
					if(duty_Z<171)
						 duty_Z=171;
					
/********************************************************/
//			OLED_ShowNum(0,0,duty_X,4,12);
//			OLED_ShowNum(0,2,duty_Y,4,12);
//			OLED_ShowNum(0,4,duty_S,4,12);
      TIM_SetCompare2(TIM3,duty_Y);	  //B5
			TIM_SetCompare3(TIM3,duty_Z);   //B0
			TIM_SetCompare3(TIM2,duty_X);   //A2
			TIM_SetCompare4(TIM3,duty_S);   //B1
			
		    
		
		
	}
}


