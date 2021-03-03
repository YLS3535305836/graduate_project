#include "show.h"

unsigned char i;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
float Vol;
/**************************************************************************
函数功能：OLED显示
入口参数：无
返回  值：无
**************************************************************************/
void oled1_show(void)
{
										OLED_ShowCHinese(2,0,5);										//指
										OLED_ShowCHinese(20,0,6);										//示
										OLED_ShowCHinese(38,0,7);										//电
										OLED_ShowCHinese(56,0,8);										//机
										OLED_ShowCHinese(74,0,9);										//显
										OLED_ShowCHinese(92,0,10);									//示
										OLED_ShowCHinese(116,0,21);									//空位，清除OLED边白
										OLED_ShowString(3,2,"---------------",12);	//隔离标题数据
										OLED_ShowString(124,2," ",12);							//空位，清除OLED边白
if(Bi_zhang==1)			OLED_ShowString(3,3,"Bizhang(mm)",12);
else								OLED_ShowString(3,3,"Putong(mm)",12);
										OLED_ShowNum(100,3,(u16)Distance,5,12);
										OLED_ShowString(3,4,"Temperature",12);
										OLED_ShowString(118,4,".",12);
										OLED_ShowNum(106,4,Temperature/10,2,12);
										OLED_ShowNum(124,4,Temperature%10,1,12);
										OLED_ShowString(3,5,"EncoLEFT",12);
if( Encoder_Left<0)	OLED_ShowString(106,5,"-",12),
										OLED_ShowNum(112,5,-Encoder_Left,3,12);
else								OLED_ShowString(106,5,"+",12),
										OLED_ShowNum(112,5, Encoder_Left,3,12);
										OLED_ShowString(3,6,"EncoRIGHT",12);
if(Encoder_Right<0)	OLED_ShowString(106,6,"-",12),
										OLED_ShowNum(112,6,-Encoder_Right,3,12);
else								OLED_ShowString(106,6,"+",12),
										OLED_ShowNum(112,6,Encoder_Right,3,12);
										OLED_ShowString(3,7,"Angle",12);
if(Angle_Balance<0)	OLED_ShowNum(112,7,Angle_Balance+360,3,12);
else								OLED_ShowNum(112,7,Angle_Balance,3,12);
}

void oled2_show(void)
{
									OLED2_ShowCHinese(2,0,0);			//平
									OLED2_ShowCHinese(21,0,1);		//衡
									OLED2_ShowCHinese(39,0,2);		//车
									OLED2_ShowCHinese(57,0,3);		//状
									OLED2_ShowCHinese(76,0,4);		//态
									OLED2_ShowCHinese(96,0,5);		//指
									OLED2_ShowCHinese(115,0,6);		//示

									OLED2_ShowCHinese(2,2,7);			//电
									OLED2_ShowCHinese(18,2,8);		//机
									OLED2_ShowString(34,2,":",16);

									OLED2_ShowCHinese(74,2,9);		//显
									OLED2_ShowCHinese(90,2,10);		//示
									OLED2_ShowString(106,2,":",16);

									OLED2_ShowCHinese(2,4,11);		//循
									OLED2_ShowCHinese(18,4,12);		//迹
									OLED2_ShowString(34,4,":",16);

									OLED2_ShowCHinese(74,4,13);		//避
									OLED2_ShowCHinese(90,4,14);		//障
									OLED2_ShowString(106,4,":",16);

									OLED2_ShowCHinese(2,6,15);		//蓝
									OLED2_ShowCHinese(18,6,16);		//牙
									OLED2_ShowString(34,6,":",16);

									OLED2_ShowCHinese(74,6,17);		//串
									OLED2_ShowCHinese(90,6,18);		//口
									OLED2_ShowString(106,6,":",16);

if(Flag_Stop==0)																//电机开关标志位
									OLED2_ShowCHinese(42,2,19);		//电机开
else							OLED2_ShowCHinese(42,2,20);		//电机关


									OLED2_ShowCHinese(42,4,20);		//循迹关


if(Bi_zhang==1)																	//超声波避障标志位
									OLED2_ShowCHinese(114,4,19);	//避障开
else							OLED2_ShowCHinese(114,4,20);	//避障关

if(Flag_Show==0)																//数据发送及显示标志位
									OLED2_ShowCHinese(114,2,19),	//显示开
									OLED2_ShowCHinese(42,6,19),		//蓝牙开
									OLED2_ShowCHinese(114,6,20);	//串口关
else							OLED2_ShowCHinese(114,2,20),	//显示关
									OLED2_ShowCHinese(114,6,19),	//蓝牙关
									OLED2_ShowCHinese(42,6,20);		//串口开

}

/**************************************************************************
函数功能：向APP发送数据
入口参数：无
返回  值：无
**************************************************************************/
void APP_Show(void)
{
		u16 Voltage=1234;//(可以改）

	  static u8 flag;
	  int app_2,app_3,app_4;
		app_4=(Voltage-1110)*2/3;		if(app_4<0)app_4=0;if(app_4>100)app_4=100;   //对电压数据进行处理
		app_3=Encoder_Right*1.1; if(app_3<0)app_3=-app_3;			                   //对编码器数据就行数据处理便于图形化
		app_2=Encoder_Left*1.1;  if(app_2<0)app_2=-app_2;
	  flag=!flag;
	if(PID_Send==1)//发送PID参数
	{
		printf("{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$",(int)(Balance_Kp*100),(int)(Balance_Kd*100),(int)(Velocity_Kp*100),(int)(Velocity_Ki*100),0,0,0,0,0);//打印到APP上面
		PID_Send=0;
	}
   else	if(flag==0)//
   printf("{A%d:%d:%d:%d}$",(u8)app_2,(u8)app_3,app_4,(int)Angle_Balance); //打印到APP上面
	 else
	 printf("{B%d:%d:%d:%d}$",(int)Angle_Balance,Distance,Encoder_Left,Encoder_Right);//打印到APP上面 显示波形
}
/**************************************************************************
函数功能：虚拟示波器往上位机发送数据 关闭显示屏
入口参数：无
返回  值：无
**************************************************************************/
void DataScope(void)
{
		u16 Voltage=1600;																//(可以改）电压值
    Vol=(float)Voltage/100;

		DataScope_Get_Channel_Data( Angle_Balance, 1 ); //显示角度 单位：度（°）
		DataScope_Get_Channel_Data( Distance/10, 2 );   //显示超声波测量的距离 单位：CM
		DataScope_Get_Channel_Data( Vol, 3 );           //显示电池电压 单位：V
		DataScope_Get_Channel_Data( Temperature/10, 4 );//显示主板温度 单位：度（°）
		DataScope_Get_Channel_Data( Encoder_Left, 5 ); 	//显示左编码器
		DataScope_Get_Channel_Data( Encoder_Right, 6 );	//显示右编码器

		DataScope_Get_Channel_Data(0, 7 );							//用需要显示的数据替换0
		DataScope_Get_Channel_Data( 0, 8 ); 						//用需要显示的数据替换0
		DataScope_Get_Channel_Data(0, 9 );  						//用需要显示的数据替换0
		DataScope_Get_Channel_Data( 0 , 10);						//用需要显示的数据替换0

		Send_Count = DataScope_Data_Generate(6);				//选择发送数据个数
		for( i = 0 ; i < Send_Count; i++)
		{
		while((USART1->SR&0X40)==0);
		USART1->DR = DataScope_OutPut_Buffer[i];
		}
}
