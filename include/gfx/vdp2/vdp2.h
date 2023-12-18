#ifndef __FAKEYAUL_VDP2_H__
#define __FAKEYAUL_VDP2_H__


#include <core/window.h>
#include <core/colour.h>
#include <gamemath/vector.h>
#include <gfx/vram.h>
#include <gfx/vdp2/vdp2_vram.h>

#include <stdint.h>

#define VDP2_VRAM_ADDR(bank, offset) (((bank) << 17) + (offset))

#define VDP2_VRAM_DIM 			512
#define VDP2_VRAM_BYTE_SIZE		0x0007FFFF


typedef uint32_t vdp2_sprite_register_t;


typedef struct VDP2_STATE
{
	struct
	{
		int16_vec2_t resolution;
	} tv;
	
	uint32_t BackgroundColour;
} vdp2_state_t;


vdp2_state_t* _state_vdp2(void);


void VDP2_Init();
void VDP2_Delete();

VRAM* VDP2_GetDisplayOutput();

void vdp2_sprite_priority_set(vdp2_sprite_register_t sprite_register, uint8_t priority);


void vdp2_sync(void);

void vdp2_sync_wait(void);


#endif
