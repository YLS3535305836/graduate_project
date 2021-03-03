#include "headfile.h"

/************************************************
��Ŀ���ƣ�����������ң����
����оƬ��STM32F103C8T6
��    �ˣ�Cortex-M3
��ʼʱ�䣺2019��6��9��
���ʱ�䣺2019��---
************************************************/

#define  X_HEAD  ((char)'X')			//X���������ݰ�֡ͷ
#define  X_MIDDLE  ((char)'A')		//X���������ݰ�У����

#define  Y_HEAD  ((char)'Y')			//Y���������ݰ�֡ͷ
#define  Y_MIDDLE  ((char)'B')		//Y���������ݰ�У����

#define  MIX_HEAD_X  ((char)'x')	//XY�������ݰ�X֡ͷ
#define  MIX_MIDDLE_X  ((char)'a')//XY�������ݰ�XУ����
#define  MIX_HEAD_Y  ((char)'y')	//XY�������ݰ�Y֡ͷ
#define  MIX_MIDDLE_Y  ((char)'b')//XY�������ݰ�YУ����

void Usart_Send(USART_TypeDef* USARTx,u8 data);
void Send_X_Post(u8 post);
void Send_Y_Post(u8 post);
void Send_XY_Post(u8 X_post,u8 Y_post,u8 key);

u8 X=0,Y=0;
u16 DATA;
u8 duty_X,duty_Y,duty_Z,duty_S,duty_s;
int main(void)
{
	SysInit();			//ϵͳ��ʼ��
	while(1)
	{
/***************����*****************************************/
		 // key2_scan();
		  key1_scan();
			
/***********************************************************/
		  X = Get_X_Value();					//��ȡX����
		  Y = Get_Y_Value();					//��ȡY����
	

		delay_ms(10);
		Send_XY_Post(X,Y,duty_S);					//����XY������֡


	}
}


//** ����X����������֡ **//
void Send_X_Post(u8 post)
{
	Usart_Send(USART1,X_HEAD);
	Usart_Send(USART1,X_MIDDLE);
	Usart_Send(USART1,post);

}


//** ����Y����������֡ **//
void Send_Y_Post(u8 post)
{
	Usart_Send(USART1,Y_HEAD);
	Usart_Send(USART1,Y_MIDDLE);
	Usart_Send(USART1,post);
}


//** ����X��Y��������֡ **//
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


//���ڷ��ͺ���
void Usart_Send(USART_TypeDef* USARTx,u8 data)
{
	USARTx->SR&=~(1<<6);
	USARTx->DR=data;
	while(!(USARTx->SR&0x40));
}



