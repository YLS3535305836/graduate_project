#include "show.h"

unsigned char i;          //��������
unsigned char Send_Count; //������Ҫ���͵����ݸ���
float Vol;
/**************************************************************************
�������ܣ�OLED��ʾ
��ڲ�������
����  ֵ����
**************************************************************************/
void oled1_show(void)
{
										OLED_ShowCHinese(2,0,5);										//ָ
										OLED_ShowCHinese(20,0,6);										//ʾ
										OLED_ShowCHinese(38,0,7);										//��
										OLED_ShowCHinese(56,0,8);										//��
										OLED_ShowCHinese(74,0,9);										//��
										OLED_ShowCHinese(92,0,10);									//ʾ
										OLED_ShowCHinese(116,0,21);									//��λ�����OLED�߰�
										OLED_ShowString(3,2,"---------------",12);	//�����������
										OLED_ShowString(124,2," ",12);							//��λ�����OLED�߰�
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
									OLED2_ShowCHinese(2,0,0);			//ƽ
									OLED2_ShowCHinese(21,0,1);		//��
									OLED2_ShowCHinese(39,0,2);		//��
									OLED2_ShowCHinese(57,0,3);		//״
									OLED2_ShowCHinese(76,0,4);		//̬
									OLED2_ShowCHinese(96,0,5);		//ָ
									OLED2_ShowCHinese(115,0,6);		//ʾ

									OLED2_ShowCHinese(2,2,7);			//��
									OLED2_ShowCHinese(18,2,8);		//��
									OLED2_ShowString(34,2,":",16);

									OLED2_ShowCHinese(74,2,9);		//��
									OLED2_ShowCHinese(90,2,10);		//ʾ
									OLED2_ShowString(106,2,":",16);

									OLED2_ShowCHinese(2,4,11);		//ѭ
									OLED2_ShowCHinese(18,4,12);		//��
									OLED2_ShowString(34,4,":",16);

									OLED2_ShowCHinese(74,4,13);		//��
									OLED2_ShowCHinese(90,4,14);		//��
									OLED2_ShowString(106,4,":",16);

									OLED2_ShowCHinese(2,6,15);		//��
									OLED2_ShowCHinese(18,6,16);		//��
									OLED2_ShowString(34,6,":",16);

									OLED2_ShowCHinese(74,6,17);		//��
									OLED2_ShowCHinese(90,6,18);		//��
									OLED2_ShowString(106,6,":",16);

if(Flag_Stop==0)																//������ر�־λ
									OLED2_ShowCHinese(42,2,19);		//�����
else							OLED2_ShowCHinese(42,2,20);		//�����


									OLED2_ShowCHinese(42,4,20);		//ѭ����


if(Bi_zhang==1)																	//���������ϱ�־λ
									OLED2_ShowCHinese(114,4,19);	//���Ͽ�
else							OLED2_ShowCHinese(114,4,20);	//���Ϲ�

if(Flag_Show==0)																//���ݷ��ͼ���ʾ��־λ
									OLED2_ShowCHinese(114,2,19),	//��ʾ��
									OLED2_ShowCHinese(42,6,19),		//������
									OLED2_ShowCHinese(114,6,20);	//���ڹ�
else							OLED2_ShowCHinese(114,2,20),	//��ʾ��
									OLED2_ShowCHinese(114,6,19),	//������
									OLED2_ShowCHinese(42,6,20);		//���ڿ�

}

/**************************************************************************
�������ܣ���APP��������
��ڲ�������
����  ֵ����
**************************************************************************/
void APP_Show(void)
{
		u16 Voltage=1234;//(���Ըģ�

	  static u8 flag;
	  int app_2,app_3,app_4;
		app_4=(Voltage-1110)*2/3;		if(app_4<0)app_4=0;if(app_4>100)app_4=100;   //�Ե�ѹ���ݽ��д���
		app_3=Encoder_Right*1.1; if(app_3<0)app_3=-app_3;			                   //�Ա��������ݾ������ݴ������ͼ�λ�
		app_2=Encoder_Left*1.1;  if(app_2<0)app_2=-app_2;
	  flag=!flag;
	if(PID_Send==1)//����PID����
	{
		printf("{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$",(int)(Balance_Kp*100),(int)(Balance_Kd*100),(int)(Velocity_Kp*100),(int)(Velocity_Ki*100),0,0,0,0,0);//��ӡ��APP����
		PID_Send=0;
	}
   else	if(flag==0)//
   printf("{A%d:%d:%d:%d}$",(u8)app_2,(u8)app_3,app_4,(int)Angle_Balance); //��ӡ��APP����
	 else
	 printf("{B%d:%d:%d:%d}$",(int)Angle_Balance,Distance,Encoder_Left,Encoder_Right);//��ӡ��APP���� ��ʾ����
}
/**************************************************************************
�������ܣ�����ʾ��������λ���������� �ر���ʾ��
��ڲ�������
����  ֵ����
**************************************************************************/
void DataScope(void)
{
		u16 Voltage=1600;																//(���Ըģ���ѹֵ
    Vol=(float)Voltage/100;

		DataScope_Get_Channel_Data( Angle_Balance, 1 ); //��ʾ�Ƕ� ��λ���ȣ��㣩
		DataScope_Get_Channel_Data( Distance/10, 2 );   //��ʾ�����������ľ��� ��λ��CM
		DataScope_Get_Channel_Data( Vol, 3 );           //��ʾ��ص�ѹ ��λ��V
		DataScope_Get_Channel_Data( Temperature/10, 4 );//��ʾ�����¶� ��λ���ȣ��㣩
		DataScope_Get_Channel_Data( Encoder_Left, 5 ); 	//��ʾ�������
		DataScope_Get_Channel_Data( Encoder_Right, 6 );	//��ʾ�ұ�����

		DataScope_Get_Channel_Data(0, 7 );							//����Ҫ��ʾ�������滻0
		DataScope_Get_Channel_Data( 0, 8 ); 						//����Ҫ��ʾ�������滻0
		DataScope_Get_Channel_Data(0, 9 );  						//����Ҫ��ʾ�������滻0
		DataScope_Get_Channel_Data( 0 , 10);						//����Ҫ��ʾ�������滻0

		Send_Count = DataScope_Data_Generate(6);				//ѡ�������ݸ���
		for( i = 0 ; i < Send_Count; i++)
		{
		while((USART1->SR&0X40)==0);
		USART1->DR = DataScope_OutPut_Buffer[i];
		}
}
