#include "headfile.h"
extern u8 duty_S;
void key1_scan(void)
{
	  static unsigned char state1;
	  switch(state1)
		{
			case 0: if(KEY1==0)
			        {
			           state1=1;
                 break;
							}
			case 1: if(KEY1==0)
			        {
                 state1=2;
								 duty_S=1;
                 break;
							}
							else
							{
                 state1=0;
								 duty_S=0;
                 break;
							}
			case 2: if(KEY1==1)
			        {
                 state1=0;
                 break;
							}
			default:   break;
		}
}

void key2_scan(void)
{
	  static unsigned char state2;
	  switch(state2)
		{
			case 0: if(KEY4==0)
			        {
			           state2=1;
                 break;
							}
			case 1: if(KEY4==0)
			        {
                 state2=2;
								 duty_S=2;
                 break;
							}
							else
							{
                 state2=0;
								 duty_S=0;
                 break;
							}
			case 2: if(KEY4==1)
			        {
                 state2=0;
                 break;
							}
			default:   break;
		}
}
								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTBʱ��

//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;//GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11;//KEY1-KEY4
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOB
	  GPIO_init(GPIOA,8,8);
		GPIO_init(GPIOB,13,8);
		GPIO_init(GPIOB,14,8);
		GPIO_init(GPIOB,15,8);
	
		GPIO_SET(GPIOA,8,1);
		GPIO_SET(GPIOB,13,1);
		GPIO_SET(GPIOB,14,1);
		GPIO_SET(GPIOB,15,1);

}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY1����
//2��KEY2����
//3��KEY3���� 
//4��KEY4����
//ע��˺�������Ӧ���ȼ�,KEY1>KEY2>KEY3>KEY4!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
		else if(KEY3==0)return KEY3_PRES;
		else if(KEY4==0)return KEY4_PRES;
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1; 	    
 	return 0;// �ް�������
}
