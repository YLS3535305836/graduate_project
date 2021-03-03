#include "mpu6050.h"

#define DEFAULT_MPU_HZ  (200)

uint8_t buffer[14];
short gyro[3], accel[3], sensors;

static signed char gyro_orientation[9] = {-1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1};

static  unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}


static  unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar;
    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}

static void run_self_test(void)
{
    int result;
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
    if (result == 0x7) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
		printf("setting bias succesfully ......\r\n");
    }
}

/**************************************************************************
�������ܣ�MPU6050����DMP�ĳ�ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void DMP_Init(void)
{
   u8 temp[1]={0};
   i2cRead(0x68,0x75,1,temp);
	 printf("mpu_set_sensor complete ......\r\n");
	if(temp[0]!=0x68)NVIC_SystemReset();
	if(!mpu_init())
  {
	  if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))
	  	 printf("mpu_set_sensor complete ......\r\n");
	  if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))
	  	 printf("mpu_configure_fifo complete ......\r\n");
	  if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))
	  	 printf("mpu_set_sample_rate complete ......\r\n");
	  if(!dmp_load_motion_driver_firmware())
	  	printf("dmp_load_motion_driver_firmware complete ......\r\n");
	  if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)))
	  	 printf("dmp_set_orientation complete ......\r\n");
	  if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
	        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
	        DMP_FEATURE_GYRO_CAL))
	  	 printf("dmp_enable_feature complete ......\r\n");
	  if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ))
	  	 printf("dmp_set_fifo_rate complete ......\r\n");
	  run_self_test();
	  if(!mpu_set_dmp_state(1))
	  	 printf("mpu_set_dmp_state complete ......\r\n");
  }
}
/******************************************************************************
��������:	����  MPU6050 ��ʱ��Դ
��ڲ�����
����  ֵ����
*******************************************************************************/
void MPU6050_setClockSource(uint8_t source)
{
    IICwriteBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}

void MPU6050_setFullScaleGyroRange(uint8_t range)
{
    IICwriteBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}
/******************************************************************************
��������:	����  MPU6050 ���ٶȼƵ��������
��ڲ�����
����  ֵ����
*******************************************************************************/
void MPU6050_setFullScaleAccelRange(uint8_t range)
{
    IICwriteBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}
/******************************************************************************
��������:	����  MPU6050 �Ƿ����˯��ģʽ
��ڲ�����enabled =1 ˯��		enabled =0 ����
����  ֵ����
*******************************************************************************/
void MPU6050_setSleepEnabled(uint8_t enabled)
{
    IICwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}
/******************************************************************************
��������:	��ȡ  MPU6050 WHO_AM_I ��ʶ	 ������ 0x68
��ڲ�����
����  ֵ��
*******************************************************************************/
uint8_t MPU6050_getDeviceID(void)
{
    IICreadBytes(devAddr, MPU6050_RA_WHO_AM_I, 1, buffer);
    return buffer[0];
}
/******************************************************************************
��������:	���MPU6050 �Ƿ��Ѿ�����
��ڲ�����
����  ֵ��
*******************************************************************************/
uint8_t MPU6050_testConnection(void)
{
   if(MPU6050_getDeviceID() == 0x68)  //0b01101000;
   return 1;
   else return 0;
}
/******************************************************************************
��������:	���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
��ڲ�����
����  ֵ����
*******************************************************************************/
void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
{
    IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}
/******************************************************************************
��������:	���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
��ڲ�����
����  ֵ����
*******************************************************************************/
void MPU6050_setI2CBypassEnabled(uint8_t enabled)
{
    IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}
/******************************************************************************
��������:	��ʼ�� 	MPU6050 �Խ������״̬
��ڲ�����
����  ֵ����
*******************************************************************************/
void MPU6050_initialize(void)
{
		MPU6050_setClockSource(MPU6050_CLOCK_PLL_YGYRO); 		//����ʱ��
		MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);//������������� +-1000��ÿ��
		MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_2);	//���ٶȶ�������� +-2G
		MPU6050_setSleepEnabled(0); 												//���빤��״̬
		MPU6050_setI2CMasterModeEnabled(0);	 								//����MPU6050 ����AUXI2C
		MPU6050_setI2CBypassEnabled(0);	 										//����������I2C��	MPU6050��AUXI2C	ֱͨ������������ֱ�ӷ���HMC5883L
}
/**************************************************************************
�������ܣ���ȡMPU6050�����¶ȴ���������
��ڲ�������
����  ֵ�������¶�
**************************************************************************/
int Read_Temperature(void)
{
	  float Temp;
	  Temp=(I2C_ReadOneByte(devAddr,MPU6050_RA_TEMP_OUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_TEMP_OUT_L);
		if(Temp>32768) Temp-=65536;
		Temp=(36.53+Temp/340)*10;
	  return (int)Temp;
}
