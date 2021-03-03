#ifndef __OLED_H
#define __OLED_H
#include "sys.h"

#define SIZE 8
#define Max_Column	128

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_6)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//SDA
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define OLED2_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_2)//SCL
#define OLED2_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_2)

#define OLED2_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_7)//SDA
#define OLED2_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_7)


//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void I2C_Start(void);
void I2C_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void I2C_Wait_Ack(void);

void OLED2_WR_Byte(unsigned dat,unsigned cmd);
//void OLED2_Init(void);
void OLED2_Clear(void);
void OLED2_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED2_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED2_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);
void OLED2_Set_Pos(unsigned char x, unsigned char y);
void OLED2_ShowCHinese(u8 x,u8 y,u8 no);
void I2C_2_Start(void);
void I2C_2_Stop(void);
void Write_IIC_2_Command(unsigned char IIC_Command);
void Write_IIC_2_Data(unsigned char IIC_Data);
void Write_IIC_2_Byte(unsigned char IIC_Byte);
void I2C_2_Wait_Ack(void);

void OLED_Clear_ALL(void);

#endif



