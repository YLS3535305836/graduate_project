#include "MPU6050.h"

int16_t mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;
int16_t mpu_acc_x,mpu_acc_y,mpu_acc_z;





//-------------------------------------------------------------------------------------------------------------------
//  @brief      MPU6050自检函数
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				调用该函数前，请先调用模拟IIC的初始化
//-------------------------------------------------------------------------------------------------------------------
void mpu6050_self1_check(void)
{
    simiic_write_reg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	//解除休眠状态
    simiic_write_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);   //125HZ采样率
    while(0x07 != simiic_read_reg(MPU6050_DEV_ADDR, SMPLRT_DIV,SIMIIC))
    {
        //卡在这里原因有以下几点
        //1 MPU6050坏了，如果是新的这样的概率极低
        //2 接线错误或者没有接好
        //3 可能你需要外接上拉电阻，上拉到3.3V
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化MPU6050
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				调用该函数前，请先调用模拟IIC的初始化
//-------------------------------------------------------------------------------------------------------------------
void mpu6050_init(void)
{
    HAL_Delay(100);                                   //上电延时

    mpu6050_self1_check();
    simiic_write_reg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	//解除休眠状态
    simiic_write_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);   //125HZ采样率
    simiic_write_reg(MPU6050_DEV_ADDR, CONFIG, 0x04);       //
    simiic_write_reg(MPU6050_DEV_ADDR, GYRO_CONFIG, 0x18);  //2000
    simiic_write_reg(MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x10); //8g
	simiic_write_reg(MPU6050_DEV_ADDR, User_Control, 0x00);
    simiic_write_reg(MPU6050_DEV_ADDR, INT_PIN_CFG, 0x02);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取MPU6050加速度计数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void get_accdata(void)
{
    uint8_t dat[6];

    simiic_read_regs(MPU6050_DEV_ADDR, ACCEL_XOUT_H, dat, 6, SIMIIC);  
    mpu_acc_x = (int16_t)(((uint16_t)dat[0]<<8 | dat[1]));
    mpu_acc_y = (int16_t)(((uint16_t)dat[2]<<8 | dat[3]));
    mpu_acc_z = (int16_t)(((uint16_t)dat[4]<<8 | dat[5]));
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取MPU6050陀螺仪数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void get_gyro(void)
{
    uint8_t dat[6];

    simiic_read_regs(MPU6050_DEV_ADDR, GYRO_XOUT_H, dat, 6, SIMIIC);  
    mpu_gyro_x = (int16_t)(((uint16_t)dat[0]<<8 | dat[1]));
    mpu_gyro_y = (int16_t)(((uint16_t)dat[2]<<8 | dat[3]));
    mpu_gyro_z = (int16_t)(((uint16_t)dat[4]<<8 | dat[5]));
}

//-------------------------------------------------------------------------------------------------------------------
//  以上函数是使用软件IIC通信，相比较硬件IIC，软件IIC引脚更加灵活，可以使用任意普通IO
//-------------------------------------------------------------------------------------------------------------------





// //-------------------------------------------------------------------------------------------------------------------
// //  以下函数是使用硬件IIC通信，相比较软件IIC，硬件IIC速度可以做到更快。
// //-------------------------------------------------------------------------------------------------------------------

// #define IIC_NUM         IIC_1
// #define IIC_SDA_PIN     IIC1_SDA_A13
// #define IIC_SCL_PIN     IIC1_SCL_A14
// //-------------------------------------------------------------------------------------------------------------------
// //  @brief      MPU6050自检函数
// //  @param      NULL
// //  @return     void					
// //  @since      v1.0
// //  Sample usage:				
// //-------------------------------------------------------------------------------------------------------------------
// void mpu6050_self2_check(void)
// {
//     iic_write_reg(IIC_NUM, MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	//解除休眠状态
//     iic_write_reg(IIC_NUM, MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);   //125HZ采样率
//     while(0x07 != iic_read_reg(IIC_NUM, MPU6050_DEV_ADDR, SMPLRT_DIV))
//     {
//         //卡在这里原因有以下几点
//         //1 MPU6050坏了，如果是新的这样的概率极低
//         //2 接线错误或者没有接好
//         //3 可能你需要外接上拉电阻，上拉到3.3V
//     }
// }

// //-------------------------------------------------------------------------------------------------------------------
// //  @brief      初始化MPU6050
// //  @param      NULL
// //  @return     void					
// //  @since      v1.0
// //  Sample usage:				
// //-------------------------------------------------------------------------------------------------------------------
// void mpu6050_init_hardware(void)
// {
//     systick_delay_ms(100);                                      //上电延时
//     iic_init(IIC_NUM, IIC_SDA_PIN, IIC_SCL_PIN,400*1000);       //硬件IIC初始化     波特率400K
    
//     mpu6050_self2_check();
//     iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	//解除休眠状态
//     iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);    //125HZ采样率
//     iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, CONFIG, 0x04);        //
//     iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, GYRO_CONFIG, 0x18);   //2000
//     iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x10);  //8g
// 	iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, User_Control, 0x00);
//     iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, INT_PIN_CFG, 0x02);
// }

// //-------------------------------------------------------------------------------------------------------------------
// //  @brief      获取MPU6050加速度计数据
// //  @param      NULL
// //  @return     void
// //  @since      v1.0
// //  Sample usage:				执行该函数后，直接查看对应的变量即可
// //-------------------------------------------------------------------------------------------------------------------
// void get_accdata_hardware(void)
// {
//     uint8_t dat[6];
    
//     iic_read_reg_bytes(IIC_NUM,MPU6050_DEV_ADDR, ACCEL_XOUT_H, dat, 6);
//     mpu_acc_x = (int16_t)(((uint16_t)dat[0]<<8 | dat[1]));
//     mpu_acc_y = (int16_t)(((uint16_t)dat[2]<<8 | dat[3]));
//     mpu_acc_z = (int16_t)(((uint16_t)dat[4]<<8 | dat[5]));
// }

// //-------------------------------------------------------------------------------------------------------------------
// //  @brief      获取MPU6050陀螺仪数据
// //  @param      NULL
// //  @return     void
// //  @since      v1.0
// //  Sample usage:				执行该函数后，直接查看对应的变量即可
// //-------------------------------------------------------------------------------------------------------------------
// void get_gyro_hardware(void)
// {
//     uint8_t dat[6];

//     iic_read_reg_bytes(IIC_NUM,MPU6050_DEV_ADDR, GYRO_XOUT_H, dat, 6);  
//     mpu_gyro_x = (int16_t)(((uint16_t)dat[0]<<8 | dat[1]));
//     mpu_gyro_y = (int16_t)(((uint16_t)dat[2]<<8 | dat[3]));
//     mpu_gyro_z = (int16_t)(((uint16_t)dat[4]<<8 | dat[5]));
// }

