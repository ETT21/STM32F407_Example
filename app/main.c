#include "main.h"

int main(void)
{
    systemCoreInit();                                      
    usart1Init(115200);                                                         /* 串口初始化为115200 */
    systickDelayInit(168);                                                      /* 延时函数初始化 */
    lcd_init(); 
    LED_Init(); 
    keyInit();
    btim_tim5_init(10000-1,84-1);
    sram_init();                                                                /* SRAM初始化 */
    my_mem_init(SRAMIN);                                                        /* 初始化内部SRAM内存池 */
    my_mem_init(SRAMCCM);                                                       /* 初始化内部CCM内存池 */                                                      /* 初始化外部SRAM内存池 */               
    my_mem_init(SRAMEX);                                                        /* 初始化外部SRAM内存池 */
    
    freertos_demo();
}
