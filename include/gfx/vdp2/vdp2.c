#include <gfx/vdp2/vdp2.h>

#include <core/preprocessor.h>
#include <gfx/vram.h>
#include <gfx/vdp1/vdp1.h>
#include <core/timer.h>
#include <core/window.h>


int __vdp2_initialised = 0;



static VRAM BackgroundRAM;
 
static VRAM DisplayOutput;


vdp2_state_t VDP2_ACTIVE_STATE;


vdp2_state_t* _state_vdp2(void)
{
	return &VDP2_ACTIVE_STATE;
}


void VDP2_Init()
{
	if(__vdp2_initialised) 
		return;
	
	VDP2_ACTIVE_STATE.tv.resolution = (int16_vec2_t){ 320, 224 };
	
	VRAM_Init(&BackgroundRAM, VDP2_VRAM_BYTE_SIZE, 0xFFFF00FF);
	VRAM_Init(&DisplayOutput, 
		VDP2_ACTIVE_STATE.tv.resolution.x * 
		VDP2_ACTIVE_STATE.tv.resolution.y * 4, 0xFFFFFFFF);

	__vdp2_initialised = 1;
}


void VDP2_Delete()
{
	if(!__vdp2_initialised) 
		return;
	
	//SDL_FreeSurface(BackgroundRAM.RAMSurface);
	VRAM_Free(&BackgroundRAM);
	VRAM_Free(&DisplayOutput);
	
	__vdp2_initialised = 0;
}


VRAM* VDP2_GetDisplayOutput()
{
	//return &DisplayOutput;
	return VDP1_GetDisplayFramebuffer();
}


void vdp2_sprite_priority_set(vdp2_sprite_register_t sprite_register, uint8_t priority)
{
}


void vdp2_sync(void)
{
	//Window_ClearColour(&mainWindow, VDP2_ACTIVE_STATE.BackgroundColour);
	//memcpy(&DisplayOutput.Memory, &VDP1_GetDisplayFramebuffer()->Memory, VDP1_GetDisplayFramebuffer()->Size);
	
}

void vdp2_sync_wait(void)
{
}
