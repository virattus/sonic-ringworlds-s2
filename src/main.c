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


#define STATE_IDLE 			(-1)
#define STATE_TYPE_SET 		(0)
#define STATE_DRAW_MODE_SET (1)
#define STATE_VERTEX_SELECT (2)
#define STATE_VERTEX_RESET 	(3)


#define PRIMITIVE_TYPE_POLYLINE 	(0)
#define PRIMITIVE_TYPE_POLYGON 		(1)
#define PRIMITIVE_TYPE_LINE 		(2)
#define PRIMITIVE_TYPE_COUNT 		(3)

#define PRIMITIVE_DRAW_MODE_NORMAL				(0)
#define PRIMITIVE_DRAW_MODE_MESH				(1)
#define PRIMITIVE_DRAW_MODE_SHADOW				(2)
#define PRIMITIVE_DRAW_MODE_HALF_LUMINANCE		(3)
#define PRIMITIVE_DRAW_MODE_HALF_TRANSPARENT 	(4)
#define PRIMITIVE_DRAW_MODE_GOURAUD_SHADING		(5)
#define PRIMITIVE_DRAW_MODE_GOURAUD_HALF_LUM	(6)
#define PRIMITIVE_DRAW_MODE_GOURAUD_HALF_TRANS	(7)
#define PRIMITIVE_DRAW_MODE_COUNT 				(8)

#define PRIMITIVE_WIDTH 		32
#define PRIMITIVE_HEIGHT 		32
#define PRIMITIVE_HALF_WIDTH 	(PRIMITIVE_WIDTH / 2)
#define PRIMITIVE_HALF_HEIGHT 	(PRIMITIVE_HEIGHT / 2)

#define PRIMITIVE_COLOR RGB1555(1, 31, 31, 31)

#define ORDER_SYSTEM_CLIP_COORDS_INDEX 0
#define ORDER_LOCAL_COORDS_INDEX       1
#define ORDER_POLYGON_INDEX            2
#define ORDER_DRAW_END_INDEX           3
#define ORDER_COUNT                    4


#define CMDT_VTX_POLYGON_A 0
#define CMDT_VTX_POLYGON_B 1
#define CMDT_VTX_POLYGON_C 2
#define CMDT_VTX_POLYGON_D 3


static struct {
	int8_t type;
	int8_t draw_mode;
	rgb1555_t colour;
	int16_vec2_t points[4];
} _primitive;


static uint32_t _state = STATE_IDLE;


static vdp1_cmdt_draw_mode_t _primitive_draw_modes[] = {
	{
		.raw = 0x0000,
	},
	{
		.mesh_enable = true,
	},
	{
		.cc_mode = 1,
	},
	{
		.cc_mode = 2,
	},
	{
		.cc_mode = 3,
	},
	{
		.cc_mode = 4,
	},
	{
		.cc_mode = 6,
	},
	{
		.cc_mode = 7,
	},
};


static vdp1_cmdt_list_t* _cmdt_list = NULL;
static vdp1_vram_partitions_t _vdp1_vram_partitions;


static char* _primitive_draw_mode_strings[] = {
	"NORMAL",
	"MESH",
	"SHADOW"
	"HALF-LUMINANCE",
	"REPLACE/HALF-TRANSPARENT",
	"GOURAUD",
	"GOURAUD + HALF-LUMINANCE",
	"GOURAUD + HALF-TRANSPARENT",
	"This should not appear",
};

static char* _primitive_type_strings[] = {
	"POLYLINE",
	"POLYGON",
	"LINE",
};



void user_init();

static uint32_t _frame_time_calculate(void);


static void InitTilemap(void)
{
	for(int i = 0; i < TILEMAP_TILE_COUNT; i++)
	{
		test_tilemap.TileOffset[i] = i * 64;
	}
	
	for(int i = 0; i < TILEMAP_DIMENSION; i++)
	{
		for(int j = 0; j < TILEMAP_DIMENSION; j++)
		{
			test_tilemap.TileMap[i][j] = 0;
		}
	}
	
	
	test_tilemap.TileMapScroll.x = 0;
	test_tilemap.TileMapScroll.y = 0;
	
	uint32_t* pixels = (uint32_t*)VDP2_GetTextureBuffer()->Memory;
	
	pixels[0] = 0xFFFFFFFF;
	pixels[1] = 0xFFFFFFFF;
	pixels[2] = 0xFFFFFFFF;
	pixels[3] = 0;
	pixels[4] = 0xFFFFFFFF;
	pixels[5] = 0xFFFFFFFF;
	pixels[6] = 0xFFFFFFFF;
	pixels[7] = 0xFFFFFFFF;
	
	pixels[8] = 0xFFFFFFFF;
	pixels[9] = 0xFFFFFFFF;
	pixels[10] = 0;
	pixels[11] = 0xFFFFFFFF;
	pixels[12] = 0;
	pixels[13] = 0xFFFFFFFF;
	pixels[14] = 0xFFFFFFFF;
	pixels[15] = 0xFFFFFFFF;
	
	pixels[16] = 0xFFFFFFFF;
	pixels[17] = 0xFFFFFFFF;
	pixels[18] = 0;
	pixels[19] = 0xFFFFFFFF;
	pixels[20] = 0;
	pixels[21] = 0xFFFFFFFF;
	pixels[22] = 0xFFFFFFFF;
	pixels[23] = 0xFFFFFFFF;
	
	pixels[24] = 0xFFFFFFFF;
	pixels[25] = 0;
	pixels[26] = 0xFFFFFFFF;
	pixels[27] = 0xFFFFFFFF;
	pixels[28] = 0xFFFFFFFF;
	pixels[29] = 0;
	pixels[30] = 0xFFFFFFFF;
	pixels[31] = 0xFFFFFFFF;
	
	pixels[32] = 0xFFFFFFFF;
	pixels[33] = 0;
	pixels[34] = 0;
	pixels[35] = 0;
	pixels[36] = 0;
	pixels[37] = 0;
	pixels[38] = 0xFFFFFFFF;
	pixels[39] = 0xFFFFFFFF;
	
	pixels[40] = 0xFFFFFFFF;
	pixels[41] = 0;
	pixels[42] = 0xFFFFFFFF;
	pixels[43] = 0xFFFFFFFF;
	pixels[44] = 0xFFFFFFFF;
	pixels[45] = 0;
	pixels[46] = 0xFFFFFFFF;
	pixels[47] = 0xFFFFFFFF;
	
	pixels[48] = 0;
	pixels[49] = 0xFFFFFFFF;
	pixels[50] = 0xFFFFFFFF;
	pixels[51] = 0xFFFFFFFF;
	pixels[52] = 0xFFFFFFFF;
	pixels[53] = 0xFFFFFFFF;
	pixels[54] = 0;
	pixels[55] = 0xFFFFFFFF;
	
	pixels[56] = 0;
	pixels[57] = 0xFFFFFFFF;
	pixels[58] = 0xFFFFFFFF;
	pixels[59] = 0xFFFFFFFF;
	pixels[60] = 0xFFFFFFFF;
	pixels[61] = 0xFFFFFFFF;
	pixels[62] = 0;
	pixels[63] = 0xFFFFFFFF;
}


static void cmdt_list_init(void)
{
	static const int16_vec2_t system_clip_coord = {
		SCREEN_WIDTH - 1,
		SCREEN_HEIGHT - 1,
	};
	
	_cmdt_list = vdp1_cmdt_list_alloc(ORDER_COUNT);
	
	(void)memset(&_cmdt_list->cmdts[0], 0x00, sizeof(vdp1_cmdt_t) * ORDER_COUNT);
	
	_cmdt_list->count = ORDER_COUNT;
	
	vdp1_cmdt_t* cmdts;
	cmdts = &_cmdt_list->cmdts[0];
	
	vdp1_cmdt_system_clip_coord_set(&cmdts[ORDER_SYSTEM_CLIP_COORDS_INDEX]);
	vdp1_cmdt_vtx_system_clip_coord_set(&cmdts[ORDER_SYSTEM_CLIP_COORDS_INDEX], system_clip_coord);
	
	vdp1_cmdt_end_set(&cmdts[ORDER_DRAW_END_INDEX]);
}


static void primitive_init(void)
{
	static const int16_vec2_t local_coord_centre = {
		(SCREEN_WIDTH / 2) - PRIMITIVE_HALF_WIDTH - 1,
		(SCREEN_HEIGHT / 2) - PRIMITIVE_HALF_HEIGHT - 1
	};
	
	_primitive.type = PRIMITIVE_TYPE_POLYGON;
	_primitive.draw_mode = PRIMITIVE_DRAW_MODE_GOURAUD_SHADING;
	
	_primitive.colour = PRIMITIVE_COLOR;
	
	_primitive.points[0].x = 0;
	_primitive.points[0].y = PRIMITIVE_HEIGHT - 1;
	
	_primitive.points[1].x = PRIMITIVE_WIDTH - 1;
	_primitive.points[1].y = PRIMITIVE_HEIGHT - 1;
	
	_primitive.points[2].x = PRIMITIVE_WIDTH - 1;
	_primitive.points[2].y = 0;
	
	_primitive.points[3].x = 0;
	_primitive.points[3].y = 0;
	
	vdp1_cmdt_t* cmdt_local_coords;
	cmdt_local_coords = &_cmdt_list->cmdts[ORDER_LOCAL_COORDS_INDEX];
	
	vdp1_cmdt_local_coord_set(cmdt_local_coords);
	vdp1_cmdt_vtx_local_coord_set(cmdt_local_coords, local_coord_centre);
	
	vdp1_cmdt_t* cmdt_polygon;
	cmdt_polygon = &_cmdt_list->cmdts[ORDER_POLYGON_INDEX];
	
	vdp1_gouraud_table_t* gouraud_base;
	gouraud_base = _vdp1_vram_partitions.gouraud_base;
	
	gouraud_base->colors[0] = RGB1555(1, 31, 0, 0);
	gouraud_base->colors[1] = RGB1555(1, 0, 31, 0);
	gouraud_base->colors[2] = RGB1555(1, 0, 0, 31);
	gouraud_base->colors[3] = RGB1555(1, 31, 31, 31);
	
	
	vdp1_cmdt_polyline_set(cmdt_polygon);
	vdp1_cmdt_colour_set(cmdt_polygon, _primitive.colour);
	vdp1_cmdt_draw_mode_set(cmdt_polygon, _primitive_draw_modes[_primitive.draw_mode]);
	vdp1_cmdt_vtx_set(cmdt_polygon, &_primitive.points[0]);
	
	//assert(cmdt_polygon->cmd_draw_mode.mesh_enable != true);
	
	//We use offsets instead of the actual position, original below
	vdp1_cmdt_gouraud_base_set(cmdt_polygon, 0);
	//vdp1_cmdt_gouraud_base_set(cmdt_polygon, (uint32_t)gouraud_base);
	
}


const uint16_t mask_pressed_buttons = 
	PERIPHERAL_DIGITAL_X |
	PERIPHERAL_DIGITAL_Y |
	PERIPHERAL_DIGITAL_B |
	PERIPHERAL_DIGITAL_A;

const uint16_t mask_pressed_type = 
	PERIPHERAL_DIGITAL_LEFT |
	PERIPHERAL_DIGITAL_RIGHT;
	

const uint16_t mask_pressed_draw_mode = 
	PERIPHERAL_DIGITAL_UP |
	PERIPHERAL_DIGITAL_DOWN;


int main(void)
{
	Window_Init();
		
	gamewindow_t gw;
	gw.resolution.x = 352 * 3;
	gw.resolution.y = 240 * 3;
	gw.displayFramerate = true;
	gw.limitFramerate = true;
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

	cmdt_list_init();
	primitive_init();
	
	InitTilemap();

	uint16_t width, height;
	vdp2_tvmd_display_res_get(&width, &height);
	
	rgb1555_t* const backBuffer = malloc(sizeof(rgb1555_t) * height);
	
	
	uint16_t buffer_count = 0;
	uint16_t count = 0;
	bool switch_buffer_count = false;

	smpc_peripheral_digital_t digital;

	while(GameWindow_HandleEvents()) 
	{
		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &digital);

		uint32_t vertex_index = CMDT_VTX_POLYGON_A;
		
		_state = STATE_IDLE;
		
		const uint16_t pressed_state = digital.pressed.raw;
		const uint16_t held_state = digital.held.raw;
		
		//printf("pressed state: %d\n", pressed_state);
		//printf("held_state: %d\n", held_state);
		
		if((pressed_state & mask_pressed_buttons) != 0x0000)
		{
			_state = STATE_VERTEX_SELECT;
		}
		else if((pressed_state & mask_pressed_type) != 0x0000)
		{
			_state = STATE_TYPE_SET;
		}
		else if((pressed_state & mask_pressed_draw_mode) != 0x0000)
		{
			_state = STATE_DRAW_MODE_SET;
		}
		else if((pressed_state & PERIPHERAL_DIGITAL_START) != 0x0000)
		{
			_state = STATE_VERTEX_RESET;
		}
		
		switch(_state)
		{
			case STATE_VERTEX_RESET:
				primitive_init();
				break;
			case STATE_TYPE_SET:
				if((held_state & PERIPHERAL_DIGITAL_LEFT) != 0x0000)
				{
					_primitive.type--;
					if(_primitive.type < 0)
					{
						_primitive.type = PRIMITIVE_TYPE_COUNT - 1;
					}
				}
				else if((held_state & PERIPHERAL_DIGITAL_RIGHT) != 0x000)
				{
					_primitive.type++;
					if(_primitive.type >= PRIMITIVE_TYPE_COUNT)
					{
						_primitive.type = 0;
					}
				}
				break;
			case STATE_DRAW_MODE_SET:
				if((held_state & PERIPHERAL_DIGITAL_UP) != 0x0000)
				{
					_primitive.draw_mode--;
					if(_primitive.draw_mode < 0)
					{
						_primitive.draw_mode = PRIMITIVE_DRAW_MODE_COUNT - 1;
					}
				}
				else if((held_state & PERIPHERAL_DIGITAL_DOWN) != 0x0000)
				{
					_primitive.draw_mode++;
					if(_primitive.draw_mode >= PRIMITIVE_DRAW_MODE_COUNT)
					{
						_primitive.draw_mode = 0;
					}
				}
				break;
			case STATE_VERTEX_SELECT:
				if((pressed_state & PERIPHERAL_DIGITAL_X) != 0x0000)
				{
					vertex_index = CMDT_VTX_POLYGON_D;
				}
				else if((pressed_state & PERIPHERAL_DIGITAL_Y) != 0x0000)
				{
					vertex_index = CMDT_VTX_POLYGON_C;
				}
				else if((pressed_state & PERIPHERAL_DIGITAL_B) != 0x0000)
				{
					vertex_index = CMDT_VTX_POLYGON_B;
				}
				else if((pressed_state & PERIPHERAL_DIGITAL_A) != 0x0000)
				{
					vertex_index = CMDT_VTX_POLYGON_A;
				}
				
				if((pressed_state & PERIPHERAL_DIGITAL_LEFT) != 0x0000)
				{
					_primitive.points[vertex_index].x--;
				}
				else if((pressed_state & PERIPHERAL_DIGITAL_RIGHT) != 0x0000)
				{
					_primitive.points[vertex_index].x++;
				}
				
				if((pressed_state & PERIPHERAL_DIGITAL_UP) != 0x0000)
				{
					_primitive.points[vertex_index].y--;
				}
				else if((pressed_state & PERIPHERAL_DIGITAL_DOWN) != 0x0000)
				{
					_primitive.points[vertex_index].y++;
				}
				break;
		}

		vdp1_cmdt_t* cmdt_polygon;
		cmdt_polygon = &_cmdt_list->cmdts[ORDER_POLYGON_INDEX];
		
		switch(_primitive.type)
		{
			case PRIMITIVE_TYPE_POLYLINE:
				vdp1_cmdt_polyline_set(cmdt_polygon);
				break;
			case PRIMITIVE_TYPE_POLYGON:
				vdp1_cmdt_polygon_set(cmdt_polygon);
				break;
			case PRIMITIVE_TYPE_LINE:
				vdp1_cmdt_line_set(cmdt_polygon);
				break;
		}
		
		
		dbgio_printf("[H[2J"
			"     type: %s\n"
			"draw_mode: %s\n",
			_primitive_type_strings[_primitive.type],
			_primitive_draw_mode_strings[_primitive.draw_mode]);
			
		dbgio_flush();
		
		/*
		printf(			"     type: %s\n"
			"draw_mode: %s\n",
			_primitive_type_strings[_primitive.type],
			_primitive_draw_mode_strings[_primitive.draw_mode]);
		
		printf("Draw Mode Raw: %d\n", _primitive.draw_mode);
		*/
		
		vdp1_cmdt_draw_mode_set(cmdt_polygon, _primitive_draw_modes[_primitive.draw_mode]);
		vdp1_cmdt_vtx_set(cmdt_polygon, &_primitive.points[0]);


		if(digital.held.button.a)
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
		
		
		vdp2_scrn_back_buffer_set(VDP2_VRAM_ADDR(1, 0), backBuffer, buffer_count);
		vdp2_scrn_back_sync();
		
		
		for(uint16_t i = 0; i < buffer_count; i++)
		{
			backBuffer[i] = RGB1555(1, i + count, i + count, i+ count);
		}
		
		count++;



		vdp1_sync_cmdt_list_put(_cmdt_list, 0);
		vdp1_sync_render();
		vdp1_sync();
		vdp2_sync();
		vdp2_sync_wait();
		
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

	vdp2_scrn_back_color_set(VDP2_VRAM_ADDR(3, 0x01FFFE), RGB1555(1, 0, 3, 15));
	vdp2_sprite_priority_set(0, 6);	

	vdp1_sync_interval_set(-1);


	vdp1_env_t env;
	vdp1_env_default_init(&env);
	
	env.erase_color = RGB1555(1, 0, 3, 15);

	vdp1_env_set(&env);

	vdp_sync_vblank_out_set(_vblank_out_handler, NULL);


	vdp2_tvmd_display_set();
	
	vdp1_vram_partitions_get(&_vdp1_vram_partitions);

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

