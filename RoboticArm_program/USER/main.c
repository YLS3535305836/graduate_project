#include "stm32f10x.h"
#include "stdlib.h"	 
#include "oled.h"	
#include "timer.h"
#include "encoder.h"
#include "usart.h"	
/************************************************
 ALIENTEK ս��STM32F103������ʵ��0
 ����ģ��
 ע�⣬�����ֲ��е��½������½�ʹ�õ�main�ļ� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//ˮƽ�����ֵ 183  ��ֵ 175   ��ֵ 192
//��ֱ���   90�� 188   0�� 178  
//1���      0��  185   90��  176
u8 duty_X,duty_Y,duty_Z,duty_S,xxy;
uint32_t time = 0,n = 0,r = 0,i = 0,r1 = 0,r2 = 0;  //ms
u8 key;

int main(void)
{
	OLED_Init();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  TIM3_PWM_Init(199,7199);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
	TIM2_PWM_Init(199,7199);
	uart_init(9600);
	duty_X=183;
	duty_Y=193;
	while(1)
	{
/**********����޷�**************************************/
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


