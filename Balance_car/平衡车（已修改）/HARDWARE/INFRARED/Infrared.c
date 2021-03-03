#include "infrared.h"

/**************************************************************************
函数功能：红外初始化
入口参数：无
返回  值：无
**************************************************************************/
void INFRARED_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 				//设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化GPIOA 3 4
}
/**************************************************************************
函数功能：红外扫描及处理
入口参数：无
返回  值：返回左右方向值
**************************************************************************/
//0，没有收到反射信号
//1，RED_LED0
//2，RED_LED1
//3，RED_LED0|RED_LED1
void INFRARED_Scan()
{
	if((RED_LED0==0)&&(RED_LED1==1))											//RED_LED0收到反射信号
		Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;  	//左
	else if((RED_LED0==1)&&(RED_LED1==0))									//RED_LED1收到反射信号
		Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0;		//右
	else	Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//前
}
