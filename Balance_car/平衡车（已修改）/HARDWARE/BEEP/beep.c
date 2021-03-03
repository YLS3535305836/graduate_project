#include "beep.h"

/**************************************************************************
函数功能：蜂鸣器接口初始化
入口参数：无
返回  值：无
**************************************************************************/
void BEEP_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能端口时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	          	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOA
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);											//配置完成关闭蜂鸣器
}
/**************************************************************************
函数功能：间断发声
入口参数：发声频率：范围1~10 输入0长响
返回  值：无
**************************************************************************/
void BEEP_Flash(u16 time)
{
	  static int temp;
	  if(0==time) BEEP=1;
	  else		if(++temp==time)	BEEP=~BEEP,temp=0;
}
