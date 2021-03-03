//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PD9��SCL��
//              D1   ��PD7��SDA��
//              RES  ��PD4
//              DC   ��PD5
//              CS   ��PD3               
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/
#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    	
//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    			

#define  imitate   0   //ģ������
#define  input_h   8   //����������
#define  Float     4   //��������

#define  Push10     1   //�������10
#define  Push20     2   //�������20
#define  Push50     3   //�������50

#define  O_dr10     5   //��©���10
#define  O_dr20     6   //��©���20
#define  O_dr50     7   //��©���50

#define  M_Push10     9    //�������10   //����
#define  M_Push20     10   //�������20
#define  M_Push50     11   //�������50

#define  MO_dr10     13   //��©���10      //����
#define  MO_dr20     14   //��©���20
#define  MO_dr50     15   //��©���50
//-----------------OLED�˿ڶ���----------------  					   
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



//PC0~7,��Ϊ������
#define DATAOUT(x) GPIO_Write(GPIOC,x);//���  
//ʹ��4�ߴ��нӿ�ʱʹ�� 

//#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_15)//CLK
//#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_15)

//#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_14)//DIN
//#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_14)

 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


void GPIO_init(GPIO_TypeDef* GPIOx,u8 pin,u8 mode);

void GPIO_SET(GPIO_TypeDef* GPIOx,u8 pin,u8 level);

//OLED�����ú���
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
	 



