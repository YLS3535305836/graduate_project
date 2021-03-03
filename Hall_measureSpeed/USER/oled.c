#include "oled.h"

#include "oledfont.h"  	 




#if OLED_MODE==1






void OLED_WR_Byte(u8 dat,u8 cmd)
{
	DATAOUT(dat);	    
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		   
	OLED_CS_Clr();
	OLED_WR_Clr();	 
	OLED_WR_Set();
	OLED_CS_Set();	  
	OLED_DC_Set();	 
} 	    	    
#else
//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 
#endif
	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x02,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}

/* 绝对值函数 */
float myabs(float a)
{ 		   
	  float temp;
		if(a<0.0)  temp=-a;  
	  else temp=a;
	  return temp;
}

void OLED_ShowNegative(u8 x,u8 y,u8 k)
{
  if(k)
		OLED_ShowString(x,y,"-");
	else
		OLED_ShowString(x,y," ");
}

void OLED_DecimalShow(u8 x,u8 y,float num,u8 lenz,u8 lenx,u8 size)  //显示小数:lenz为整数位数；lenx为小数位数
{
	u32 a;
 	u8 i;
	a=num;
	OLED_ShowNum(x,y,a,lenz,size);
	num-=a;
	//a=num*1000;
	for(i=0;i<lenx;i++)
	{
		num*=10;
	}
	OLED_ShowString(x+8*lenz,y,"."); 
  OLED_ShowNum(x+8+8*lenz,y,num,lenx,size);
}

void OLED_ShowAngle(u8 x,u8 y,float num,u8 dian,u8 len ,u8 size)
{
  if(num>=0)
		 {
			 OLED_ShowNegative(x,y,0);
		   OLED_DecimalShow(x+16,y,num,dian,len,size);
		 }
		 else
		 {
			 OLED_ShowNegative(x,y,1);
			 OLED_DecimalShow(x+16,y,myabs(num),dian,len,size);
		 }
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
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
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
} 
//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
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
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 


void GPIO_init(GPIO_TypeDef* GPIOx,u8 pin,u8 mode)
{
	  switch((int)GPIOx)
		{
				case (int)GPIOA:
					RCC->APB2ENR|=1<<2;
					break;
				case (int)GPIOB:
					RCC->APB2ENR|=1<<3;
					break;
				case (int)GPIOC:
					RCC->APB2ENR|=1<<4;
					break;
				case (int)GPIOD:
					RCC->APB2ENR|=1<<5;
					break;
				case (int)GPIOE:
					RCC->APB2ENR|=1<<6;
					break;
				case (int)GPIOF:
					RCC->APB2ENR|=1<<7;
					break;
				
		}
		if(pin<8)
		{
				GPIOx->CRL&=~(0x0000000f<<(pin*4));
			  GPIOx->CRL|=(0x00000000|mode)<<(pin*4);
			  
		}
		else 
		{
				GPIOx->CRH&=~(0X0000000F<<( (pin-8)*4 ));
			  GPIOx->CRH|=(0x00000000|mode)<<( (pin-8)*4 );
		}
//		switch(pin)
//		{
//			case 0:   GPIOx->CRL&=0xfffffff0; break;
//			case 1:   GPIOx->CRL&=0xffffff0f; break;
//			case 2:   GPIOx->CRL&=0xfffff0ff; break;
//			case 3:   GPIOx->CRL&=0xffff0fff; break;
//			case 4:   GPIOx->CRL&=0xfff0ffff; break;
//			case 5:   GPIOx->CRL&=0xff0fffff; break;
//			case 6:   GPIOx->CRL&=0xf0ffffff; break;
//			case 7:   GPIOx->CRL&=0x0fffffff; break;
//			case 8:   GPIOx->CRH&=0xFFFFFFF0; break;
//			case 9:   GPIOx->CRL&=0xffffff0F; break;
//		}
			
}

//设置io的电平高低
//  ex:  GPIO_SET(GPIOA,5,1)    设置GPIOA的5口为高电平
void GPIO_SET(GPIO_TypeDef* GPIOx,u8 pin,u8 level)
{
		if(level==1)
		{
				GPIOx->ODR=1<<pin;
		}
		else 
		{
				GPIOx->ODR=~(1<<pin);
		}
}

//获得io的电平高低
//   ex:  temp=GPIO_SET(GPIOA,5)   读取GPIOA的5口的电平  
//   返回 1 高电平 0 低电平
u8 GPIO_GET(GPIO_TypeDef* GPIOx,u8 pin)
{
		if(GPIOx->ODR&1<<pin)
			return 1;
		else
			return 0;
}




//初始化SSD1306					    
void OLED_Init(void)
{ 	
		
		GPIO_init(GPIOA,8,Push50);
		GPIO_init(GPIOB,13,Push50);
		GPIO_init(GPIOB,14,Push50);
		GPIO_init(GPIOB,15,Push50);
	
		GPIO_SET(GPIOA,8,1);
		GPIO_SET(GPIOB,13,1);
		GPIO_SET(GPIOB,14,1);
		GPIO_SET(GPIOB,15,1);
 	 
// 		GPIO_InitTypeDef  GPIO_InitStructure;
// 	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PC,D,G端口时钟

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11;	 //PD3,PD9推挽输出  
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOD3,9
// 	GPIO_SetBits(GPIOB,GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11);	//PD3,PD9 输出高

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PC,D,G端口时钟

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	 //PD3,PD9推挽输出  
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOD3,9
// 	GPIO_SetBits(GPIOA,GPIO_Pin_15);	
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PC,D,G端口时钟

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	 //PD3,PD9推挽输出  
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
// 	GPIO_Init(GPIOD, &GPIO_InitStructure);	  //初始化GPIOD3,9
// 	GPIO_SetBits(GPIOD,GPIO_Pin_15);
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PC,D,G端口时钟

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	 //PD3,PD9推挽输出  
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
// 	GPIO_Init(GPIOE, &GPIO_InitStructure);	  //初始化GPIOD3,9
// 	GPIO_SetBits(GPIOE,GPIO_Pin_15);
	
	// #if OLED_MODE==1
	// 
	// 	GPIO_InitStructure.GPIO_Pin =0xFF; //PC0~7 OUT推挽输出
	// 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// 	GPIO_SetBits(GPIOC,0xFF); //PC0~7输出高

	// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //PG13,14,15 OUT推挽输出
	// 	GPIO_Init(GPIOG, &GPIO_InitStructure);
	// 	GPIO_SetBits(GPIOG,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);						 //PG13,14,15 OUT  输出高

	// #else
	// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;				 //PC0,1 OUT推挽输出
	// 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// 	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);						 //PC0,1 OUT  输出高

	//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //PG15 OUT推挽输出	  RST
	// 	GPIO_Init(GPIOG, &GPIO_InitStructure);
	// 	GPIO_SetBits(GPIOG,GPIO_Pin_15);						 //PG15 OUT  输出高


	// #endif


 
  OLED_RST_Set();

	OLED_RST_Clr();

	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x02,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
		OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
	OLED_WR_Byte(0xfF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
} 


