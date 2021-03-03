#include "control.h"

int Balance_Pwm,Velocity_Pwm,Turn_Pwm;	//ֱ��PD����PWM���ֵ �ٶ�PI����PWM���ֵ ת��P����PWM���ֵ
u8 Flag_Target;													//10msһ�ο��Ʊ�־λ
u32 Flash_R_Count;											//FLASH��ʱ��־λ
/*******************************************************************************************************
�������ܣ����еĿ��ƴ��� 5ms��ʱ�ж���MPU6050��INT���Ŵ��� �ϸ�֤���������ݴ����ʱ��ͬ��
��ڲ�������
����  ֵ����
*******************************************************************************************************/
int EXTI9_5_IRQHandler(void)
{    
	if(INT==0)																														//ȷ������������ж�
	{
		EXTI->PR=1<<5;                                                      //���LINE5�ϵ��жϱ�־λ
		Flag_Target=!Flag_Target;																						//10ms���Ʊ�־λ
		if(delay_flag==1)																										//������ѭ����־λ���� 1 �ȴ��жϷ��������ڴ��ж��м��������� 0 ������ѭ��
		{
		if(++delay_50==10)	 delay_50=0,delay_flag=0;                     	//����ѭ���ṩ50ms�ľ�׼��ʱ��50ms��һ����ѭ���������ݴ�����������ʾ
		}																																		//����һ��Ϊ5ms�жϣ�����10�μ�50ms�����50ms���ѭ����־λ��������ѭ��
		if(Flag_Target==1)                                                  //5ms��ȡһ�������Ǻͼ��ٶȼƵ�ֵ�����ߵĲ���Ƶ�ʿ��Ը��ƿ������˲��ͻ����˲���Ч��
		{
		Get_Angle();                                               					//������̬ �������˲�
		if(++Flash_R_Count==150&&Angle_Balance>30)Flash_Read();             //��ȡFlash��PID���� ������ʱ����Ƶ����ȡflash�������Ǵ���30�㣬˵��PID���󣬴�ʱ��ȡflash����ȷ����
		return 0;	                                               						//10ms���Ʊ�־λΪ1��ֱ��return����ѭ��֮�ȴ�ѭ���£��ȴ��´��жϷ�������10ms����һ��
		}                                                                   //10ms����һ�Σ�Ϊ�˱�֤M�����ٵ�ʱ���׼�����ȶ�ȡ����������
		Encoder_Left=-Read_Encoder(2);                                      //��ȡ��������ֵ����Ϊ�����������ת��180�ȵģ����Զ�����һ��ȡ������֤�������һ��
		Encoder_Right=Read_Encoder(4);                                      //��ȡ��������ֵ
		Get_Angle();                                               					//������̬	�������˲�
		Read_Distane();                                                     //��ȡ��������������ֵ
		if(Bi_zhang==0)Led_Flash(100);                                      //LED��˸;����ģʽ 1s�ı�һ��ָʾ�Ƶ�״̬
		if(Bi_zhang==1)Led_Flash(0);                                        //LED��˸;����ģʽ ָʾ�Ƴ���
		Key();                                                              //ɨ�谴��״̬ ����˫�����Ըı�С������״̬
		Balance_Pwm  = balance(Angle_Balance,Gyro_Balance);                 //ֱ��PD����
		Velocity_Pwm = velocity(Encoder_Left,Encoder_Right);                //�ٶ�PI����	�ٶȷ���Ϊ���������ٶȹ��������Ӧ����
		Turn_Pwm     = turn(Encoder_Left,Encoder_Right,Gyro_Turn);          //ת��P����
		Moto1=Balance_Pwm-Velocity_Pwm+Turn_Pwm;                            //�������ֵ������PWM
		Moto2=Balance_Pwm-Velocity_Pwm-Turn_Pwm;                            //�������ֵ������PWM
		Xianfu_Pwm();                                                       //PWM�޷���PWM������7200��������6900
		if(Pick_Up(Acceleration_Z,Angle_Balance,Encoder_Left,Encoder_Right))//����Ƿ�С��������
		Flag_Stop=1;	                                                      //������رյ��
		if(Put_Down(Angle_Balance,Encoder_Left,Encoder_Right))              //����Ƿ�С��������
		Flag_Stop=0;	                                                      //���������������
		if(Turn_Off(Angle_Balance)==0)                              				//�쳣��� ����Ƿ񳬹���Χ��ֹͣ��־λ�Ƿ���1
		Set_Pwm(Moto1,Moto2);                                               //��ֵ��PWM�Ĵ���
	}
	return 0;	  																													//��⵽�����ж���return��ԭѭ������
}

/**************************************************************************
�������ܣ�ֱ��PD����
��ڲ�������ǡ����ٶ�
����  ֵ��ֱ������PWM
**************************************************************************/
int balance(float Angle,float Gyro)
{
   float Bias;															//��ʵ�Ƕ�
	 int balance;															//����ֵ����Ϊֱ��PWM
	 Bias=Angle-ZHONGZHI;       							//���ƽ��ĽǶ���ֵ �ͻ�е��� ��ƫ��Ϊ0 ZHONGZHI�궨��Ϊ 3����control.h�޸�
	 balance=Balance_Kp*Bias+Gyro*Balance_Kd; //����ƽ����Ƶĵ��PWM
	 return balance;													//����ֱ��PWM
}

/**************************************************************************
�������ܣ��ٶ�PI���� �޸�ǰ�������ٶȣ�Target_VelocityΪ�н��ٶȱ�־�����Ϊ60�����
��ڲ��������ֱ����������ֱ�����
����  ֵ���ٶȿ���PWM
**************************************************************************/
int velocity(int encoder_left,int encoder_right)
{
    static float Velocity,Encoder_Least,Encoder,Movement;							//�����ٶȣ���ΪPWM��� �ٶ�ƫ�� λ��ƫ�� ң��ǰ������
	  static float Encoder_Integral,Target_Velocity;										//λ��ƫ����� �ٶ�ָ��
	  //������������������������������������ң��ǰ�����˲��֡�����������������������������������//					//��������������������������������������������������������������������������������������������
	  if(Bi_zhang==1&&Flag_sudu==1)  Target_Velocity=45;                //����������ģʽ,�Զ��������ģʽ
    else 	                         Target_Velocity=65;               //��ͨΪ����ģʽ
		if(1==Flag_Qian)    	Movement=Target_Velocity/Flag_sudu;	        //ǰ����־λ��1 ǰ�� Flag_suduĬ��Ϊ2
		else if(1==Flag_Hou)	Movement=-Target_Velocity/Flag_sudu;        //���˱�־λ��1 ���� Flag_suduĬ��Ϊ2
	  else  Movement=0;																									//���ź� ת����������ֹͣ
	  if(Bi_zhang==1&&Distance<500&&Flag_Left!=1&&Flag_Right!=1)        //���ϱ�־λ��1�ҷ�ң��ת���ʱ�򣬽������ģʽ
	  Movement=-Target_Velocity/Flag_sudu;															//����ģʽ ����
   //�����������������������������������������ٶ�PI��������������������������������������������������//					//��������������������������������������������������������������������������������������������
		Encoder_Least =(Encoder_Left+Encoder_Right)-0;                    //��ȡ�����ٶ�ƫ��=�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩
		Encoder *= 0.8;		                                                //һ�׵�ͨ�˲���
		Encoder += Encoder_Least*0.2;	                                    //һ�׵�ͨ�˲���
		Encoder_Integral +=Encoder;                                       //���ֳ�λ�� ����ʱ�䣺10ms
		Encoder_Integral=Encoder_Integral-Movement;                       //����ң�������ݣ�����ǰ������
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //�����޷�
		if(Encoder_Integral<-10000)	Encoder_Integral=-10000;              //�����޷�
		Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;        //�ٶȿ���
		if(Turn_Off(Angle_Balance)==1||Flag_Stop==1)  Encoder_Integral=0; //����رպ��������
	  return Velocity;																									//�����ٶ�ֵ���ڼ���PWM
}

/**************************************************************************
�������ܣ�ת�����  ת���ٶȱ�����Turn_Amplitude
��ڲ��������ֱ����������ֱ�������Z��������
����  ֵ��ת�����PWM
**************************************************************************/
int turn(int encoder_left,int encoder_right,float gyro)//ת�����
{
		static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.9,Turn_Count;	//ת��ָ�� ת��PWM����ֵ ��������ʱ ת��ת�� ת�����
	  float Turn_Amplitude=88/Flag_sudu,Kp=-42,Kd=0;     										 	//ת���ٶ� PD����
	  //����������������������������������������ң��������ת���֡���������������������������������������//						//��������������������������������������������������������������������������������������������
  	if(1==Flag_Left||1==Flag_Right)                      										//��һ������Ҫ�Ǹ�����תǰ���ٶȵ����ٶȵ���ʼ�ٶȣ�����С������Ӧ��
		{																																				//��⵽��ת����ת�ź�
			if(++Turn_Count==1)																										//ֻ����һ��
			Encoder_temp=myabs(encoder_left+encoder_right);												//����ת��ǰ���ٶ�
			Turn_Convert=50/Encoder_temp;																					//ת��ת��
			if(Turn_Convert<0.6)Turn_Convert=0.6;																	//������Ͳ���С��0.6
			if(Turn_Convert>3)Turn_Convert=3;																			//������߲��ô���3.0
		}
	  else																																		//������ת�ź�
		{
			Turn_Convert=0.9;																											//��Ϊ��ֵ
			Turn_Count=0;																													//��Ϊ��ֵ
			Encoder_temp=0;																												//��Ϊ��ֵ
		}
		if(1==Flag_Left)	           Turn_Target-=Turn_Convert;									//��ת
		else if(1==Flag_Right)	     Turn_Target+=Turn_Convert; 								//��ת
		else Turn_Target=0;																											//��ת
    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    					//ת���ٶ��޷�
	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;						//ת���ٶ��޷�
		if(Flag_Qian==1||Flag_Hou==1)  Kd=0.5;        													//����ǰ�������ź� ����Kd����
		else Kd=0;   																														//����ת���ʱ��ȡ�������ǵľ��� �е�ģ��PID��˼��
  	//������������������������������������������������ת��PD��������������������������������������������������//					//��������������������������������������������������������������������������������������������
		Turn=-Turn_Target*Kp -gyro*Kd;                 													//���Z�������ǽ���PD����
	  return Turn;																														//����ת��ֵ
}

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
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
�������ܣ�����PWM��ֵ
��ڲ�������
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(void)
{
	  int Amplitude=6900;    								//PWM������7200 ������6900
	  if(Flag_Qian==1)  Moto1-=DIFFERENCE;  //DIFFERENCE��һ������ƽ��С������ͻ�е��װ�����һ��������ֱ���������������С�����и��õ�һ���ԡ�
	  if(Flag_Hou==1)   Moto2+=DIFFERENCE;  //DIFFERENCE��һ������ƽ��С������ͻ�е��װ�����һ��������ֱ���������������С�����и��õ�һ���ԡ�
    if(Moto1<-Amplitude) Moto1=-Amplitude;//�޷�
		if(Moto1>Amplitude)  Moto1=Amplitude;	//�޷�
	  if(Moto2<-Amplitude) Moto2=-Amplitude;//�޷�
		if(Moto2>Amplitude)  Moto2=Amplitude;	//�޷�

}
/**************************************************************************
�������ܣ������޸�С������״̬
��ڲ�������
����  ֵ����
**************************************************************************/
void Key(void)
{
	u8 tmp,tmp2;
	tmp=click_N_Double(50); 				//��˫�����
	if(tmp==1)Flag_Stop=!Flag_Stop;	//��������С������ͣ
	if(tmp==2)Flag_Show=!Flag_Show;	//˫������С������ʾ״̬
	tmp2=Long_Press();              //�������
  if(tmp2==1) Bi_zhang=!Bi_zhang;	//��������С���Ƿ���볬��������ģʽ
}

/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ�������Ǻ͵�ѹ
����  ֵ��1���쳣  0������
**************************************************************************/
u8 Turn_Off(float angle)
{
	    u8 temp;
			if(angle<-40||angle>40||1==Flag_Stop)		//��Ǵ���40�Ȼ�Flag_Stop��1�رյ��
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
�������ܣ���ȡ�Ƕ�
��ڲ�������
����  ֵ����
**************************************************************************/
void Get_Angle()
{
	    float Accel_Y,Accel_X,Accel_Z,Gyro_Y,Gyro_Z;	//���������� X���߼��ٶ� Z���߼��ٶ� Y����ٶ� Z����ٶ�
	   	Temperature=Read_Temperature();      					//��ȡMPU6050�����¶ȴ��������ݣ����Ʊ�ʾ�����¶ȡ�

			Gyro_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L);    //��ȡY��������
			Gyro_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_L);    //��ȡZ��������
		  Accel_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L); //��ȡX����ٶȼ�
	  	Accel_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L); //��ȡZ����ٶȼ�
		  if(Gyro_Y>32768)  Gyro_Y-=65536;				//��������ת��  Ҳ��ͨ��shortǿ������ת��
			if(Gyro_Z>32768)  Gyro_Z-=65536;				//��������ת��
	  	if(Accel_X>32768) Accel_X-=65536;				//��������ת��
		  if(Accel_Z>32768) Accel_Z-=65536;				//��������ת��
			Gyro_Balance=-Gyro_Y;										//����ƽ����ٶ�
	   	Accel_Y=atan2(Accel_X,Accel_Z)*180/PI;	//�������
		  Gyro_Y=Gyro_Y/16.4;											//����������ת��
		  Kalman_Filter(Accel_Y,-Gyro_Y);					//�������˲�
	    Angle_Balance=angle;										//����ƽ�����
			Gyro_Turn=Gyro_Z;												//����ת����ٶ�
			Acceleration_Z=Accel_Z;									//����Z����ٶȼ�

}
/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{
	  int temp;
		if(a<0)  temp=-a;
	  else temp=a;
	  return temp;
}
/**************************************************************************
�������ܣ����С���Ƿ�����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int Pick_Up(float Acceleration,float Angle,int encoder_left,int encoder_right)
{
	 static u16 flag,count0,count1,count2;
	if(flag==0)                                                                   //��һ��
	 {
	      if(myabs(encoder_left)+myabs(encoder_right)<30)                         //����1��С���ӽ���ֹ
				count0++;
        else
        count0=0;
        if(count0>10)
		    flag=1,count0=0;
	 }
	 if(flag==1)                                                                  //����ڶ���
	 {
		    if(++count1>200)       count1=0,flag=0;                                 //��ʱ���ٵȴ�2000ms
	      if(Acceleration>26000&&(Angle>(-20+ZHONGZHI))&&(Angle<(20+ZHONGZHI)))   //����2��С������0�ȸ���������
		    flag=2;
	 }
	 if(flag==2)                                                                  //������
	 {
		  if(++count2>100)       count2=0,flag=0;                                   //��ʱ���ٵȴ�1000ms
	    if(myabs(encoder_left+encoder_right)>135)                                 //����3��С������̥��Ϊ�������ﵽ����ת��
      {
				flag=0;
				return 1;                                                               //��⵽С��������
			}
	 }
	return 0;
}
/**************************************************************************
�������ܣ����С���Ƿ񱻷���
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int Put_Down(float Angle,int encoder_left,int encoder_right)
{
	 static u16 flag,count;
	 if(Flag_Stop==0)                           								//��ֹ���
   return 0;
	 if(flag==0)
	 {
	      if(Angle>(-10+ZHONGZHI)&&Angle<(10+ZHONGZHI)&&encoder_left==0&&encoder_right==0)     //����1��С������0�ȸ�����
		    flag=1;
	 }
	 if(flag==1)
	 {
		  if(++count>50)                                          //��ʱ���ٵȴ� 500ms
		  {
				count=0;flag=0;
		  }
	    if(encoder_left>3&&encoder_right>3&&encoder_left<60&&encoder_right<60)                //����2��С������̥��δ�ϵ��ʱ����Ϊת��
      {
				flag=0;
				flag=0;
				return 1;                                             //��⵽С��������
			}
	 }
	return 0;
}


