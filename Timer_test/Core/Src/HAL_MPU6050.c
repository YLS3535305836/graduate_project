#include "HAL_MPU6050.h"
extern I2C_HandleTypeDef hi2c1;

#define Gyro_Fsr 3		//陀螺仪传感器Gyro_fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
#define Accel_Fsr 0		//加速度传感器Accel_fsr:0,±2g;1,±4g;2,±8g;3,±16g
//float fRad2Deg = 57.295779513f; //将弧度转为角度的乘数

//初始化MPU6050
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Init(void)
{ 
	uint8_t res;
	//IIC_Init();//初始化IIC总线
	MPU_Write_Byte(MPU_ADDR,MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050
    HAL_Delay(200-1);
	MPU_Write_Byte(MPU_ADDR,MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050 
	MPU_Set_Gyro_Fsr(Gyro_Fsr);					//陀螺仪传感器,±250dps,参数见宏定义
	MPU_Set_Accel_Fsr(Accel_Fsr);					//加速度传感器,±2g,参数见宏定义
	MPU_Set_Rate(50);						//设置采样率50Hz
	MPU_Write_Byte(MPU_ADDR,MPU_INT_EN_REG,0X00);	//关闭所有中断
	MPU_Write_Byte(MPU_ADDR,MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	MPU_Write_Byte(MPU_ADDR,MPU_FIFO_EN_REG,0X00);	//关闭FIFO
	MPU_Write_Byte(MPU_ADDR,MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	MPU_Read_Byte(MPU_ADDR,MPU_DEVICE_ID_REG,&res);
	if(res==0x68)//器件ID正确
	{
		MPU_Write_Byte(MPU_ADDR,MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
		MPU_Write_Byte(MPU_ADDR,MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
		MPU_Set_Rate(50);						//设置采样率为50Hz
 	}
	else 
		return 1;
	return 0;
}
//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_ADDR,MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_ADDR,MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_ADDR,MPU_CFG_REG,data);//设置数字低通滤波器  
}
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_ADDR,MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}



float MPU_Get_Temperature(void)
{
    uint8_t buf[2]; 
    int16_t raw;
		float temp;
		MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
		raw=((uint16_t)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp;
}


//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码

uint8_t MPU_Get_Gyroscope(int16_t *gx,int16_t *gy,int16_t *gz)
{
  uint8_t buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((uint16_t)buf[0]<<8)|buf[1];  
		*gy=((uint16_t)buf[2]<<8)|buf[3];  
		*gz=((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;
}
//对陀螺仪数据进行处理
void MPU_Progress_Gyro_Data(float* Gyro_X,float* Gyro_Y,float* Gyro_Z)
{
	float Power;
	GyrostructRaw Gyrol = {0,0,0};
	MPU_Get_Gyroscope(&Gyrol.Gyro_X,&Gyrol.Gyro_Y,&Gyrol.Gyro_Z);
	//Gyro_fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
	switch (Gyro_Fsr)
	{
		case 0:
			Power = 250;
			break;
		case 1:
			Power = 500;
			break;
		case 2:
			Power = 1000;
			break;
		case 3:
			Power = 2000;
			break;
	}
	*Gyro_X = (Power * Gyrol.Gyro_X)/32768;
	*Gyro_Y = (Power * Gyrol.Gyro_Y)/32768;
	*Gyro_Z = (Power * Gyrol.Gyro_Z)/32768;
}

//对加速度数据处理
void MPU_Progress_Accel_Data(float* Acc_X,float* Acc_Y,float* Acc_Z)
{
	float Power;
	AccelstructRaw Accl = {0,0,0};
	MPU_Get_Accelerometer(&Accl.Accel_X,&Accl.Accel_Y,&Accl.Accel_Z);
	//Accel_fsr:0,±2g;1,±4g;2,±8g;3,±16g
	switch (Accel_Fsr)
	{
		case 0:
			Power = 2;
			break;
		case 1:
			Power = 4;
			break;
		case 2:
			Power = 8;
			break;
		case 3:
			Power = 16;
			break;
	}
	*Acc_X = (Power * Accl.Accel_X)/32768;
	*Acc_Y = (Power * Accl.Accel_Y)/32768;
	*Acc_Z = (Power * Accl.Accel_Z)/32768;
}

//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Accelerometer(int16_t *ax,int16_t *ay,int16_t *az)
{
  uint8_t buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax = ((uint16_t)buf[0]<<8)|buf[1];  
		*ay = ((uint16_t)buf[2]<<8)|buf[3];  
		*az = ((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;
}



//IIC连续写
//addr:器件地址 
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	return HAL_I2C_Mem_Write(&hi2c1, addr, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 10*len);
} 
//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
 	return HAL_I2C_Mem_Read(&hi2c1, addr, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 10*len);
}
//IIC写一个字节 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Write_Byte(uint8_t addr, uint8_t reg,uint8_t data) 				 
{ 
  uint8_t sendbuf[2];
	sendbuf[0] = reg;
	sendbuf[1] = data;
	return HAL_I2C_Master_Transmit(&hi2c1, addr, sendbuf, 2, 10);  
}
//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
uint8_t MPU_Read_Byte(uint8_t addr, uint8_t reg, uint8_t* data)
{
  return HAL_I2C_Mem_Read(&hi2c1, addr, reg, I2C_MEMADD_SIZE_8BIT, data, 1, 10);		
}

