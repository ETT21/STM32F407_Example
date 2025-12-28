#ifndef __MALLOC_H
#define __MALLOC_H

#include "../../core/system/system_hal.h"

#ifndef NULL
#define NULL 0
#endif

/* 内存池定义 (适配STM32F407ZGT6 + 1MB外扩SRAM) */
#define     SRAMIN                  0                               /* 内部SRAM池 (192KB) */
#define     SRAMCCM                 1                               /* CCM内存池 (64KB) */
#define     SRAMEX                  2                               /* 外扩SRAM池 (1MB) */
#define     SRAMBANK                3                               /* 内存池总数 */

/* ==================== 内存池参数 (严格匹配硬件) ==================== */
/* mem1: 内部SRAM (192KB总容量，预留12KB系统空间) */
#define     MEM1_BLOCK_SIZE         32                              /* 32字节/块 */
#define     MEM1_MAX_SIZE           180*1024                         /* 管理180KB */
#define     MEM1_ALLOC_TABLE_SIZE   MEM1_MAX_SIZE / MEM1_BLOCK_SIZE  /* 180*1024/32=5760项 */

/* mem2: CCM内存 (64KB总容量，预留4KB) */
#define     MEM2_BLOCK_SIZE         32                              /* 32字节/块 */
#define     MEM2_MAX_SIZE           60*1024                          /* 管理60KB */
#define     MEM2_ALLOC_TABLE_SIZE   MEM2_MAX_SIZE / MEM2_BLOCK_SIZE  /* 60*1024/32=1920项 */

/* mem3: 外扩SRAM (1MB总容量，预留232KB给其他数据) */
#define     MEM3_BLOCK_SIZE         32                              /* 32字节/块 */
#define     MEM3_MAX_SIZE           768*1024                         /* 管理768KB内存池 */
#define     MEM3_ALLOC_TABLE_SIZE   MEM3_MAX_SIZE / MEM3_BLOCK_SIZE  /* 768*1024/32=24576项 */
/* 状态表占用：24576项 × 2字节 = 48KB，总占用768+48=816KB ≤ 1024KB */

/* 内存管理控制器 */
struct _m_mallco_dev
{
    void (*init)(uint8_t);              /* 初始化函数 */
    uint16_t (*perused)(uint8_t);       /* 使用率函数 (扩大10倍) */
    uint8_t *membase[SRAMBANK];         /* 内存池基地址 */
    uint16_t *memmap[SRAMBANK];         /* 状态表基地址 */
    uint8_t memrdy[SRAMBANK];           /* 就绪标志 */
};

extern struct _m_mallco_dev mallco_dev;

/* 底层函数 */
void mymemset(void *s, uint8_t c, uint32_t count);
void mymemcpy(void *des, void *src, uint32_t n);
void my_mem_init(uint8_t memx);
uint32_t my_mem_malloc(uint8_t memx, uint32_t size);
uint8_t my_mem_free(uint8_t memx, uint32_t offset);
uint16_t my_mem_perused(uint8_t memx);

/* 用户接口 */
void myfree(uint8_t memx, void *ptr);
void *mymalloc(uint8_t memx, uint32_t size);
void *myrealloc(uint8_t memx, void *ptr, uint32_t size);

#endif
