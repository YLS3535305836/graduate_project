/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Direcion_Pin_GPIO_Port, Direcion_Pin_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, IIC_SDA_Pin|IIC_SCL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = Direcion_Pin_Pin|IIC_SDA_Pin|IIC_SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
void GPIO_DIR(GPIO_TypeDef* GPIOx,uint8_t pin,uint8_t mode)
{
	  switch((int)GPIOx)
		{
				case (int)GPIOA:
					RCC->APB2ENR|=1<<2;
					break;
				case (int)GPIOB:
					RCC->APB2ENR|=1<<3;
					break;
				case (int)GPIOC:
					RCC->APB2ENR|=1<<4;
					break;
				case (int)GPIOD:
					RCC->APB2ENR|=1<<5;
					break;
				// case (int)GPIOE:
				// 	RCC->APB2ENR|=1<<6;
				// 	break;
				// case (int)GPIOF:
				// 	RCC->APB2ENR|=1<<7;
				// 	break;
                default:
                    break;
		}
		if(pin<8)
		{
			GPIOx->CRL&=~(0x0000000f<<(pin*4));
		    GPIOx->CRL|=(0x00000000|mode)<<(pin*4);
		}
		else 
		{
			GPIOx->CRH&=~(0X0000000F<<( (pin-8)*4 ));
			GPIOx->CRH|=(0x00000000|mode)<<( (pin-8)*4 );
		}
}

//设置io的电平高�?
//  ex:  GPIO_SET(GPIOA,5,1)    设置GPIOA�?5口为高电�?
void GPIO_SET(GPIO_TypeDef* GPIOx,uint8_t pin,uint8_t level)
{
		if(level==1)
		{
				GPIOx->ODR=1<<pin;
		}
		else 
		{
				GPIOx->ODR=~(1<<pin);
		}
}

//获得io的电平高�?
//   ex:  temp=GPIO_SET(GPIOA,5)   读取GPIOA�?5口的电平  
//   返回 1 高电�? 0 低电�?
uint8_t GPIO_GET(GPIO_TypeDef* GPIOx,uint8_t pin)
{
		if(GPIOx->ODR&1<<pin)
			return 1;
		else
			return 0;
}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
