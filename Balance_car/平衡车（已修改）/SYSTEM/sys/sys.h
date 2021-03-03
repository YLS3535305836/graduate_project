#ifndef __SYS_H
#define __SYS_H
#include <stm32f10x.h>

//JTAG模式设置定义
#define JTAG_SWD_DISABLE   0X02
#define SWD_ENABLE         0X01
#define JTAG_SWD_ENABLE    0X00
void JTAG_Set(u8 mode);



#define SYSTEM_SUPPORT_UCOS		0		//定义系统文件夹是否支持UCOS  0,不支持

//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 n 可取 0~15

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 n 可取 0~15

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 n 可取 13~15

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 n 可取 0~1


extern int Encoder_Left,Encoder_Right;
extern int Moto1,Moto2;
extern u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right,Flag_sudu;
extern u8 Flag_Stop,Flag_Show;
extern float Angle_Balance,Gyro_Balance,Gyro_Turn;
extern float Show_Data_Mb;
extern int Temperature;
extern u32 Distance;
extern u8 Bi_zhang,delay_50,delay_flag,PID_Send,Flash_Send;
extern float Acceleration_Z;
extern float Balance_Kp,Balance_Kd,Velocity_Kp,Velocity_Ki;
extern u16 PID_Parameter[10],Flash_Parameter[10];

#include "delay.h"
#include "led.h"
#include "key.h"
#include "oled.h"
#include "usart.h"
#include "usart3.h"
#include "timer.h"
#include "motor.h"
#include "encoder.h"
#include "iic.h"
#include "mpu6050.h"
#include "show.h"
#include "exti.h"
#include "DataScope_DP.h"
#include "stmflash.h"
#include "filter.h"
#include "beep.h"
#include "infrared.h"

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "dmpmap.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#endif











