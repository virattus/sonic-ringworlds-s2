#include <gfx/vdp2/vdp2.h>

#include <core/preprocessor.h>
#include <core/colour.h>
#include <gfx/vram.h>
#include <gfx/vdp1/vdp1.h>
#include <gfx/vdp2/vdp2_render.h>
#include <gfx/vdp2/vdp2_scrn.h>


#include <assert.h>
#include <stdbool.h>



static bool vdp2_initialised = false;

static VRAM BackgroundRAM;
 
static VRAM DisplayOutput;


static vdp2_state_t VDP2_ACTIVE_STATE = {
	.sprite_priority[0] = 0,
	.sprite_priority[1] = 0,
	.sprite_priority[2] = 0,
	.sprite_priority[3] = 0,
	.sprite_priority[4] = 0,
	.sprite_priority[5] = 0,
	.sprite_priority[6] = 0,
	.sprite_priority[7] = 0,
	
	.back.vram = 0,
	.back.buffer = NULL,
	.back.len = 0,

	.tv.resolution = (int16_vec2_t){ 320, 240 },
	.tv.interlaceMode = VDP2_TVMD_INTERLACE_NONE,
	
	.InterlaceOffset = 0,
};


vdp2_state_t* _state_vdp2(void)
{
	return &VDP2_ACTIVE_STATE;
}


void VDP2_Init()
{
	if(vdp2_initialised)
	{
		return;
	}
	
	BackgroundRAM.Memory = NULL;
	BackgroundRAM.Size = 0;
	VRAM_Init(&BackgroundRAM, VDP2_VRAM_FRAMEBUFFER_SIZE, 0xFFFF00FF);

	DisplayOutput.Memory = NULL;
	DisplayOutput.Size = 0;
	VRAM_Init(&DisplayOutput, VDP2_VRAM_FRAMEBUFFER_SIZE, 0x000000FF);

	vdp2_scrn_priority_set(VDP2_SCRN_NBG0, 1);
	vdp2_scrn_priority_set(VDP2_SCRN_NBG1, 1);
	vdp2_scrn_priority_set(VDP2_SCRN_NBG2, 1);
	vdp2_scrn_priority_set(VDP2_SCRN_NBG3, 1);
	vdp2_scrn_priority_set(VDP2_SCRN_RBG0, 1);
	
	vdp2_scrn_reduction_x_set(VDP2_SCRN_NBG0, FIX16(1.0));
	vdp2_scrn_reduction_y_set(VDP2_SCRN_NBG0, FIX16(1.0));
	
	vdp2_scrn_reduction_x_set(VDP2_SCRN_NBG1, FIX16(1.0));
	vdp2_scrn_reduction_y_set(VDP2_SCRN_NBG2, FIX16(1.0));
	
	vdp2_scrn_back_color_set(VDP2_VRAM_ADDR(3, 0x0001FFFE), RGB1555(0, 0, 0, 0));
	
	__vdp2_cram_init();
	__vdp2_vram_init();

	vdp2_initialised = true;
}


void VDP2_Delete()
{
	if(!vdp2_initialised)
	{
		return;
	}
	
	VRAM_Free(&BackgroundRAM);
	VRAM_Free(&DisplayOutput);
	
	vdp2_initialised = false;
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
	assert(sprite_register < 8);
	
	_state_vdp2()->sprite_priority[sprite_register] = priority;
}


void vdp2_sync(void)
{
	VDP2_WaitThread();
}

void vdp2_sync_wait(void)
{
	vdp2_sync();
}
