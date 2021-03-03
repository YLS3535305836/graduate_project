#include "headfile.h"

/************************************************
项目名称：电赛用蓝牙遥控器
主控芯片：STM32F103C8T6
内    核：Cortex-M3
开始时间：2019年6月9日
完成时间：2019年---
************************************************/

#define  X_HEAD  ((char)'X')			//X轴坐标数据包帧头
#define  X_MIDDLE  ((char)'A')		//X轴坐标数据包校验码

#define  Y_HEAD  ((char)'Y')			//Y轴坐标数据包帧头
#define  Y_MIDDLE  ((char)'B')		//Y轴坐标数据包校验码

#define  MIX_HEAD_X  ((char)'x')	//XY坐标数据包X帧头
#define  MIX_MIDDLE_X  ((char)'a')//XY坐标数据包X校验码
#define  MIX_HEAD_Y  ((char)'y')	//XY坐标数据包Y帧头
#define  MIX_MIDDLE_Y  ((char)'b')//XY坐标数据包Y校验码

void Usart_Send(USART_TypeDef* USARTx,u8 data);
void Send_X_Post(u8 post);
void Send_Y_Post(u8 post);
void Send_XY_Post(u8 X_post,u8 Y_post,u8 key);

u8 X=0,Y=0;
u16 DATA;
u8 duty_X,duty_Y,duty_Z,duty_S,duty_s;
int main(void)
{
	SysInit();			//系统初始化
	while(1)
	{
/***************按键*****************************************/
		 // key2_scan();
		  key1_scan();
			
/***********************************************************/
		  X = Get_X_Value();					//获取X坐标
		  Y = Get_Y_Value();					//获取Y坐标
	

		delay_ms(10);
		Send_XY_Post(X,Y,duty_S);					//发送XY轴数据帧


	}
}


//** 发送X轴坐标数据帧 **//
void Send_X_Post(u8 post)
{
	Usart_Send(USART1,X_HEAD);
	Usart_Send(USART1,X_MIDDLE);
	Usart_Send(USART1,post);

}


//** 发送Y轴坐标数据帧 **//
void Send_Y_Post(u8 post)
{
	Usart_Send(USART1,Y_HEAD);
	Usart_Send(USART1,Y_MIDDLE);
	Usart_Send(USART1,post);
}


//** 发送X、Y坐标数据帧 **//
void Send_XY_Post(u8 X_post,u8 Y_post,u8 key)
{
//	Usart_Send(USART1,MIX_HEAD_X);
//	Usart_Send(USART1,MIX_MIDDLE_X);
//	Usart_Send(USART1,X_post);
//	delay_ms(1);
//	Usart_Send(USART1,MIX_HEAD_Y);
//	Usart_Send(USART1,MIX_MIDDLE_Y);
//	Usart_Send(USART1,Y_post);
	  Usart_Send(USART1,0xff);
		Usart_Send(USART1,0xfe);
		Usart_Send(USART1,X_post);
		Usart_Send(USART1,Y_post);
	  Usart_Send(USART1,key);
		Usart_Send(USART1,0xfd);
}


//串口发送函数
void Usart_Send(USART_TypeDef* USARTx,u8 data)
{
	USARTx->SR&=~(1<<6);
	USARTx->DR=data;
	while(!(USARTx->SR&0x40));
}



