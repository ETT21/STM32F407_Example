/**
 ****************************************************************************************************
 * @file        malloc.c
 * @brief       内存管理驱动 (适配STM32F407ZGT6 + 1MB外扩SRAM)
 ****************************************************************************************************
 */

#include "./malloc.h"

/* ==================== 内存池定义 (32字节对齐，地址严格匹配外扩SRAM) ==================== */
/* 内部SRAM池: 0x20000000，180KB */
__align(32) uint8_t mem1base[MEM1_MAX_SIZE];

/* CCM内存池: 0x10000000，60KB (仅CPU访问) */
__align(32) uint8_t mem2base[MEM2_MAX_SIZE] __attribute__((at(0x10000000)));

/* 外扩SRAM池: 0x68000000，768KB (1MB外扩SRAM起始地址) */
__align(32) uint8_t mem3base[MEM3_MAX_SIZE] __attribute__((at(0x68000000)));

/* ==================== 状态表定义 (紧跟内存池，地址连续) ==================== */
/* 内部SRAM状态表 */
__align(32) uint16_t mem1mapbase[MEM1_ALLOC_TABLE_SIZE];

/* CCM状态表: 0x10000000 + 60KB = 0x1000EA00 */
__align(32) uint16_t mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0x10000000 + MEM2_MAX_SIZE)));

/* 外扩SRAM状态表: 0x68000000 + 768KB = 0x680C0000 */
__align(32) uint16_t mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0x68000000 + MEM3_MAX_SIZE)));

/* 内存管理参数表 */
const uint32_t memtblsize[SRAMBANK] = {MEM1_ALLOC_TABLE_SIZE, MEM2_ALLOC_TABLE_SIZE, MEM3_ALLOC_TABLE_SIZE};
const uint32_t memblksize[SRAMBANK] = {MEM1_BLOCK_SIZE, MEM2_BLOCK_SIZE, MEM3_BLOCK_SIZE};
const uint32_t memsize[SRAMBANK]    = {MEM1_MAX_SIZE, MEM2_MAX_SIZE, MEM3_MAX_SIZE};

/* 控制器实例 */
struct _m_mallco_dev mallco_dev = {
    my_mem_init,
    my_mem_perused,
    mem1base, mem2base, mem3base,
    mem1mapbase, mem2mapbase, mem3mapbase,
    0, 0, 0
};

/**
 * @brief   内存拷贝
 */
void mymemcpy(void *des, void *src, uint32_t n)
{
    uint8_t *xdes = (uint8_t *)des;
    uint8_t *xsrc = (uint8_t *)src;
    while (n--) *xdes++ = *xsrc++;
}

/**
 * @brief   内存填充
 */
void mymemset(void *s, uint8_t c, uint32_t count)
{
    uint8_t *xs = (uint8_t *)s;
    while (count--) *xs++ = c;
}

/**
 * @brief   内存池初始化
 */
void my_mem_init(uint8_t memx)
{
    if (memx >= SRAMBANK) return;
    mymemset(mallco_dev.memmap[memx], 0, memtblsize[memx] * sizeof(uint16_t));
    mymemset(mallco_dev.membase[memx], 0, memsize[memx]);
    mallco_dev.memrdy[memx] = 1;
}

/**
 * @brief   获取内存使用率
 */
uint16_t my_mem_perused(uint8_t memx)
{
    uint32_t used = 0, i;
    if (memx >= SRAMBANK || !mallco_dev.memrdy[memx]) return 0;
    for (i = 0; i < memtblsize[memx]; i++) {
        if (mallco_dev.memmap[memx][i]) used++;
    }
    return (used * 1000) / memtblsize[memx];
}

/**
 * @brief   内存分配 (内部)
 */
uint32_t my_mem_malloc(uint8_t memx, uint32_t size)
{
    signed long offset = 0;
    uint32_t nmemb, cmemb = 0, i;
    if (memx >= SRAMBANK || !mallco_dev.memrdy[memx] || size == 0) return 0XFFFFFFFF;
    nmemb = size / memblksize[memx];
    if (size % memblksize[memx]) nmemb++;
    for (offset = memtblsize[memx] - 1; offset >= 0; offset--) {
        if (!mallco_dev.memmap[memx][offset]) cmemb++;
        else cmemb = 0;
        if (cmemb == nmemb) {
            for (i = 0; i < nmemb; i++) {
                mallco_dev.memmap[memx][offset + i] = nmemb;
            }
            return offset * memblksize[memx];
        }
    }
    return 0XFFFFFFFF;
}

/**
 * @brief   内存释放 (内部)
 */
uint8_t my_mem_free(uint8_t memx, uint32_t offset)
{
    int i, index, nmemb;
    if (memx >= SRAMBANK || !mallco_dev.memrdy[memx]) return 1;
    if (offset >= memsize[memx]) return 2;
    index = offset / memblksize[memx];
    nmemb = mallco_dev.memmap[memx][index];
    for (i = 0; i < nmemb; i++) {
        mallco_dev.memmap[memx][index + i] = 0;
    }
    return 0;
}

/**
 * @brief   内存释放 (用户接口)
 */
void myfree(uint8_t memx, void *ptr)
{
    uint32_t offset;
    if (ptr == NULL || memx >= SRAMBANK) return;
    offset = (uint32_t)ptr - (uint32_t)mallco_dev.membase[memx];
    my_mem_free(memx, offset);
}

/**
 * @brief   内存分配 (用户接口)
 */
void *mymalloc(uint8_t memx, uint32_t size)
{
    uint32_t offset = my_mem_malloc(memx, size);
    return offset == 0XFFFFFFFF ? NULL : (void *)((uint32_t)mallco_dev.membase[memx] + offset);
}

/**
 * @brief   内存重分配 (用户接口)
 */
void *myrealloc(uint8_t memx, void *ptr, uint32_t size)
{
    void *newptr;
    if (ptr == NULL) return mymalloc(memx, size);
    if (size == 0) {
        myfree(memx, ptr);
        return NULL;
    }
    newptr = mymalloc(memx, size);
    if (newptr) {
        mymemcpy(newptr, ptr, size);
        myfree(memx, ptr);
    }
    return newptr;
}
