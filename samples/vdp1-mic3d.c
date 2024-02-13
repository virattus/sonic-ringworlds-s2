#include <yaul.h>

#include <mic3d.h>


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>


extern const mesh_t mesh_m;
extern const mesh_t mesh_i;
extern const mesh_t mesh_c;


static vdp1_gouraud_table_t _pool_shading_tables[CONFIG_MIC3D_CMDT_COUNT] __aligned(16);
static vdp1_gouraud_table_t _pool_shading_tables2[512] __aligned(16);
static sort_list_t _sort_list[512] __aligned(4);


static workarea_mic3d_depth_values_t _pool_depth_values;
static workarea_mic3d_z_values_t _pool_z_values;
static workarea_mic3d_screen_points_t _pool_screen_points;
static workarea_mic3d_sort_singles_t _pool_sort_singles;
static workarea_mic3d_cmdts_t _pool_cmdts;
static workarea_mic3d_render_matrices_t _pool_render_matrices;
static workarea_mic3d_light_matrices_t _pool_light_matrices;
static workarea_mic3d_colors_t _pool_colors;
static workarea_mic3d_work_t _pool_work;


static workarea_mic3d_t _workarea = {
	&_pool_depth_values, &_pool_z_values, &_pool_screen_points,
	&_pool_sort_singles, &_pool_cmdts, &_pool_render_matrices,
	&_pool_light_matrices, &_pool_colors, &_pool_work,
};


void user_init();

static uint32_t _frame_time_calculate(void);


int main(void)
{
	Window_Init();
		
	gamewindow_t gw;
	gw.width = 320 * 3;
	gw.height = 224 * 3;
	gw.displayFramerate = true;
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

	dbgio_init();
	dbgio_dev_default_init(DBGIO_DEV_VDP2);
	dbgio_dev_font_load();

	smpc_peripheral_digital_t digital;

	vdp1_vram_partitions_t vdp1_vram_partitions;
	vdp1_vram_partitions_get(&vdp1_vram_partitions);
	
	mic3d_init(&_workarea);
	render_sort_depth_set(_sort_list, 512);
	
	light_gst_set(_pool_shading_tables, CONFIG_MIC3D_CMDT_COUNT, 512);
	
	vdp1_vram_t texture_base;
	texture_base = (vdp1_vram_t)vdp1_vram_partitions.texture_base;
	
	
	camera_t camera;
	
	camera.position.x = FIX16(0.0);
	camera.position.y = FIX16(0.0);
	camera.position.z = FIX16(10.0);
	
	camera.target.x = FIX16(0.0);
	camera.target.y = FIX16(0.0);
	camera.target.z = FIX16(0.0);
	
	camera.up.x = FIX16(0.0);
	camera.up.y = FIX16(1.0);
	camera.up.z = FIX16(1.0);
	
	camera_lookat(&camera);
	
	angle_t theta;
	
	theta = DEG2ANGLE(0.0);
	
	for(uint32_t i = 0; i < 512; i++)
	{
		const rgb1555_t color = RGB1555_INITIALIZER(1,
										fix16_int32_to(fix16_int32_from(i * 31) / 512U),
										fix16_int32_to(fix16_int32_from(i * 31) / 512U),
										fix16_int32_to(fix16_int32_from(i * 31) / 512U));
		
		_pool_shading_tables2[i].colors[0] = color;
		_pool_shading_tables2[i].colors[1] = color;
		_pool_shading_tables2[i].colors[2] = color;
		_pool_shading_tables2[i].colors[3] = color;

	}
	
	
	gst_set(0);
	gst_put(_pool_shading_tables2, 512);
	gst_unset();
	
	
	static fix16_mat43_t world[5];
	
	fix16_mat33_identity(&world[0].rotation);
	
	world[0].translation.x = FIX16(0.0);
	world[0].translation.y = FIX16(0.0);
	world[0].translation.z = FIX16(-100.0);
	
	
	vdp1_cmdt_t cmdt_polygon;
	vdp1_cmdt_polygon_set(&cmdt_polygon);
	vdp1_cmdt_draw_mode_t polygon_draw_mode;
	polygon_draw_mode.raw = 0x0000;
	vdp1_cmdt_draw_mode_set(&cmdt_polygon, polygon_draw_mode);
	
		
	while(GameWindow_HandleEvents()) 
	{
		render_start();
		
		render_enable(RENDER_FLAGS_LIGHTING);
		
		render_disable(RENDER_FLAGS_LIGHTING);
		
		render_end();
		
		
		

		vdp1_sync_render();
		vdp1_sync();
		vdp1_sync_wait();
		
			
		dbgio_flush();
		
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
	vdp2_tvmd_display_res_set(VDP2_TVMD_INTERLACE_NONE, VDP2_TVMD_HORZ_NORMAL_A, VDP2_TVMD_VERT_224);

	vdp2_scrn_back_color_set(VDP2_VRAM_ADDR(3, 0x01FFFE), RGB1555_INITIALIZER(1, 0, 3, 15));
	vdp2_sprite_priority_set(0, 6);	

	vdp1_sync_interval_set(-1);


	vdp1_env_t env;
	vdp1_env_default_init(&env);
	
	//env.erase_color = RGB1555(1, 0, 3, 15);

	vdp1_env_set(&env);

	vdp_sync_vblank_out_set(_vblank_out_handler, NULL);


	vdp2_tvmd_display_set();
	
	cd_block_init();
	
	smpc_peripheral_init();

	vdp2_sync();
	vdp2_sync_wait();
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

