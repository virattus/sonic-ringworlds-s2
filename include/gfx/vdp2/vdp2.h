#ifndef __FAKEYAUL_VDP2_H__
#define __FAKEYAUL_VDP2_H__


#include <core/window.h>
#include <core/colour.h>
#include <gamemath/vector.h>
#include <gfx/vram.h>
#include <gfx/vdp2/vdp2_vram.h>
#include <gfx/vdp2/vdp2_tvmd.h>

#include <stdint.h>

#define VDP2_DISPLAY_OUTPUT_BPP		4


typedef uint32_t vdp2_sprite_register_t;


typedef struct VDP2_STATE
{	
	struct
	{
		int16_vec2_t resolution;
		vdp2_tvmd_interlace_t interlaceMode;
	} tv;
	
	uint32_t BackgroundColour;
	uint8_t InterlaceOffset; //Use this to determine if updating odd or even scanlines
	
} vdp2_state_t;


vdp2_state_t* _state_vdp2(void);


void VDP2_Init(void);
void VDP2_Delete(void);


VRAM* VDP2_GetDisplayOutput(void);
VRAM* VDP2_GetTextureBuffer(void);

void vdp2_sprite_priority_set(vdp2_sprite_register_t sprite_register, uint8_t priority);


void vdp2_sync(void);

void vdp2_sync_wait(void);


void VDP2_Render(void);


#endif
