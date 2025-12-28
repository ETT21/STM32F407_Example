#include "freertos_demo.h"

const uint16_t lcd_discolor[11] = {                                                   /* lcd屏幕刷新显示的颜色 */
    WHITE,BLACK,BLUE,RED,
    MAGENTA,GREEN,CYAN,YELLOW,
    BROWN,BRRED,GRAY
};

//START_TASK
TaskHandle_t                StartTask_Handler;
//TASK1
TaskHandle_t                TASK1Task_Handler;

/**
 * @brief   FreeRTOS例程入口函数
 * @param   无
 * @retval  无
 */
void freertos_demo(void)
{
    lcd_show_string(10,10,220,32,32,"STM32",BLACK);
    lcd_show_string(10,80,520,32,32,"FreeRTOS Porting",BLACK);
    lcd_show_string(10,150,220,32,32,"Hello World",BLACK);

    //动态创建任务
    xTaskCreate((TaskFunction_t)Start_Task,                                     /* 任务函数 */
                (const char *  )"Start_task",                                   /* 任务名称 */
                (uint16_t      )START_STK_SIZE,                                 /* 任务堆栈大小 */
                (void*         )NULL,                                           /* 传入给任务函数的参数 */
                (UBaseType_t   )START_TASK_PRIO,                                /* 任务优先级 */
                (TaskHandle_t* )&StartTask_Handler);                            /* 任务句柄 */
    vTaskStartScheduler();
}

/**
 * @brief   Start_Task
 * @param   pvParameters :传入参数
 * @retval  无
 */
void Start_Task(void *pvParameters)
{

    taskENTER_CRITICAL();                                                       /* 非中断进入临界区 */

    //初始化TIM3和TIM5
    btim_tim3_init(10000-1,7200-1);
    btim_tim5_init(10000-1,7200-1);

    //动态创建任务1
    xTaskCreate((TaskFunction_t )Task1,
                (const char *   )"Task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t  *)&TASK1Task_Handler);
    
    vTaskDelete(StartTask_Handler);                                             /* 删除开始任务 */
    taskEXIT_CRITICAL();                                                        /*　退出临界区 */
}

/**
 * @brief   Task1
 * @param   pvParameters :传入参数
 * @retval  无
 */
void Task1(void *pvParameters)
{
    uint32_t task1_num = 0;

    while(1)
    {
        LED1(0);
        if(++task1_num == 5)
        {
            LED0(0);
            printf("FreeRTOS关闭中断\r\n");
            portDISABLE_INTERRUPTS();                                           /* FreeRTOS关闭中断 */
            systickDelay_ms(500);
            printf("FreeRTOS打开中断\r\n");
            portENABLE_INTERRUPTS();                                            /* FreeRTOS打开中断 */
            LED0(1);
        }
        vTaskDelay(1000);
    }
}
