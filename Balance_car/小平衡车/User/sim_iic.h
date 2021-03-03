#ifndef _SIM_IIC_H
#define _SIM_IIC_H

#include "stm32f10x.h"
#include "sys.h"
//IO��������
#define SDA_IN()  {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=0x08;}    //PB.8
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=0x03;}    //PB.8
//IO��������	 
#define IIC_SCL    PBout(9) 	//SCL
#define IIC_SDA    PBout(8) 	//SDA	 
#define READ_SDA   PBin(8) 	  //����SDA 
//IIC���в�������
void IIC_Delay(u16 i);									//IIC��ʱ
void IIC_Init(void);               	  //��ʼ��IIC��IO��				 
void IIC_Start(void);									//����IIC��ʼ�ź�
void IIC_Stop(void);	  								//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);							//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);		//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 									//IIC�ȴ�ACK�ź�
void IIC_Ack(void);										//IIC����ACK�ź�
void IIC_NAck(void);									  //IIC������ACK�ź�

u8 IIC_Set_Reg(u8 addr,u8 data,u8 Dev_addr);				//IIC�����Ĵ�������
u8 IIC_Read_Reg(u8 addr,u8 Dev_addr);								//IIC�����Ĵ�����ȡ



#endif



