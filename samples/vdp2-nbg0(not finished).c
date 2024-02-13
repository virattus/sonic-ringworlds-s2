#include <yaul.h>

#include <mic3d.h>

#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>



#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224



static void _cpd_copy(uint8_t* cpd_base);
static void _palette_copy(rgb1555_t* palette_base);
static void _map_copy(const vdp2_scrn_cell_format_t* format, const vdp2_scrn_normal_map_t* normal_map);



void user_init();

static uint32_t _frame_time_calculate(void);

int main(void)
{
	Window_Init();
		
	gamewindow_t gw;
	gw.resolution.x = 320 * 3;
	gw.resolution.y = 224 * 3;
	gw.displayFramerate = true;
	gw.limitFramerate = false;
	gw.win.flags.integerScale = 1;
	gw.win.flags.fullscreen = false;
	gw.win.flags.vsync = false;
	
	GameWindow_Open(&gw, "Ringworlds Stage 2");
	
	int16_vec2_t res = {
		.x = 320,
		.y = 224,
	};
	
	Window_SetInternalResolution(&gw.win, res);
	
	user_init();
	
	const vdp2_scrn_cell_format_t format = {
		.scroll_screen = VDP2_SCRN_NBG0,
		.ccc = VDP2_SCRN_CCC_PALETTE_16,
		.char_size = VDP2_SCRN_CHAR_SIZE_1X1,
		.pnd_size = 1,
		.aux_mode = VDP2_SCRN_AUX_MODE_1,
		.plane_size = VDP2_SCRN_PLANE_SIZE_2X2,
		.cpd_base = (uint32_t)VDP2_VRAM_ADDR(0, 0X0000),
		.palette_base = (uint32_t)VDP2_CRAM_MODE_0_OFFSET(0, 0, 0),
	};
	
	const vdp2_scrn_normal_map_t normal_map = {
		.plane_a = (vdp2_vram_t)VDP2_VRAM_ADDR(0, 0x08000),
		.plane_b = (vdp2_vram_t)VDP2_VRAM_ADDR(0, 0x08000),
		.plane_c = (vdp2_vram_t)VDP2_VRAM_ADDR(0, 0x08000),
		.plane_d = (vdp2_vram_t)VDP2_VRAM_ADDR(0, 0x08000),
	};

	vdp2_scrn_cell_format_set(&format, &normal_map);
	vdp2_scrn_priority_set(VDP2_SCRN_NBG0, 7);
	vdp2_scrn_display_set(VDP2_SCRN_DISP_NBG0);
	
	_cpd_copy((uint8_t*)format.cpd_base);
	_palette_copy((rgb1555_t*)format.palette_base);
	_map_copy(&format, &normal_map);
	
	vdp2_tvmd_display_set();
	
	vdp2_sync();
	vdp2_sync_wait();

	smpc_peripheral_digital_t digital;

	while(GameWindow_HandleEvents()) 
	{
		GameWindow_Update(&gw);		
		
		//break;
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
	vdp2_scrn_back_color_set(VDP2_VRAM_ADDR(2, 0x01FFFE), RGB1555(1, 0, 0, 7));
	
	vdp2_tvmd_display_res_set(VDP2_TVMD_INTERLACE_NONE, VDP2_TVMD_HORZ_NORMAL_A, VDP2_TVMD_VERT_224);
}


void _cpd_copy(uint8_t* cpd_base)
{
	(void)memset(cpd_base + 0x00, 0x00, 0x20);
	(void)memset(cpd_base + 0x20, 0x11, 0x20);
}


void _palette_copy(rgb1555_t* palette_base)
{
	palette_base[0] = RGB1555(1, 31, 31, 31);
	palette_base[1] = RGB1555(1, 31, 0, 0);
}


void _map_copy(const vdp2_scrn_cell_format_t* format, const vdp2_scrn_normal_map_t* normal_map)
{
	const uint32_t page_width = VDP2_SCRN_PAGE_WIDTH_CALCULATE(format);
	const uint32_t page_height = VDP2_SCRN_PAGE_HEIGHT_CALCULATE(format);
	const uint32_t page_size = VDP2_SCRN_PAGE_SIZE_CALCULATE(format);
	
	uint16_t* const a_pages[4] = {
		(uint16_t*)normal_map->base_addr[0],
		(uint16_t*)normal_map->base_addr[1 * (page_size / sizeof(uint16_t))],
		(uint16_t*)normal_map->base_addr[2 * (page_size / sizeof(uint16_t))],
		(uint16_t*)normal_map->base_addr[3 * (page_size / sizeof(uint16_t))]
	};
	
	uint16_t tile = 0;
	
	for(uint32_t page_y = 0; page_y < page_height; page_y++)
	{
		for(uint32_t page_x = 0; page_x < page_width; page_x++)
		{
			const uint16_t page_idx = page_x + (page_width * page_y);
			const uint16_t pnd = VDP2_SCRN_PND_CONFIG_1(1, (uint32_t)format->cpd_base, (uint32_t)format->palette_base);
			
			a_pages[0][page_idx] = pnd | tile;
			
			tile ^= 1;
		}
		
		tile ^= 1;
	}
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

