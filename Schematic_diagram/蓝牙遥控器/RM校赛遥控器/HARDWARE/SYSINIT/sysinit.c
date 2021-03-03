#include "headfile.h"



void SysInit(void)   //系统初始化函数
{
	/***** --关闭JTAG-- ******/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //使能RCC_APB2Periph_AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     //禁用JTAG
	/***** --以上勿动-- *****/
	
  delay_init(); 	  //CPU延时初始化
	LED_Init();				//LED初始化
	KEY_Init();				//按键初始化
	uart_init(9600);	//异步串口初始化:波特率9600bps/ TX:PA.9  RX:PA.10  (支持使用printf函数)
	dma_init();				//ADC-DMA初始化
//	OLED_Init();			//OLED初始化
//	OLED_Clear();			//OLED清屏
}



