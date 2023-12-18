#ifndef __FAKEYAUL_SATURN_CONTROL_UNIT_H__
#define __FAKEYAUL_SATURN_CONTROL_UNIT_H__



#include <stdint.h>
#include <stddef.h>


typedef uint32_t scu_dma_level_t;


void scu_dma_transfer(scu_dma_level_t level, void* dst, const void* src, size_t len);

void scu_dma_transfer_wait(scu_dma_level_t level);

#endif
