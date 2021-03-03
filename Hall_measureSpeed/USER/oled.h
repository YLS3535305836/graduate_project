//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PD9（SCL）
//              D1   接PD7（SDA）
//              RES  接PD4
//              DC   接PD5
//              CS   接PD3               
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/
#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    	
//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    			

#define  imitate   0   //模拟输入
#define  input_h   8   //上下拉输入
#define  Float     4   //浮空输入

#define  Push10     1   //推挽输出10
#define  Push20     2   //推挽输出20
#define  Push50     3   //推挽输出50

#define  O_dr10     5   //开漏输出10
#define  O_dr20     6   //开漏输出20
#define  O_dr50     7   //开漏输出50

#define  M_Push10     9    //推挽输出10   //复用
#define  M_Push20     10   //推挽输出20
#define  M_Push50     11   //推挽输出50

#define  MO_dr10     13   //开漏输出10      //复用
#define  MO_dr20     14   //开漏输出20
#define  MO_dr50     15   //开漏输出50
//-----------------OLED端口定义----------------  					   
//#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_11)//CS
//#define OLED_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_11)

//#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_13)//RES
//#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_13)

//#define OLED_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_12)//DC
//#define OLED_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_12)

//#define OLED_WR_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_10)
//#define OLED_WR_Set() GPIO_SetBits(GPIOC,GPIO_Pin_10)

//#define OLED_RD_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_11)
//#define OLED_RD_Set() GPIO_SetBits(GPIOC,GPIO_Pin_11)

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_8)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_8)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_15)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_15)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_14)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_14)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_13)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_13)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_15)



//PC0~7,作为数据线
#define DATAOUT(x) GPIO_Write(GPIOC,x);//输出  
//使用4线串行接口时使用 

//#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_15)//CLK
//#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_15)

//#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_14)//DIN
//#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_14)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


void GPIO_init(GPIO_TypeDef* GPIOx,u8 pin,u8 mode);

void GPIO_SET(GPIO_TypeDef* GPIOx,u8 pin,u8 level);

//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_ShowAngle(u8 x,u8 y,float num,u8 dian,u8 len ,u8 size);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 



