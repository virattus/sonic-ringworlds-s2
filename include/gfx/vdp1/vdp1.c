#include <gfx/vdp1/vdp1.h>

#include <gfx/vram.h>
#include <gfx/vdp1/vdp1_env.h>
#include <gfx/vdp1/vdp1_cmdt.h>
#include <gfx/vdp1/vdp1_render.h>
#include <gfx/render/drawstate.h>


#include <stdio.h>
#include <assert.h>
#include <string.h>


static int __vdp1_initialised = 0;

static uint32_t ActiveFramebuffer = 0;
static VRAM Framebuffers[2];
static VRAM TextureArea;


static vdp1_state_t VDP1_ACTIVE_STATE = {
	.flags = 0,
	.frame_count = 0,
	.current_env = NULL,
};


vdp1_state_t* _state_vdp1(void)
{
	return &VDP1_ACTIVE_STATE;
}


void VDP1_Init()
{
	if(__vdp1_initialised) 
	{
		return;
	}
	
	assert(sizeof(vdp1_cmdt_t) == 32);
	
	__vdp1_env_init();
	
	vdp1_env_default_set();
	
	assert(VDP1_ACTIVE_STATE.current_env != NULL);
	
	Framebuffers[0].Memory = NULL;
	Framebuffers[0].Size = 0;
	Framebuffers[1].Memory = NULL;
	Framebuffers[1].Size = 0;
	
	VRAM_Init(&Framebuffers[0], VDP1_FRAMEBUFFER_SIZE, VDP1_ACTIVE_STATE.current_env->erase_color.raw);
	VRAM_Init(&Framebuffers[1], VDP1_FRAMEBUFFER_SIZE, VDP1_ACTIVE_STATE.current_env->erase_color.raw);
	
	VRAM_Init(&TextureArea, VDP1_VRAM_SIZE, 0);		
	
	__vdp1_initialised = 1;
	
	//Run this afterwards so that we don't lose the assert
	vdp1_vram_partitions_set(VDP1_VRAM_DEFAULT_CMDT_COUNT,
		VDP1_VRAM_DEFAULT_TEXTURE_SIZE,
		VDP1_VRAM_DEFAULT_GOURAUD_COUNT,
		VDP1_VRAM_DEFAULT_CLUT_COUNT);	
	
	//set all cmdts to be end codes, keeps the renderer from trying to write bad polygons
	vdp1_cmdt_t* cmdts = VDP1_ACTIVE_STATE.vram_partitions.cmdt_base;
	for(int i = 0; i < VDP1_VRAM_DEFAULT_CMDT_COUNT; i++)
	{
		vdp1_cmdt_end_set(&cmdts[i]);
	}
	
}


void VDP1_Delete()
{
	if(!__vdp1_initialised)
	{
		return;
	}
	
	VRAM_Free(&Framebuffers[0]);
	VRAM_Free(&Framebuffers[1]);
	VRAM_Free(&TextureArea);
	
	__vdp1_initialised = 0;
}


VRAM* VDP1_GetFramebuffer(uint32_t ID)
{
	assert(ID < 2);
	assert(__vdp1_initialised);
	
	return &Framebuffers[ID];
}


VRAM* VDP1_GetDisplayFramebuffer()
{
	return VDP1_GetFramebuffer(ActiveFramebuffer ^ 1);
}


VRAM* VDP1_GetDrawFramebuffer()
{
	return VDP1_GetFramebuffer(ActiveFramebuffer);
}


VRAM* VDP1_GetTextureBuffer()
{
	return &TextureArea;
}


void VDP1_SwapBuffers()
{	
	ActiveFramebuffer ^= 1;
	
	//TODO Probably take care of the cmdt buffers too
}

