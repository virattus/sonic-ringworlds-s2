#include <gfx/vdp2/vdp2.h>

#include <core/preprocessor.h>
#include <core/colour.h>
#include <gfx/vram.h>
#include <gfx/vdp1/vdp1.h>
#include <core/timer.h>
#include <core/window.h>


#include <SDL2/SDL_thread.h>
#include <assert.h>




static int __vdp2_initialised = 0;

static VRAM BackgroundRAM;
 
static VRAM DisplayOutput;


static SDL_Thread* VDP2_RenderThread = NULL;


static vdp2_state_t VDP2_ACTIVE_STATE = {
	.tv.resolution = (int16_vec2_t){ 320, 240 },
	.tv.interlaceMode = VDP2_TVMD_INTERLACE_NONE,
	
	.BackgroundColour = 0x000000FF,
	.InterlaceOffset = 0,
};


vdp2_state_t* _state_vdp2(void)
{
	return &VDP2_ACTIVE_STATE;
}


void VDP2_Init()
{
	if(__vdp2_initialised) 
		return;
	
	BackgroundRAM.Memory = NULL;
	BackgroundRAM.Size = 0;
	VRAM_Init(&BackgroundRAM, VDP2_VRAM_FRAMEBUFFER_SIZE, 0xFFFF00FF);

	DisplayOutput.Memory = NULL;
	DisplayOutput.Size = 0;
	VRAM_Init(&DisplayOutput, VDP2_VRAM_FRAMEBUFFER_SIZE, 0x000000FF);

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
	return &DisplayOutput;
}


VRAM* VDP2_GetTextureBuffer()
{
	return &BackgroundRAM;
}


void vdp2_sprite_priority_set(vdp2_sprite_register_t sprite_register, uint8_t priority)
{
}


void vdp2_sync(void)
{
	if(!VDP2_RenderThread)
	{
		VDP2_Render();
	}
	else
	{
		SDL_WaitThread(VDP2_RenderThread, NULL);
		VDP2_RenderThread = NULL;
	}
}

void vdp2_sync_wait(void)
{
	vdp2_sync();
}


static int VDP2_RenderThreadFunc(void* data)
{
	
	for(int j = 0; j < VDP2_ACTIVE_STATE.tv.resolution.y; j++)
	{
		
		for(int i = 0; i < VDP2_ACTIVE_STATE.tv.resolution.x; i++)
		{
			uint32_t destColour = VDP2_ACTIVE_STATE.BackgroundColour;
			uint32_t offset = (i + (j * VDP2_ACTIVE_STATE.tv.resolution.x));
			
			uint32_t vdp1Col = VRAM_ReadInt(VDP1_GetDisplayFramebuffer(), offset);
			
			if((vdp1Col & 0x000000FF))
			{
				destColour = vdp1Col;
			}
			
			VRAM_WriteInt(&DisplayOutput, offset, destColour);	
		}
	}
}


void VDP2_Render(void)
{
	assert(VDP2_RenderThread == NULL);
	
	VDP2_RenderThread = SDL_CreateThread(VDP2_RenderThreadFunc, "VDP2_Render", NULL);
}
