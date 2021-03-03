#include "headfile.h"



void SysInit(void)   //ϵͳ��ʼ������
{
	/***** --�ر�JTAG-- ******/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //ʹ��RCC_APB2Periph_AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     //����JTAG
	/***** --������-- *****/
	
  delay_init(); 	  //CPU��ʱ��ʼ��
	LED_Init();				//LED��ʼ��
	KEY_Init();				//������ʼ��
	uart_init(9600);	//�첽���ڳ�ʼ��:������9600bps/ TX:PA.9  RX:PA.10  (֧��ʹ��printf����)
	dma_init();				//ADC-DMA��ʼ��
//	OLED_Init();			//OLED��ʼ��
//	OLED_Clear();			//OLED����
}



