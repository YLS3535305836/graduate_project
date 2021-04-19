/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define  imitate   0   //模拟输入
#define  input_h   8   //上下拉输�?
#define  Float     4   //浮空输入

#define  Push10     1   //推挽输出10
#define  Push20     2   //推挽输出20
#define  Push50     3   //推挽输出50

#define  O_dr10     5   //�?漏输�?10
#define  O_dr20     6   //�?漏输�?20
#define  O_dr50     7   //�?漏输�?50

#define  M_Push10     9    //推挽输出10   //复用
#define  M_Push20     10   //推挽输出20
#define  M_Push50     11   //推挽输出50

#define  MO_dr10     13   //�?漏输�?10      //复用
#define  MO_dr20     14   //�?漏输�?20
#define  MO_dr50     15   //�?漏输�?50
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
void GPIO_DIR(GPIO_TypeDef* GPIOx,uint8_t pin,uint8_t mode);
void GPIO_SET(GPIO_TypeDef* GPIOx,uint8_t pin,uint8_t level);
uint8_t GPIO_GET(GPIO_TypeDef* GPIOx,uint8_t pin);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
