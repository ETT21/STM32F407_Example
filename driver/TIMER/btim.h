#ifndef __BTIM_H
#define __BTIM_H

#include "stm32f4xx_hal.h"  // F407 HAL库核心头文件
#include <stdio.h>          // 用于printf输出

/* 基本定时器 TIM3 宏定义 (适配F407) */
#define BTIM_TIM3_INT               TIM3
#define BTIM_TIM3_INT_IRQn          TIM3_IRQn
#define BTIM_TIM3_INT_IRQHandler    TIM3_IRQHandler
#define BTIM_TIM3_INT_CLK_ENABLE()  do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM3时钟使能 */

/* 基本定时器 TIM5 宏定义 (适配F407) */
#define BTIM_TIM5_INT               TIM5
#define BTIM_TIM5_INT_IRQn          TIM5_IRQn
#define BTIM_TIM5_INT_IRQHandler    TIM5_IRQHandler
#define BTIM_TIM5_INT_CLK_ENABLE()  do{ __HAL_RCC_TIM5_CLK_ENABLE(); }while(0)  /* TIM5时钟使能 */

/* 外部声明定时器句柄 (供回调函数访问) */
extern TIM_HandleTypeDef g_tim3_handle;
extern TIM_HandleTypeDef g_tim5_handle;

/* 函数声明 */
void btim_tim3_init(uint16_t arr, uint16_t psc);  // TIM3定时中断初始化
void btim_tim5_init(uint16_t arr, uint16_t psc);  // TIM5定时中断初始化

#endif /* __BTIM_H */
