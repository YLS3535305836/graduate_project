#include "oled.h"
#include "oledfont.h"

/******************************************************************************
��������:	����IIC��ʼ�ź�
��ڲ�������
����  ֵ����
*******************************************************************************/
void I2C_Start()
{
	OLED_SCLK_Set();
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

void I2C_2_Start()
{
	OLED2_SCLK_Set();
	OLED2_SDIN_Set();
	OLED2_SDIN_Clr();
	OLED2_SCLK_Clr();
}
/******************************************************************************
��������:	����IICֹͣ�ź�
��ڲ�������
����  ֵ����
*******************************************************************************/
void I2C_Stop()
{
	OLED_SCLK_Set() ;
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
}

void I2C_2_Stop()
{
	OLED2_SCLK_Set() ;
	OLED2_SDIN_Clr();
	OLED2_SDIN_Set();
}
/******************************************************************************
��������:	����ACKӦ��
��ڲ�������
����  ֵ����
*******************************************************************************/
void I2C_Wait_Ack()
{
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}

void I2C_2_Wait_Ack()
{
	OLED2_SCLK_Set() ;
	OLED2_SCLK_Clr();
}
/******************************************************************************
��������:	IIC����һ���ֽ�
��ڲ�����
����  ֵ����
*******************************************************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)
	{
		m=da;
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
		da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
		}
}

void Write_IIC_2_Byte(unsigned char IIC_2_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_2_Byte;
	OLED2_SCLK_Clr();
	for(i=0;i<8;i++)
	{
		m=da;
		m=m&0x80;
		if(m==0x80)
		{OLED2_SDIN_Set();}
		else OLED2_SDIN_Clr();
		da=da<<1;
		OLED2_SCLK_Set();
		OLED2_SCLK_Clr();
		}
}
/******************************************************************************
��������:	IICд����
��ڲ�����
����  ֵ����
*******************************************************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   I2C_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
	I2C_Wait_Ack();
   Write_IIC_Byte(0x00);			//write command
	I2C_Wait_Ack();
   Write_IIC_Byte(IIC_Command);
	I2C_Wait_Ack();
   I2C_Stop();
}

void Write_IIC_2_Command(unsigned char IIC_2_Command)
{
   I2C_2_Start();
   Write_IIC_2_Byte(0x78);            //Slave address,SA0=0
	I2C_2_Wait_Ack();
   Write_IIC_2_Byte(0x00);			//write command
	I2C_2_Wait_Ack();
   Write_IIC_2_Byte(IIC_2_Command);
	I2C_2_Wait_Ack();
   I2C_2_Stop();
}
/******************************************************************************
��������:	IICд����
��ڲ�����
����  ֵ����
*******************************************************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   I2C_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	I2C_Wait_Ack();
   Write_IIC_Byte(0x40);			//write data
	I2C_Wait_Ack();
   Write_IIC_Byte(IIC_Data);
	I2C_Wait_Ack();
   I2C_Stop();
}

void Write_IIC_2_Data(unsigned char IIC_2_Data)
{
   I2C_2_Start();
   Write_IIC_2_Byte(0x78);			//D/C#=0; R/W#=0
	I2C_2_Wait_Ack();
   Write_IIC_2_Byte(0x40);			//write data
	I2C_2_Wait_Ack();
   Write_IIC_2_Byte(IIC_2_Data);
	I2C_2_Wait_Ack();
   I2C_2_Stop();
}
/******************************************************************************
��������:	IICд ���� ����
��ڲ�����
����  ֵ����
*******************************************************************************/
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
   Write_IIC_Data(dat);
	else
   Write_IIC_Command(dat);
}

void OLED2_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
   Write_IIC_2_Data(dat);
	else
   Write_IIC_2_Command(dat);
}
/******************************************************************************
��������:	OLED��������
��ڲ�����
����  ֵ����
*******************************************************************************/
	void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD);
}

	void OLED2_Set_Pos(unsigned char x, unsigned char y)
{
	OLED2_WR_Byte(0xb0+y,OLED_CMD);
	OLED2_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED2_WR_Byte((x&0x0f),OLED_CMD);
}
/******************************************************************************
��������:	��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
��ڲ�����
����  ֵ����
*******************************************************************************/
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA);
	} //������ʾ
}

void OLED2_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED2_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED2_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED2_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ
		for(n=0;n<128;n++)OLED2_WR_Byte(0,OLED_DATA);
	} //������ʾ
}
/******************************************************************************
��������:	��ָ��λ����ʾһ���ַ�,���������ַ�
��ڲ�����x:0~127 y:0~63 size:ѡ������ 16/12
����  ֵ����
*******************************************************************************/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{
	unsigned char c=0,i=0;
		c=chr-' ';//�õ�ƫ�ƺ��ֵ
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else
				{
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				}
}

void OLED2_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{
	unsigned char c=0,i=0;
		c=chr-' ';//�õ�ƫ�ƺ��ֵ
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED2_Set_Pos(x,y);
			for(i=0;i<8;i++)
			OLED2_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED2_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED2_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else
				{
				OLED2_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED2_WR_Byte(F6x8[c][i],OLED_DATA);
				}
}
/******************************************************************************
��������:	m^n����
��ڲ�����
����  ֵ��
*******************************************************************************/
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)result*=m;
	return result;
}
/******************************************************************************
��������:	��ʾ2������
��ڲ�����x,y:������� len:���ֵ�λ�� size:�����С num:��ֵ(0~4294967295);
����  ֵ����
*******************************************************************************/
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{
	u8 t,temp;
	u8 enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1;

		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2);
	}
}

void OLED2_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{
	u8 t,temp;
	u8 enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED2_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1;

		}
	 	OLED2_ShowChar(x+(size2/2)*t,y,temp+'0',size2);
	}
}
/******************************************************************************
��������:	��ʾһ���ַ��Ŵ�
��ڲ�����
����  ֵ����
*******************************************************************************/
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

void OLED2_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED2_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
/******************************************************************************
��������:	��ʾ����
��ڲ�����
����  ֵ����
*******************************************************************************/
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{
	u8 t,adder=0;
	OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }
		OLED_Set_Pos(x,y+1);
    for(t=0;t<16;t++)
			{
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }
}

void OLED2_ShowCHinese(u8 x,u8 y,u8 no)
{
	u8 t,adder=0;
	OLED2_Set_Pos(x,y);
    for(t=0;t<16;t++)
		{
				OLED2_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }
		OLED2_Set_Pos(x,y+1);
    for(t=0;t<16;t++)
			{
				OLED2_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }
}
/******************************************************************************
��������:	������������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
��ڲ�����
����  ֵ����
*******************************************************************************/
void OLED_Clear_ALL(void)
{
		OLED_Clear();
		OLED2_Clear();
}
/******************************************************************************
��������:	��ʼ��SSD1306
��ڲ�����
����  ֵ����
*******************************************************************************/
void OLED_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��AB�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;							//GPIOA 5 6 7
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 												//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//�ٶ�50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  																	//��ʼ��GPIOA 5 6 7
 	GPIO_SetBits(GPIOA,GPIO_Pin_3|GPIO_Pin_4);																	//�øߵ�ƽ����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;																		//GPIOB 2
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  																	//��ʼ��GPIOB 2
	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4);																	//�øߵ�ƽ����

	delay_ms(800);

	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD);// contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//

	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//

	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//

	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//

	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//

	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//

	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//

	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel





	OLED2_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED2_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED2_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED2_WR_Byte(0x40,OLED_CMD);//--set start line address
	OLED2_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED2_WR_Byte(0x81,OLED_CMD);// contract control
	OLED2_WR_Byte(0xFF,OLED_CMD);//--128
	OLED2_WR_Byte(0xA1,OLED_CMD);//set segment remap
	OLED2_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED2_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED2_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED2_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED2_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED2_WR_Byte(0x00,OLED_CMD);//

	OLED2_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED2_WR_Byte(0x80,OLED_CMD);//

	OLED2_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED2_WR_Byte(0x05,OLED_CMD);//

	OLED2_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED2_WR_Byte(0xF1,OLED_CMD);//

	OLED2_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED2_WR_Byte(0x12,OLED_CMD);//

	OLED2_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED2_WR_Byte(0x30,OLED_CMD);//

	OLED2_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED2_WR_Byte(0x14,OLED_CMD);//

	OLED2_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	
	
	OLED_Clear_ALL();																	//��ʼ������������ʾ��

}
