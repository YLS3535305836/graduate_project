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
								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTB时钟

//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;//GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11;//KEY1-KEY4
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOB
	  GPIO_init(GPIOA,8,8);
		GPIO_init(GPIOB,13,8);
		GPIO_init(GPIOB,14,8);
		GPIO_init(GPIOB,15,8);
	
		GPIO_SET(GPIOA,8,1);
		GPIO_SET(GPIOB,13,1);
		GPIO_SET(GPIOB,14,1);
		GPIO_SET(GPIOB,15,1);

}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY1按下
//2，KEY2按下
//3，KEY3按下 
//4，KEY4按下
//注意此函数有响应优先级,KEY1>KEY2>KEY3>KEY4!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
		else if(KEY3==0)return KEY3_PRES;
		else if(KEY4==0)return KEY4_PRES;
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1; 	    
 	return 0;// 无按键按下
}
