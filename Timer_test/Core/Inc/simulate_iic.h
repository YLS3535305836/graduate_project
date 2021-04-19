#ifndef _SIMULATE_IIC_h
#define _SIMULATE_IIC_h

#include "main.h"
#include "gpio.h"

#define SDA             HAL_GPIO_ReadPin (MPU_SCL_GPIO_Port, MPU_SDA_Pin)
#define SDA0()          HAL_GPIO_WritePin (MPU_SDA_GPIO_Port, MPU_SDA_Pin, GPIO_PIN_RESET)		//IO口输出低电平
#define SDA1()          HAL_GPIO_WritePin (MPU_SDA_GPIO_Port, MPU_SDA_Pin, GPIO_PIN_SET)		//IO口输出高电平  
#define SCL0()          HAL_GPIO_WritePin (MPU_SCL_GPIO_Port, MPU_SCL_Pin, GPIO_PIN_RESET)		//IO口输出低电平
#define SCL1()          HAL_GPIO_WritePin (MPU_SCL_GPIO_Port, MPU_SCL_Pin, GPIO_PIN_SET)		//IO口输出高电平
#define DIR_OUT()       GPIO_DIR (MPU_SDA_GPIO_Port, 7, Push50)    //输出方向
#define DIR_IN()        GPIO_DIR (MPU_SDA_GPIO_Port, 7, input_h)    //输入方向

#define SDA_OUT()       GPIO_DIR (MPU_SDA_GPIO_Port, 7, Push50)    //输出方向
#define SDA_IN()        GPIO_DIR (MPU_SDA_GPIO_Port, 7, input_h)    //输入方向


typedef enum IIC       //DAC模块
{
    SIMIIC,
    SCCB
} IIC_type;



void  simiic_start(void);
void  simiic_stop(void);
void  simiic_ack_main(uint8_t ack_main);
void  send_ch(uint8_t c);
uint8_t read_ch(uint8_t ack);
void  simiic_write_reg(uint8_t dev_add, uint8_t reg, uint8_t dat);
uint8_t simiic_read_reg(uint8_t dev_add, uint8_t reg, IIC_type type);
void  simiic_read_regs(uint8_t dev_add, uint8_t reg, uint8_t *dat_add, uint8_t num, IIC_type type);
//void  simiic_init(void);











#endif
