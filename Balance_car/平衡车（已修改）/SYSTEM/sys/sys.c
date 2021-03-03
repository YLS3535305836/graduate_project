#include "sys.h"

/**************************************************************************
�������ܣ�JTAGģʽ����,��������JTAG��ģʽ
��ڲ�����jtag,swdģʽ����;00,ȫʹ��;01,ʹ��SWD;10,ȫ�ر�;
					#define JTAG_SWD_DISABLE   0X02
					#define SWD_ENABLE         0X01
					#define JTAG_SWD_ENABLE    0X00
����  ֵ����
**************************************************************************/
void JTAG_Set(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;
	RCC->APB2ENR|=1<<0;     //��������ʱ��
	AFIO->MAPR&=0XF8FFFFFF; //���MAPR��[26:24]
	AFIO->MAPR|=temp;       //����jtagģʽ
}




























