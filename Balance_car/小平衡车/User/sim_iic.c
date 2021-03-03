/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,徐氏伟业
 * All rights reserved.
 *
 * 以下所有内容版权均属徐氏伟业所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留徐氏伟业的版权声明。
 *
 * @file       		I2C
 * @company	   		华北理工大学
 * @author     		徐氏伟业
 * @Software 			MDK 5.24a
 * @Target core		STM32F407VET6
 * @date       		2019-01-31
 ********************************************************************************************************************/

#include "sim_iic.h"

void IIC_Delay(u16 i)
{
	u16 k = i;
	while(k--);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      软件I2C初始化
//  @return     void
//  Sample usage:             IIC_Init();    //软件IIC初始化：SDA=PB7、SCL=PB6
//-------------------------------------------------------------------------------------------------------------------
void IIC_Init()
{					     
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
// 	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_15);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      I2C主机发起起始信号
//  @return     void					
//  Sample usage:             IIC_Start();    //I2C主机发起起始信号
//-------------------------------------------------------------------------------------------------------------------
void IIC_Start()
{
	SDA_OUT();
	IIC_SDA=1;
	IIC_SCL=1;
	IIC_Delay(100);
 	IIC_SDA=0;
	IIC_Delay(100);
	IIC_SCL=0;
}	  

//-------------------------------------------------------------------------------------------------------------------
//  @brief      I2C主机发起停止信号
//  @return     void					
//  Sample usage:             IIC_Stop();    //I2C主机发起停止信号
//-------------------------------------------------------------------------------------------------------------------
void IIC_Stop()
{
	SDA_OUT();
	IIC_SCL=0;
	IIC_SDA=0;
 	IIC_Delay(100);
	IIC_SCL=1;
	IIC_SDA=1;
	IIC_Delay(100);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      I2C主机等待应答信号
//  @return     u8						返回0为应答成功，返回1为应答失败
//  Sample usage:             while(IIC_Wait_Ack());    //等待应答信号，若应答失败则卡死在循环中
//-------------------------------------------------------------------------------------------------------------------
u8 IIC_Wait_Ack()
{
	u8 ucErrTime=0;
	SDA_IN(); 
	IIC_SDA=1;IIC_Delay(100);
	IIC_SCL=1;IIC_Delay(100);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;
	return 0;
} 

//-------------------------------------------------------------------------------------------------------------------
//  @brief      发送应答信号（ACK）
//  @return     void
//  Sample usage:             IIC_Ack();    //发送应答信号
//-------------------------------------------------------------------------------------------------------------------	 
void IIC_Ack()
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	IIC_Delay(100);
	IIC_SCL=1;
	IIC_Delay(100);
	IIC_SCL=0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      发送非应答信号（NACK）
//  @return     void
//  Sample usage:             IIC_NAck();    //发送非应答信号
//-------------------------------------------------------------------------------------------------------------------	    
void IIC_NAck()
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	IIC_Delay(100);
	IIC_SCL=1;
	IIC_Delay(100);
	IIC_SCL=0;
}					 				     
		
//-------------------------------------------------------------------------------------------------------------------
//  @brief      I2C总线发送一个字节
//  @param      txd           待发送数据（8位）
//  @return     void
//  Sample usage:             IIC_Send_Byte(0x52);    //向总线发送数据"0x52"
//-------------------------------------------------------------------------------------------------------------------
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		SDA_OUT(); 	    
    IIC_SCL=0;
    for(t=0;t<8;t++)
    {              
			if((txd&0x80)>>7)
				IIC_SDA=1;
			else
				IIC_SDA=0;
			txd<<=1; 	  
			IIC_Delay(100);
			IIC_SCL=1;
			IIC_Delay(100);
			IIC_SCL=0;	
			IIC_Delay(100);
    }
} 	    

//-------------------------------------------------------------------------------------------------------------------
//  @brief      I2C主机接收一个字节
//  @param      ack           产生应答或非应答信号【ack=1为产生应答信号，ack=0为产生非应答信号】
//  @return     u8						返回一个字节的数据
//  Sample usage:             data = IIC_Read_Byte(0);    //读取一个字节的数据并且发送NACK信号使从机释放总线控制权
//-------------------------------------------------------------------------------------------------------------------
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        IIC_Delay(100);
				IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)
					receive++;   
				IIC_Delay(100);
    }
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack();
    return receive;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      I2C主机配置寄存器（最多8位寄存器）
//  @param      addr          寄存器地址
//  @param      data          待发送数据
//  @param      Dev_addr      I2C器件物理地址
//  @return     u8						返回0为成功，返回1为失败
//  Sample usage:             while(IIC_Set_Reg(0x03,0x6B,0x01));    //I2C主机配置寄存器
//-------------------------------------------------------------------------------------------------------------------
u8 IIC_Set_Reg(u8 addr,u8 data,u8 Dev_addr)
{
	IIC_Start();
	IIC_Send_Byte((Dev_addr<<1)|0);
	while(IIC_Wait_Ack())
	{
		IIC_Stop();
		return 1;
	}
	IIC_Send_Byte(addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(data);
	while(IIC_Wait_Ack())
	{
		IIC_Stop();
		return 1;
	}
	IIC_Stop();
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      I2C主机读取寄存器值（最多8位寄存器）
//  @param      addr          寄存器地址
//  @param      Dev_addr      I2C器件物理地址
//  @return     u8						返回一个字节的数据
//  Sample usage:             data=IIC_Read_Reg(0x02,0x01);    //I2C主机读取寄存器值
//-------------------------------------------------------------------------------------------------------------------
u8 IIC_Read_Reg(u8 addr,u8 Dev_addr)
{
	u8 reg_data=0;
	IIC_Start();
	IIC_Send_Byte((Dev_addr<<1)|0);
	while(IIC_Wait_Ack())
	{
		IIC_Stop();
		return 1;
	}
	IIC_Send_Byte(addr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte((Dev_addr<<1)|1);
	IIC_Wait_Ack();
	reg_data=IIC_Read_Byte(0);
	IIC_Stop();
	return reg_data;
}







