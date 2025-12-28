#include "btim.h"
#include "../KEY/key.h"

/* 定时器句柄定义 */
TIM_HandleTypeDef g_tim3_handle;      /* 定时器3句柄 */
TIM_HandleTypeDef g_tim5_handle;      /* 定时器5句柄 */

/**
 * @brief       基本定时器TIM3定时中断初始化函数 (适配F407)
 * @note
 *              F407系统时钟168MHz → APB1=42MHz → TIM3时钟=APB1×2=84MHz
 *              溢出时间计算: Tout = ((arr + 1) * (psc + 1)) / 84  (单位：ms)
 *              示例：arr=9999, psc=8399 → Tout=(10000×8400)/84M=1秒
 *
 * @param       arr: 自动重装值
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void btim_tim3_init(uint16_t arr, uint16_t psc)
{
    BTIM_TIM3_INT_CLK_ENABLE();                                                 /* 使能TIM3时钟 */
    
    g_tim3_handle.Instance = BTIM_TIM3_INT;                                     /* 通用定时器TIM3 */
    g_tim3_handle.Init.Prescaler = psc;                                         /* 预分频系数 */
    g_tim3_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                        /* 向上计数模式 */
    g_tim3_handle.Init.Period = arr;                                            /* 自动重装值 */
    g_tim3_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;                  /* 时钟分频因子 */
    g_tim3_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;       /* 使能自动重装 */
    
    /* 初始化定时器 */
    HAL_TIM_Base_Init(&g_tim3_handle);

    /* TIM3中断不受FreeRTOS管理：抢占优先级4 (F407分组4，0-15级) */
    HAL_NVIC_SetPriority(BTIM_TIM3_INT_IRQn, 4, 0);                            
    HAL_NVIC_EnableIRQ(BTIM_TIM3_INT_IRQn);                                     /* 开启TIM3中断 */
    
    HAL_TIM_Base_Start_IT(&g_tim3_handle);                                      /* 使能定时器+更新中断 */
}

/**
 * @brief       基本定时器TIM5定时中断初始化函数 (适配F407)
 * @note
 *              F407 TIM5时钟=84MHz，溢出时间计算同TIM3
 *
 * @param       arr: 自动重装值
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void btim_tim5_init(uint16_t arr, uint16_t psc)
{
    BTIM_TIM5_INT_CLK_ENABLE();                                                 /* 使能TIM5时钟 */
    
    g_tim5_handle.Instance = BTIM_TIM5_INT;                                     /* 通用定时器TIM5 */
    g_tim5_handle.Init.Prescaler = psc;                                         /* 预分频系数 */
    g_tim5_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                        /* 向上计数模式 */
    g_tim5_handle.Init.Period = arr;                                            /* 自动重装值 */
    g_tim5_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;                  /* 时钟分频因子 */
    g_tim5_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;       /* 使能自动重装 */
    
    /* 初始化定时器 */
    HAL_TIM_Base_Init(&g_tim5_handle); 

    /* TIM5中断受FreeRTOS管理：抢占优先级6 (需≤configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY) */
    HAL_NVIC_SetPriority(BTIM_TIM5_INT_IRQn, 6, 0);                            
    HAL_NVIC_EnableIRQ(BTIM_TIM5_INT_IRQn);                                     /* 开启TIM5中断 */
    
    HAL_TIM_Base_Start_IT(&g_tim5_handle);                                      /* 使能定时器+更新中断 */
}

/**
 * @brief       HAL_TIM_Base_MspInit回调函数 (F407必须实现)
 * @note        初始化定时器时钟、中断（HAL库强制要求）
 * @param       htim: 定时器句柄指针
 * @retval      无
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        BTIM_TIM3_INT_CLK_ENABLE();    /* TIM3时钟使能 */
        HAL_NVIC_EnableIRQ(TIM3_IRQn); /* TIM3中断使能 */
    }
    else if (htim->Instance == TIM5)
    {
        BTIM_TIM5_INT_CLK_ENABLE();    /* TIM5时钟使能 */
        HAL_NVIC_EnableIRQ(TIM5_IRQn); /* TIM5中断使能 */
    }
}

/**
 * @brief       TIM3中断服务函数
 * @retval      无
 */
void BTIM_TIM3_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_tim3_handle);  /* 调用HAL库中断处理函数 */
}

/**
 * @brief       TIM5中断服务函数
 * @retval      无
 */
void BTIM_TIM5_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_tim5_handle);  /* 调用HAL库中断处理函数 */
}

/**
 * @brief       定时器更新中断回调函数
 * @param       htim: 定时器句柄指针
 * @retval      无
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5)
    {
        for(uint8_t i = 0; i < KEY_NUM; i++){
            vKeyDetect(&xKeyCB[i]);
        }
    }
}
