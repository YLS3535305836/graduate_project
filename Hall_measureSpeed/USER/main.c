#include "stm32f10x.h"
#include "stdlib.h"	 
#include "delay.h"
#include "oled.h"	 
#include "encoder.h"
#include "usart.h"	
u16 duty;

 

 int main(void)
 {	
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	  OLED_Init();    //OLED��ʼ��
	  KEY_Init();     //������ʼ��
//	  huoer_init();	  //������ʼ��
//	  TIM4_Init(999,7199);  //��ʱ��4����ʱʱ�䣨999+1��*��7199+1��/72000=100ms
//	  TIM3_PWM_Init(899,0);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
//	  EXIT_Init();        //�ⲿ�жϳ�ʼ��  ʹ���ⲿ�ж�0
    duty=200;           //�����ʼռ�ձ� 
//    uart_init(9600);

  while(1)
	{
		
		     key1_scan();
//		  TIM_SetCompare2(TIM3,duty);	  //���ռ�ձ��������

//        duty=USART_ReceiveData(USART1);
		    OLED_ShowNum(0,0,duty,4,12);		 

//			OLED_ShowNum(0,4,duty,4,16);                       //��ʾռ�ձ�
			
			delay_ms(50);
	}
 }
