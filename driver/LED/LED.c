/**
  ******************************************************************************
  * @file       LED.c
  * @author     czzzlq
  * @version    V0.1
  * @date       15-7-2025
  * @brief      LED驱动
  * @attention  仅供学习使用
  * @version    
  *             -V0.1
  ******************************************************************************
**/
#include "./LED.h"

/**
 * @brief   LED初始化
 * @param   无
 * @retval  无
 */
void LED_Init(void)
{
    //使能时钟
    LED0_GPIO_CLK_ENABLE();
    LED1_GPIO_CLK_ENABLE();

    GPIO_InitTypeDef gpioInitStruct;
    gpioInitStruct.Pin      = LED0_GPIO_PIN;
    gpioInitStruct.Mode     = GPIO_MODE_OUTPUT_PP;
    gpioInitStruct.Pull     = GPIO_PULLUP;                                           /* 上拉模式，输出模式下可控制引脚电平初始电平状态 */
    gpioInitStruct.Speed    = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LED0_GPIO_PORT,&gpioInitStruct);

    gpioInitStruct.Pin      = LED1_GPIO_PIN;
    HAL_GPIO_Init(LED1_GPIO_PORT,&gpioInitStruct);

    //LED初始状态为关闭状态
    LED0(1);
    LED1(1);
}
