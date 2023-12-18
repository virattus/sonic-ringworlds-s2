#ifndef __FAKEYAUL_TIMER_H__
#define __FAKEYAUL_TIMER_H__


#include <stdint.h>

typedef enum FRT_DIV_MODE
{
	CPU_FRT_CLOCK_DIV_8 = 0,
	CPU_FRT_CLOCK_DIV_32 = 1,
	CPU_FRT_CLOCK_DIV_128 = 2,
	CPU_FRT_CLOCK_DIV_RISING_EDGE = 3,
};


//the size of a tick count at specific resolutions
//technically the values are the actual size of the tick count
//This'll be redundant too
typedef enum FRT_NTSC_320_1MS
{
	CPU_FRT_NTSC_320_8_COUNT_1MS = 0x0D1F,
	CPU_FRT_NTSC_320_32_COUNT_1MS = 0x0348,
	CPU_FRT_NTSC_320_128_COUNT_1MS = 0x00D2,
};


//This is a callback handler?
typedef void (*cpu_frt_ihr_t)(void);


void cpu_frt_init(uint8_t clock_div);
void cpu_frt_ovi_set(cpu_frt_ihr_t ihr);


void cpu_frt_count_set(uint16_t count);
uint32_t cpu_frt_count_get(void);


uint32_t Timer_GetFPS();

int32_t Timer_UpdateFPS();


#endif
