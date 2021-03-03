#include "control.h"

int Balance_Pwm,Velocity_Pwm,Turn_Pwm;	//直立PD控制PWM输出值 速度PI控制PWM输出值 转向P控制PWM输出值
u8 Flag_Target;													//10ms一次控制标志位
u32 Flash_R_Count;											//FLASH延时标志位
/*******************************************************************************************************
函数功能：所有的控制代码 5ms定时中断由MPU6050的INT引脚触发 严格保证采样和数据处理的时间同步
入口参数：无
返回  值：无
*******************************************************************************************************/
int EXTI9_5_IRQHandler(void)
{    
	if(INT==0)																														//确认陀螺仪输出中断
	{
		EXTI->PR=1<<5;                                                      //清除LINE5上的中断标志位
		Flag_Target=!Flag_Target;																						//10ms控制标志位
		if(delay_flag==1)																										//进入主循环标志位，置 1 等待中断发生（并在此中断中计数），置 0 进入主循环
		{
		if(++delay_50==10)	 delay_50=0,delay_flag=0;                     	//给主循环提供50ms的精准延时，50ms进一次主循环进行数据传输与数据显示
		}																																		//运行一次为5ms中断，运行10次即50ms，随后50ms与大循环标志位清零进入大循环
		if(Flag_Target==1)                                                  //5ms读取一次陀螺仪和加速度计的值，更高的采样频率可以改善卡尔曼滤波和互补滤波的效果
		{
		Get_Angle();                                               					//更新姿态 卡尔曼滤波
		if(++Flash_R_Count==150&&Angle_Balance>30)Flash_Read();             //读取Flash的PID参数 运用延时避免频繁读取flash，如果倾角大于30°，说明PID有误，此时读取flash中正确参数
		return 0;	                                               						//10ms控制标志位为1，直接return到主循环之等待循环下，等待下次中断发生，即10ms控制一次
		}                                                                   //10ms控制一次，为了保证M法测速的时间基准，首先读取编码器数据
		Encoder_Left=-Read_Encoder(2);                                      //读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
		Encoder_Right=Read_Encoder(4);                                      //读取编码器的值
		Get_Angle();                                               					//更新姿态	卡尔曼滤波
		Read_Distane();                                                     //获取超声波测量距离值
		if(Bi_zhang==0)Led_Flash(100);                                      //LED闪烁;常规模式 1s改变一次指示灯的状态
		if(Bi_zhang==1)Led_Flash(0);                                        //LED闪烁;避障模式 指示灯常亮
		Key();                                                              //扫描按键状态 单击双击可以改变小车运行状态
		Balance_Pwm  = balance(Angle_Balance,Gyro_Balance);                 //直立PD控制
		Velocity_Pwm = velocity(Encoder_Left,Encoder_Right);                //速度PI控制	速度反馈为正反馈，速度过快减速则应加速
		Turn_Pwm     = turn(Encoder_Left,Encoder_Right,Gyro_Turn);          //转向P控制
		Moto1=Balance_Pwm-Velocity_Pwm+Turn_Pwm;                            //计算左轮电机最终PWM
		Moto2=Balance_Pwm-Velocity_Pwm-Turn_Pwm;                            //计算右轮电机最终PWM
		Xianfu_Pwm();                                                       //PWM限幅，PWM满幅是7200，限制在6900
		if(Pick_Up(Acceleration_Z,Angle_Balance,Encoder_Left,Encoder_Right))//检测是否小车被拿起
		Flag_Stop=1;	                                                      //被拿则关闭电机
		if(Put_Down(Angle_Balance,Encoder_Left,Encoder_Right))              //检测是否小车被放下
		Flag_Stop=0;	                                                      //被放下则启动电机
		if(Turn_Off(Angle_Balance)==0)                              				//异常检测 倾角是否超过范围与停止标志位是否被置1
		Set_Pwm(Moto1,Moto2);                                               //赋值给PWM寄存器
	}
	return 0;	  																													//检测到误入中断则return到原循环函数
}

/**************************************************************************
函数功能：直立PD控制
入口参数：倾角、角速度
返回  值：直立控制PWM
**************************************************************************/
int balance(float Angle,float Gyro)
{
   float Bias;															//真实角度
	 int balance;															//返回值，作为直立PWM
	 Bias=Angle-ZHONGZHI;       							//求出平衡的角度中值 和机械相关 即偏差为0 ZHONGZHI宏定义为 3，到control.h修改
	 balance=Balance_Kp*Bias+Gyro*Balance_Kd; //计算平衡控制的电机PWM
	 return balance;													//返回直立PWM
}

/**************************************************************************
函数功能：速度PI控制 修改前进后退速度，Target_Velocity为行进速度标志，如改为60则低速
入口参数：左轮编码器、右轮编码器
返回  值：速度控制PWM
**************************************************************************/
int velocity(int encoder_left,int encoder_right)
{
    static float Velocity,Encoder_Least,Encoder,Movement;							//返回速度，作为PWM输出 速度偏差 位置偏差 遥控前进后退
	  static float Encoder_Integral,Target_Velocity;										//位置偏差积分 速度指标
	  //——————————————————遥控前进后退部分——————————————————//					//——————————————————————————————————————————————
	  if(Bi_zhang==1&&Flag_sudu==1)  Target_Velocity=45;                //如果进入避障模式,自动进入低速模式
    else 	                         Target_Velocity=65;               //普通为高速模式
		if(1==Flag_Qian)    	Movement=Target_Velocity/Flag_sudu;	        //前进标志位置1 前进 Flag_sudu默认为2
		else if(1==Flag_Hou)	Movement=-Target_Velocity/Flag_sudu;        //后退标志位置1 后退 Flag_sudu默认为2
	  else  Movement=0;																									//无信号 转动变量置零停止
	  if(Bi_zhang==1&&Distance<500&&Flag_Left!=1&&Flag_Right!=1)        //避障标志位置1且非遥控转弯的时候，进入避障模式
	  Movement=-Target_Velocity/Flag_sudu;															//避障模式 后退
   //————————————————————速度PI控制器——————————————————————//					//——————————————————————————————————————————————
		Encoder_Least =(Encoder_Left+Encoder_Right)-0;                    //获取最新速度偏差=测量速度（左右编码器之和）-目标速度（此处为零）
		Encoder *= 0.8;		                                                //一阶低通滤波器
		Encoder += Encoder_Least*0.2;	                                    //一阶低通滤波器
		Encoder_Integral +=Encoder;                                       //积分出位移 积分时间：10ms
		Encoder_Integral=Encoder_Integral-Movement;                       //接收遥控器数据，控制前进后退
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //积分限幅
		if(Encoder_Integral<-10000)	Encoder_Integral=-10000;              //积分限幅
		Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;        //速度控制
		if(Turn_Off(Angle_Balance)==1||Flag_Stop==1)  Encoder_Integral=0; //电机关闭后清除积分
	  return Velocity;																									//返回速度值用于计算PWM
}

/**************************************************************************
函数功能：转向控制  转向速度变量：Turn_Amplitude
入口参数：左轮编码器、右轮编码器、Z轴陀螺仪
返回  值：转向控制PWM
**************************************************************************/
int turn(int encoder_left,int encoder_right,float gyro)//转向控制
{
		static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.9,Turn_Count;	//转向指标 转向PWM返回值 编码器临时 转向转换 转向计算
	  float Turn_Amplitude=88/Flag_sudu,Kp=-42,Kd=0;     										 	//转向速度 PD参数
	  //————————————————————遥控左右旋转部分————————————————————//						//——————————————————————————————————————————————
  	if(1==Flag_Left||1==Flag_Right)                      										//这一部分主要是根据旋转前的速度调整速度的起始速度，增加小车的适应性
		{																																				//检测到左转或右转信号
			if(++Turn_Count==1)																										//只运行一遍
			Encoder_temp=myabs(encoder_left+encoder_right);												//计算转向前的速度
			Turn_Convert=50/Encoder_temp;																					//转向转换
			if(Turn_Convert<0.6)Turn_Convert=0.6;																	//限制最低不得小于0.6
			if(Turn_Convert>3)Turn_Convert=3;																			//限制最高不得大于3.0
		}
	  else																																		//无左右转信号
		{
			Turn_Convert=0.9;																											//设为初值
			Turn_Count=0;																													//设为初值
			Encoder_temp=0;																												//设为初值
		}
		if(1==Flag_Left)	           Turn_Target-=Turn_Convert;									//左转
		else if(1==Flag_Right)	     Turn_Target+=Turn_Convert; 								//右转
		else Turn_Target=0;																											//不转
    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    					//转向速度限幅
	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;						//转向速度限幅
		if(Flag_Qian==1||Flag_Hou==1)  Kd=0.5;        													//若有前进后退信号 设置Kd参数
		else Kd=0;   																														//否则转向的时候取消陀螺仪的纠正 有点模糊PID的思想
  	//————————————————————————转向PD控制器——————————————————————//					//——————————————————————————————————————————————
		Turn=-Turn_Target*Kp -gyro*Kd;                 													//结合Z轴陀螺仪进行PD控制
	  return Turn;																														//返回转向值
}

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int moto1,int moto2)
{
    	if(moto1<0)			AIN2=1,			AIN1=0;
			else 	          AIN2=0,			AIN1=1;
			PWMA=myabs(moto1);
		  if(moto2<0)	BIN1=0,			BIN2=1;
			else        BIN1=1,			BIN2=0;
			PWMB=myabs(moto2);
}

/**************************************************************************
函数功能：限制PWM赋值
入口参数：无
返回  值：无
**************************************************************************/
void Xianfu_Pwm(void)
{
	  int Amplitude=6900;    								//PWM满幅是7200 限制在6900
	  if(Flag_Qian==1)  Moto1-=DIFFERENCE;  //DIFFERENCE是一个衡量平衡小车电机和机械安装差异的一个变量。直接作用于输出，让小车具有更好的一致性。
	  if(Flag_Hou==1)   Moto2+=DIFFERENCE;  //DIFFERENCE是一个衡量平衡小车电机和机械安装差异的一个变量。直接作用于输出，让小车具有更好的一致性。
    if(Moto1<-Amplitude) Moto1=-Amplitude;//限幅
		if(Moto1>Amplitude)  Moto1=Amplitude;	//限幅
	  if(Moto2<-Amplitude) Moto2=-Amplitude;//限幅
		if(Moto2>Amplitude)  Moto2=Amplitude;	//限幅

}
/**************************************************************************
函数功能：按键修改小车运行状态
入口参数：无
返回  值：无
**************************************************************************/
void Key(void)
{
	u8 tmp,tmp2;
	tmp=click_N_Double(50); 				//单双击检测
	if(tmp==1)Flag_Stop=!Flag_Stop;	//单击控制小车的启停
	if(tmp==2)Flag_Show=!Flag_Show;	//双击控制小车的显示状态
	tmp2=Long_Press();              //长按检测
  if(tmp2==1) Bi_zhang=!Bi_zhang;	//长按控制小车是否进入超声波避障模式
}

/**************************************************************************
函数功能：异常关闭电机
入口参数：倾角和电压
返回  值：1：异常  0：正常
**************************************************************************/
u8 Turn_Off(float angle)
{
	    u8 temp;
			if(angle<-40||angle>40||1==Flag_Stop)		//倾角大于40度或Flag_Stop置1关闭电机
			{
      temp=1;
			AIN1=0;
			AIN2=0;
			BIN1=0;
			BIN2=0;
      }
			else
      temp=0;
      return temp;
}

/**************************************************************************
函数功能：获取角度
入口参数：无
返回  值：无
**************************************************************************/
void Get_Angle()
{
	    float Accel_Y,Accel_X,Accel_Z,Gyro_Y,Gyro_Z;	//计算出的倾角 X轴线加速度 Z轴线加速度 Y轴角速度 Z轴角速度
	   	Temperature=Read_Temperature();      					//读取MPU6050内置温度传感器数据，近似表示主板温度。

			Gyro_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L);    //读取Y轴陀螺仪
			Gyro_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_L);    //读取Z轴陀螺仪
		  Accel_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L); //读取X轴加速度计
	  	Accel_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L); //读取Z轴加速度计
		  if(Gyro_Y>32768)  Gyro_Y-=65536;				//数据类型转换  也可通过short强制类型转换
			if(Gyro_Z>32768)  Gyro_Z-=65536;				//数据类型转换
	  	if(Accel_X>32768) Accel_X-=65536;				//数据类型转换
		  if(Accel_Z>32768) Accel_Z-=65536;				//数据类型转换
			Gyro_Balance=-Gyro_Y;										//更新平衡角速度
	   	Accel_Y=atan2(Accel_X,Accel_Z)*180/PI;	//计算倾角
		  Gyro_Y=Gyro_Y/16.4;											//陀螺仪量程转换
		  Kalman_Filter(Accel_Y,-Gyro_Y);					//卡尔曼滤波
	    Angle_Balance=angle;										//更新平衡倾角
			Gyro_Turn=Gyro_Z;												//更新转向角速度
			Acceleration_Z=Accel_Z;									//更新Z轴加速度计

}
/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{
	  int temp;
		if(a<0)  temp=-a;
	  else temp=a;
	  return temp;
}
/**************************************************************************
函数功能：检测小车是否被拿起
入口参数：int
返回  值：unsigned int
**************************************************************************/
int Pick_Up(float Acceleration,float Angle,int encoder_left,int encoder_right)
{
	 static u16 flag,count0,count1,count2;
	if(flag==0)                                                                   //第一步
	 {
	      if(myabs(encoder_left)+myabs(encoder_right)<30)                         //条件1，小车接近静止
				count0++;
        else
        count0=0;
        if(count0>10)
		    flag=1,count0=0;
	 }
	 if(flag==1)                                                                  //进入第二步
	 {
		    if(++count1>200)       count1=0,flag=0;                                 //超时不再等待2000ms
	      if(Acceleration>26000&&(Angle>(-20+ZHONGZHI))&&(Angle<(20+ZHONGZHI)))   //条件2，小车是在0度附近被拿起
		    flag=2;
	 }
	 if(flag==2)                                                                  //第三步
	 {
		  if(++count2>100)       count2=0,flag=0;                                   //超时不再等待1000ms
	    if(myabs(encoder_left+encoder_right)>135)                                 //条件3，小车的轮胎因为正反馈达到最大的转速
      {
				flag=0;
				return 1;                                                               //检测到小车被拿起
			}
	 }
	return 0;
}
/**************************************************************************
函数功能：检测小车是否被放下
入口参数：int
返回  值：unsigned int
**************************************************************************/
int Put_Down(float Angle,int encoder_left,int encoder_right)
{
	 static u16 flag,count;
	 if(Flag_Stop==0)                           								//防止误检
   return 0;
	 if(flag==0)
	 {
	      if(Angle>(-10+ZHONGZHI)&&Angle<(10+ZHONGZHI)&&encoder_left==0&&encoder_right==0)     //条件1，小车是在0度附近的
		    flag=1;
	 }
	 if(flag==1)
	 {
		  if(++count>50)                                          //超时不再等待 500ms
		  {
				count=0;flag=0;
		  }
	    if(encoder_left>3&&encoder_right>3&&encoder_left<60&&encoder_right<60)                //条件2，小车的轮胎在未上电的时候被人为转动
      {
				flag=0;
				flag=0;
				return 1;                                             //检测到小车被放下
			}
	 }
	return 0;
}


