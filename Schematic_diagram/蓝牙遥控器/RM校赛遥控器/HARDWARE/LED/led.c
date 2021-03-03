#include "led.h"
#include "headfile.h"


void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

}




void GPIO_init(GPIO_TypeDef* GPIOx,u8 pin,u8 mode)
{
	  switch((int)GPIOx)
		{
				case (int)GPIOA:
					RCC->APB2ENR|=1<<2;
					break;
				case (int)GPIOB:
					RCC->APB2ENR|=1<<3;
					break;
				case (int)GPIOC:
					RCC->APB2ENR|=1<<4;
					break;
				case (int)GPIOD:
					RCC->APB2ENR|=1<<5;
					break;
				case (int)GPIOE:
					RCC->APB2ENR|=1<<6;
					break;
				case (int)GPIOF:
					RCC->APB2ENR|=1<<7;
					break;
				
		}
		if(pin<8)
		{
				GPIOx->CRL&=~(0x0000000f<<(pin*4));
			  GPIOx->CRL|=(0x00000000|mode)<<(pin*4);
			  
		}
		else 
		{
				GPIOx->CRH&=~(0X0000000F<<( (pin-8)*4 ));
			  GPIOx->CRH|=(0x00000000|mode)<<( (pin-8)*4 );
		}
//		switch(pin)
//		{
//			case 0:   GPIOx->CRL&=0xfffffff0; break;
//			case 1:   GPIOx->CRL&=0xffffff0f; break;
//			case 2:   GPIOx->CRL&=0xfffff0ff; break;
//			case 3:   GPIOx->CRL&=0xffff0fff; break;
//			case 4:   GPIOx->CRL&=0xfff0ffff; break;
//			case 5:   GPIOx->CRL&=0xff0fffff; break;
//			case 6:   GPIOx->CRL&=0xf0ffffff; break;
//			case 7:   GPIOx->CRL&=0x0fffffff; break;
//			case 8:   GPIOx->CRH&=0xFFFFFFF0; break;
//			case 9:   GPIOx->CRL&=0xffffff0F; break;
//		}
			
}

//设置io的电平高低
//  ex:  GPIO_SET(GPIOA,5,1)    设置GPIOA的5口为高电平
void GPIO_SET(GPIO_TypeDef* GPIOx,u8 pin,u8 level)
{
		if(level==1)
		{
				GPIOx->ODR=1<<pin;
		}
		else 
		{
				GPIOx->ODR=~(1<<pin);
		}
}

//获得io的电平高低
//   ex:  temp=GPIO_SET(GPIOA,5)   读取GPIOA的5口的电平  
//   返回 1 高电平 0 低电平
u8 GPIO_GET(GPIO_TypeDef* GPIOx,u8 pin)
{
		if(GPIOx->ODR&1<<pin)
			return 1;
		else
			return 0;
}




