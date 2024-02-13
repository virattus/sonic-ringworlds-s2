#include <gfx/vdp2/vdp2_render.h>

#include <core/colour.h>
#include <gfx/vdp1/vdp1.h>
#include <gfx/vdp2/vdp2.h>
#include <gfx/vdp2/vdp2_screen_back.h>
#include <gfx/vdp2/vdp2_tilemap.h>

#include <SDL2/SDL_thread.h>
#include <assert.h>

#define RGBA_ZERO 			RGBA8888_INITIALIZER(0, 0, 0, 0)

static SDL_Thread* VDP2_RenderThread = NULL;



static rgba8888_t VDP2_GetVDP1PixelColour(const VRAM* vdp1Framebuffer, const vdp2_state_t* state, uint32_t offset, uint8_t* priority)
{
	rgb1555_t vdp1Col;
	vdp1Col.raw = VRAM_ReadShort(vdp1Framebuffer, offset);
	
	if(vdp1Col.raw) //Contains anything but 0
	{
		if(vdp1Col.msb)
		{
			*priority = state->sprite_priority[0];
			return RGB1555_to_RGBA8888(vdp1Col);
		}
		else
		{
			//TODO figure out how to get sprite ID
			//Turns out that sprite priority size and location is variable, so this will be fun
			*priority = state->sprite_priority[0]; 
			
			//temp
			return RGBA_ZERO;
		}
	}
	else
	{
		*priority = 0;
		return RGBA_ZERO;
	}
}




static void VDP2_ComposeLayers(VRAM* dest)
{
	vdp2_state_t vdp2_state = *_state_vdp2();
	const VRAM* vdp1DisplayFramebuffer = VDP1_GetDisplayFramebuffer();
	
	
	//build array of layers to go through
	
	
	for(int j = 0; j < vdp2_state.tv.resolution.y; j++)
	{
		rgba8888_t backColour = RGB1555_to_RGBA8888(vdp2_scrn_back_colour_get(j));
		
		for(int i = 0; i < vdp2_state.tv.resolution.x; i++)
		{
			rgba8888_t destColour = backColour;
			//Currently we assume that the offset is the same for both vdp1/2, but we can have different resolutions for both as it turns out
			uint32_t offset = (i + (j * vdp2_state.tv.resolution.x)); 
			uint8_t vdp1Priority;
			
			rgba8888_t vdp1Col = VDP2_GetVDP1PixelColour(vdp1DisplayFramebuffer, &vdp2_state, offset, &vdp1Priority);
			
			if(vdp1Priority != 0)
			{
				if(vdp1Col.raw != 0)
				{
					destColour = vdp1Col;
				}
			}
			else
			{
				destColour = (rgba8888_t)VDP2_Tilemap_GetPixel(&test_tilemap, INT16_VEC2_INITIALIZER(i, j));
				if((destColour.a & 0x80) == 0)
				{
					destColour = backColour;
				}
			}
			
			VRAM_WriteInt(dest, offset, destColour.raw);	
		}
	}
}






static int VDP2_RenderThreadFunc(void* data)
{
	VDP2_ComposeLayers(VDP2_GetDisplayOutput());
}



void VDP2_Render(void)
{	
#ifdef VDP2_MULTITHREADING
	assert(VDP2_RenderThread == NULL);

	VDP2_RenderThread = SDL_CreateThread(VDP2_RenderThreadFunc, "VDP2_Render", NULL);
#else
	VDP2_RenderThreadFunc(NULL);
#endif
}


void VDP2_WaitThread(void)
{
#ifdef VDP2_MULTITHREADING
	if(VDP2_RenderThread)
	{
		SDL_WaitThread(VDP2_RenderThread, NULL);
		VDP2_RenderThread = NULL;
	}
#endif
}

