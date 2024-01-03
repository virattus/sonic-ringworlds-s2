#ifndef __FAKEYAUL_VDP2_VRAM_H__
#define __FAKEYAUL_VDP2_VRAM_H__


#include <stdint.h>


#define VDP2_VRAM_ADDR(bank, offset) 		(((bank) << 17) + (offset))

#define VDP2_VRAM_DIM 						512
#define VDP2_VRAM_BYTE_SIZE					0x0007FFFF

#define VDP2_VRAM_FRAMEBUFFER_SIZE			1048576


typedef uint32_t vdp2_vram_t;

#endif
