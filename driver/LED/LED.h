/**
  ******************************************************************************
  * @file       LED.h
  * @author     czzzlq
  * @version    V0.1
  * @date       15-7-2025
  * @brief      LED驱动头文件
  * @attention  仅供学习使用
  * @version    
  *             -V0.1
  ******************************************************************************
**/

/* Define to prevent recursive inclusion */
#ifndef __LED_H
#define __LED_H

/* Includes */
#include "stm32f4xx.h"

/* Exported macro */

//引脚定义

//LED0
#define LED0_GPIO_PORT          GPIOF
#define LED0_GPIO_PIN           GPIO_PIN_9
#define LED0_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)

//LED1
#define LED1_GPIO_PORT          GPIOF
#define LED1_GPIO_PIN           GPIO_PIN_10
#define LED1_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)

//端口定义
#define LED0(x)                 do{ x? \
                                    HAL_GPIO_WritePin(LED0_GPIO_PORT,LED0_GPIO_PIN,GPIO_PIN_SET) : \
                                    HAL_GPIO_WritePin(LED0_GPIO_PORT,LED0_GPIO_PIN,GPIO_PIN_RESET); \
                                }while(0)

#define LED1(x)                 do{ x? \
                                    HAL_GPIO_WritePin(LED1_GPIO_PORT,LED1_GPIO_PIN,GPIO_PIN_SET) : \
                                    HAL_GPIO_WritePin(LED1_GPIO_PORT,LED1_GPIO_PIN,GPIO_PIN_RESET); \
                                  }while(0)

//LED取反
#define LED0_TOGGLE()           do{ HAL_GPIO_TogglePin(LED0_GPIO_PORT,LED0_GPIO_PIN); }while(0)
#define LED1_TOGGLE()           do{ HAL_GPIO_TogglePin(LED1_GPIO_PORT,LED1_GPIO_PIN); }while(0)

/* Exported constants */

/* Exported variables */

/* Exported functions */
void LED_Init();

#endif /* LED_H */
/******************************************************************************/
