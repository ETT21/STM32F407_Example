/**
  ******************************************************************************
  * @file       freertos_demo.h
  * @author     czzzlq
  * @version    V0.1
  * @date       15-7-2025
  * @brief      FreeRTOS demo头文件
  * @attention  仅供学习使用
  * @version    
  *             -V0.1
  ******************************************************************************
**/

/* Define to prevent recursive inclusion */

#ifndef __FREERTOS_DEMO_H
#define __FREERTOS_DEMO_H

/* Includes */

#include "../core/CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h"
#include "../../../driver/LED/LED.h"
#include "../../../driver/LCD/lcd.h"
#include "../../../driver/TIMER/btim.h"
#include "../../driver/usart/usart.h"
#include "../../driver/delay/delay.h"
#include "../../driver/KEY/key.h"
//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"

/* Exported macro */

//START_TASK
#define START_TASK_PRIO      1                                                  /* 任务优先级 */
#define START_STK_SIZE      128                                                 /* 任务堆栈大小 */

//TASK1
#define TASK1_PRIO           2                                                  /* 任务优先级 */    
#define TASK1_STK_SIZE      128                                                 /* 任务堆栈大小 */

/* Exported constants */

/* Exported variables */

/* Exported functions */

void freertos_demo(void);
void Start_Task(void *pvParameters);
void Task1(void *pvParameters);

#endif /* FREERTOS_DEMO_H */
/******************************************************************************/
