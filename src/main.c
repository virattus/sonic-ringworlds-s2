#include <yaul.h>

#include <stdio.h>
#include <stdlib.h>

#include <mic3d.h>

#include <gfx/vdp1/vdp1_displaylist.h>


#include "backend/cd_loader.h"
#include "backend/workarea.h"

#include "object/character/character.h"
#include "object/character/sonic/sonic.h"

#include "collision/octree.h"

#include "backend/ssv.h"

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	224
#define SCREEN_SCALE	3


void user_init();



extern const mesh_t mesh_torus;

extern const mesh_t mesh_Cube;


static size_t _texture_load(texture_t *textures, uint32_t slot, const picture_t *picture, vdp1_vram_t texture_base);
static void _palette_load(uint16_t bank_256, uint16_t bank_16, const palette_t *palette);


static uint32_t _frame_time_calculate(void);



static vdp1_gouraud_table_t _pool_shading_tables[CONFIG_MIC3D_CMDT_COUNT] __aligned(16);
static vdp1_gouraud_table_t _pool_shading_tables2[512] __aligned(16);


static sort_list_t _sort_list[512] __aligned(4);


static smpc_peripheral_digital_t _digital;


static const char *_error_message =
    "[1;4H[2K[11CThe extended RAM\n"
    "[11Ccartridge is not\n"
    "[11Cinserted properly.\n"
    "\n"
    "[11CPlease turn off\n"
    "[11Cpower and re-insert\n"
    "[11Cthe extended RAM\n"
    "[11Ccartridge.\n";


int main(void)
{
	Window_Init();
	
	gamewindow_t gw;
	gw.width = SCREEN_WIDTH;
	gw.height = SCREEN_HEIGHT;
	gw.scale = SCREEN_SCALE;
	
	GameWindow_Open(&gw, "Ringworlds Stage 2");
		
	//user_init();
	
	// LOAD SSV FILE
	
	cdfs_config_default_set();
	
	CD_InitFileList();


	void* loadArea = LWRAM(0x00000000);
	uint32_t* loadInt = (uint32_t*)loadArea;
	*loadInt = 0xF1F1001F;

	CD_LoadFile("SNC.SSV", loadArea);
	//CD_LoadFile("bossfrost/bossfrost.smf", loadArea);
	
	
	// END LOAD SSV FILE
	
	ssv_mesh_t testmesh;
	SSV_LoadFromMemory(&testmesh, loadArea);
	
	dbgio_init();
	dbgio_dev_default_init(DBGIO_DEV_VDP2_ASYNC);
	dbgio_dev_font_load();
	

	
	
	/*
	 * //disabled until I figure out why it can't detect the cart in mednafen
	uint32_t ram_id;
	ram_id = dram_cart_id_get();
	if(ram_id != DRAM_CART_ID_4MIB)
	{
		dbgio_puts(_error_message);
		dbgio_printf("Cart ID: %d", ram_id);
		dbgio_flush();
		vdp2_sync();
		vdp2_sync_wait();
		abort();
	}
	*/
	

	vdp1_vram_partitions_t vdp1_vram_partitions;

	vdp1_vram_partitions_get(&vdp1_vram_partitions);

	mic3d_init(&_workarea);
	render_sort_depth_set(_sort_list, 512);

	//Init sonic
	char_sonic_t sonic;
	sonic.spatial.position = (fix16_vec3_t){ 0, FIX16(0), FIX16(-40) };
	sonic.spatial.velocity = (fix16_vec3_t){ 0, 0, 0 };
	sonic.rot = 0;
	sonic.mesh.pMesh = &mesh_Cube;


	light_gst_set(_pool_shading_tables,
		CONFIG_MIC3D_CMDT_COUNT,
		(vdp1_vram_t)(vdp1_vram_partitions.gouraud_base + 512));

	vdp1_vram_t texture_base;
	texture_base = (vdp1_vram_t)vdp1_vram_partitions.texture_base;

	

	camera_t camera;

	camera.position.x = FIX16( 0.0);
	camera.position.y = FIX16( 0.0);
	camera.position.z = FIX16( 10.0);

	camera.target.x = FIX16(0.0);
	camera.target.y = FIX16(0.0);
	camera.target.z = FIX16(0.0);

	camera.up.x = FIX16(0.0);
	camera.up.y = FIX16(1.0);
	camera.up.z = FIX16(0.0);

	camera_lookat(&camera);


	for (uint32_t i = 0; i < 512; i++) {
			const rgb1555_t color = RGB1555(1,
											fix16_int32_to(fix16_int32_from(i * 31) / 512U),
											fix16_int32_to(fix16_int32_from(i * 31) / 512U),
											fix16_int32_to(fix16_int32_from(i * 31) / 512U));

			_pool_shading_tables2[i].colors[0] = color;
			_pool_shading_tables2[i].colors[1] = color;
			_pool_shading_tables2[i].colors[2] = color;
			_pool_shading_tables2[i].colors[3] = color;
	}

	gst_set((vdp1_vram_t)vdp1_vram_partitions.gouraud_base);
	gst_put(_pool_shading_tables2, 512);
	gst_unset();

	/* Set up a command table for insertion */
	vdp1_cmdt_t cmdt_polygon;
	vdp1_cmdt_polygon_set(&cmdt_polygon);
	vdp1_cmdt_draw_mode_t polygon_draw_mode;
	polygon_draw_mode.raw = 0x0000;
	vdp1_cmdt_draw_mode_set(&cmdt_polygon, polygon_draw_mode);
	
	
	
	fix16_mat43_t ssv_world;
	fix16_mat33_identity(&ssv_world.rotation);
	fix16_vec3_zero(&ssv_world.translation);
	ssv_world.translation.z = FIX16(-10);
	
	
	
	int32_t frame_counter = 0;
	
	PRIMITIVE testPrim = {
		.zSort = 0,
		.flags = { 
			.draw_mesh = false, 
			.draw_paletted = false, 
			.highSpeedShrink = false, 
			.textureID = 0,
			.use_texture = false,
		},
		.v[0] = { { 0, 0 }, { 0, 0 }, { 255, 255, 255 } },
		.v[1] = { { 150, 0 }, { 255, 0 }, { 0, 0, 0 } }, 
		.v[2] = { { 150, 10 }, { 255, 255 }, { 0, 0, 0 } },
		.v[3] = { { 10, 10 }, { 0, 255 }, { 255, 255, 255 } },
	};
	
	fix16_vec3_t pos = { 0, 0, 0 };

	int loop = 1;
	while(GameWindow_HandleEvents()) 
	//while(loop)
	{
		loop = 0;
		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &_digital);
		
		/*
		 * Held only fires once per hold action, so use pressed when checking each frame
		 */
		if((_digital.pressed & PERIPHERAL_DIGITAL_LEFT) != 0) 
		{
			pos.x -= FIX16(0.1);
			//printf("moving left\n");
		}
		
		if((_digital.pressed & PERIPHERAL_DIGITAL_RIGHT) != 0) 
		{
			pos.x += FIX16(0.1);
			//printf("moving right\n");
		}
		
		if((_digital.pressed & PERIPHERAL_DIGITAL_UP) != 0) 
		{
			pos.z -= FIX16(0.1);
			//printf("moving up\n");
		}
		
		if((_digital.pressed & PERIPHERAL_DIGITAL_DOWN) != 0) 
		{
			pos.z += FIX16(0.1);
			//printf("moving down\n");
		}

		if((_digital.pressed & PERIPHERAL_DIGITAL_L) != 0) 
		{
			pos.y += FIX16(0.1);
			//printf("moving down\n");
		}
		if((_digital.pressed & PERIPHERAL_DIGITAL_R) != 0) 
		{
			pos.y -= FIX16(0.1);
			//printf("moving down\n");
		}		
		
		
		fix16_vec3_add(&sonic.spatial.position, &pos, &sonic.spatial.position); 
		
		fix16_vec3_zero(&pos);
		
		Sonic_Update(&sonic);
		
		char sonicPosBuffer[64];
		//fix16_vec3_str(&sonic.spatial.position, &sonicPosBuffer, 3);
		//printf("%s\n", sonicPosBuffer);

		/* Call this before rendering */
		render_start();
		
		render_enable(RENDER_FLAGS_LIGHTING);
		//Sonic_Draw(&sonic);
		
		render_disable(RENDER_FLAGS_LIGHTING);
		
		SSV_DrawMeshAnim(&testmesh, &sonic.mesh.matrix, 0);
		
		PRIMITIVE testPrim3 = testPrim;

		
		for(int i = 0; i < 4; i++)
		{
			testPrim3.v[i].pos.x += pos.x / 6553;
			testPrim3.v[i].pos.y += pos.z / 6553;
		}
		
		
		//DisplayList_VDP1_AddPrimitive(&testPrim);
		//DisplayList_VDP1_AddPrimitive(&testPrim3);
		

		/* End of rendering */
		render_end();

		vdp1_sync_render();
		vdp1_sync();
		vdp1_sync_wait();
		
		uint32_t frameT;
		frameT = _frame_time_calculate();
		
		char fixed[16];
		//fix16_str(frameT, fixed, 7);
		
		dbgio_printf("[H[2JFrame Counter: %d\n", frame_counter);
		
		dbgio_printf("%sms\n", fixed);
		
		dbgio_printf("LWRAM 0: %d\n", *(uint32_t*)loadArea);
		
		fix16_vec3_t dist;
		fix16_vec3_sub(&camera.position, &sonic.spatial.position, &dist);
		
		char buffer[64];
		fix16_vec3_str(&dist, buffer, 5);
		dbgio_printf("Distance From Camera: %s\n", buffer);
		

		dbgio_flush();
		
		frame_counter += 1;
		
		vdp2_sync();
		
		
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
	vdp2_tvmd_display_res_set(VDP2_TVMD_INTERLACE_NONE, VDP2_TVMD_HORZ_NORMAL_B, VDP2_TVMD_VERT_224);

	vdp2_scrn_back_color_set(VDP2_VRAM_ADDR(3, 0x01FFFE), RGB1555(1, 0, 3, 15));
	

	vdp1_sync_interval_set(-1);

	vdp1_env_default_set();

	vdp2_sprite_priority_set(0, 6);

	vdp2_tvmd_display_set();
	
	vdp_sync_vblank_out_set(_vblank_out_handler, NULL);
	
	cpu_frt_init(CPU_FRT_CLOCK_DIV_32);
	cpu_frt_ovi_set(_cpu_frt_ovi_handler);

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

