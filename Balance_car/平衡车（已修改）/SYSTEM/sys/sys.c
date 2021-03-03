#include "sys.h"

/**************************************************************************
函数功能：JTAG模式设置,用于设置JTAG的模式
入口参数：jtag,swd模式设置;00,全使能;01,使能SWD;10,全关闭;
					#define JTAG_SWD_DISABLE   0X02
					#define SWD_ENABLE         0X01
					#define JTAG_SWD_ENABLE    0X00
返回  值：无
**************************************************************************/
void JTAG_Set(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;
	RCC->APB2ENR|=1<<0;     //开启辅助时钟
	AFIO->MAPR&=0XF8FFFFFF; //清除MAPR的[26:24]
	AFIO->MAPR|=temp;       //设置jtag模式
}




























