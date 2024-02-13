#ifndef __FAKEYAUL_VDP1_H__
#define __FAKEYAUL_VDP1_H__


#include <gamemath/vector.h>
#include <gamemath/int_vector.h>
#include <gfx/vram.h>
#include <gfx/vdp1/vdp1_vram.h>
#include <gfx/vdp1/vdp1_env.h>
#include <gfx/vdp1/vdp1_cmdt.h>
#include <gfx/vdp1/vdp1_sync.h>

#include <stdint.h>
#include <stdbool.h>


//DEFAULT CLEAR COLOUR IS RGB1555(0, 0, 0, 0)


#define VDP1_FRAMEBUFFER_SIZE		1048576
#define VDP1_FRAMEBUFFER_SIZE_BPP	4


#define VDP1_FLAG_IDLE				(0)
#define VDP1_FLAG_LIST_COMMITTED	(1 << 4)
#define VDP1_FLAG_CHANGED			(1 << 5)


//Good news everyone, there's two state structs, and you're combining them
typedef struct VDP1_STATE
{
	uint8_t flags;
	uint32_t frame_count;
	vdp1_vram_partitions_t vram_partitions;
	vdp1_env_t* current_env;
	
} vdp1_state_t;


vdp1_state_t* _state_vdp1(void);


void VDP1_Init();
void VDP1_Delete();


VRAM* VDP1_GetFramebuffer(uint32_t ID);
VRAM* VDP1_GetDisplayFramebuffer();
VRAM* VDP1_GetDrawFramebuffer();
VRAM* VDP1_GetTextureBuffer();

void VDP1_SwapBuffers();


#endif
