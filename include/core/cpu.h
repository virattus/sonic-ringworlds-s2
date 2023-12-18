#ifndef __FAKEYAUL_CPU_H__
#define __FAKEYAUL_CPU_H__


#include <stdint.h>


typedef uint32_t cpu_dmac_channel_t;


void cpu_divu_32_32_set(uint32_t divident, uint32_t divisor);

uint32_t cpu_divu_quotient_get(void);


#endif
