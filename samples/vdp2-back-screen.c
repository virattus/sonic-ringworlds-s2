#include <yaul.h>

#include <stdio.h>
#include <stdlib.h>

#include <mic3d.h>

#include <gfx/vram.h>

#include <gfx/render/drawstate.h>
#include <gfx/render/primitive.h>
#include <gfx/render/software/draw.h>

#include <stdbool.h>
#include <assert.h>


#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224



void user_init();


int main(void)
{
	Window_Init();
		
	gamewindow_t gw;
	gw.resolution.x = 352 * 3;
	gw.resolution.y = 240 * 3;
	gw.displayFramerate = true;
	gw.win.flags.integerScale = 1;
	gw.win.flags.fullscreen = false;
	gw.win.flags.vsync = true;
	
	GameWindow_Open(&gw, "Ringworlds Stage 2");
	
	int16_vec2_t res = {
		.x = 320,
		.y = 224,
	};
	
	Window_SetInternalResolution(&gw.win, res);
	
	user_init();



	smpc_peripheral_digital_t digital;
	
	
	uint16_t width, height;
	vdp2_tvmd_display_res_get(&width, &height);
	
	rgb1555_t* const backBuffer = malloc(sizeof(rgb1555_t) * height);
	
	
	uint16_t buffer_count = 0;
	uint16_t count = 0;
	bool switch_buffer_count = false;
	

	while(GameWindow_HandleEvents()) 
	{
		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &digital);

		if(digital.held.a)
		{
			switch_buffer_count ^= true;
		}
		
		
		if(switch_buffer_count)
		{
			buffer_count = height;
		}
		else
		{
			buffer_count = 1;
		}
		
		
		vdp2_scrn_back_buffer_set(VDP2_VRAM_ADDR(0, 0), backBuffer, buffer_count);
		vdp2_scrn_back_sync();
		
		
		for(uint16_t i = 0; i < buffer_count; i++)
		{
			backBuffer[i] = RGB1555(1, i + count, i + count, i+ count);
		}
		
		count++;
		
		
		vdp2_sync();
		vdp2_sync_wait();
		
		GameWindow_Update(&gw);		
	}
	
	
	
	GameWindow_Close(&gw);
	
	return 0;
}


static void _vblank_out_handler(void* work __unused)
{
	smpc_peripheral_intback_issue();
}


static void _cpu_frt_ovi_handler(void)
{
}


void user_init(void)
{
	smpc_peripheral_init();

	vdp2_sprite_priority_set(0, 0);
	vdp2_sprite_priority_set(1, 0);	
	vdp2_sprite_priority_set(2, 0);	
	vdp2_sprite_priority_set(3, 0);	
	vdp2_sprite_priority_set(4, 0);	
	vdp2_sprite_priority_set(5, 0);	
	vdp2_sprite_priority_set(6, 0);	
	vdp2_sprite_priority_set(7, 0);	
	

	vdp_sync_vblank_out_set(_vblank_out_handler, NULL);

	vdp2_tvmd_display_res_set(VDP2_TVMD_INTERLACE_NONE, VDP2_TVMD_HORZ_NORMAL_A, VDP2_TVMD_VERT_224);

	vdp2_tvmd_display_set();
}





static size_t _texture_load(texture_t *textures, uint32_t slot, const picture_t *picture, vdp1_vram_t texture_base)
{
        texture_t * const texture = &textures[slot];

        texture->size       = TEXTURE_SIZE(picture->width, picture->height);
        texture->vram_index = TEXTURE_VRAM_INDEX(texture_base);

        scu_dma_transfer(0, (void *)texture_base, picture->data, picture->data_size);
        scu_dma_transfer_wait(0);

        return picture->data_size;
}


static void _palette_load(uint16_t bank_256, uint16_t bank_16, const palette_t *palette)
{
        scu_dma_transfer(0, (void *)VDP2_CRAM_MODE_0_OFFSET(bank_256, bank_16, 0), palette->data, palette->data_size);
        scu_dma_transfer_wait(0);
}


static uint32_t _frame_time_calculate(void)
{
	const uint16_t frt = cpu_frt_count_get();
	
	cpu_frt_count_set(0);
	
	const uint32_t delta_fix = frt << 8;
	const uint32_t divisor_fix = CPU_FRT_NTSC_320_32_COUNT_1MS << 4;
	
	cpu_divu_32_32_set(delta_fix, divisor_fix);
	
	uint32_t result;
	result = cpu_divu_quotient_get();
	result <<= 12;
	
	return result;
}

