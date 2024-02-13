#include <yaul.h>

#include <mic3d.h>

#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>



#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224


static smpc_peripheral_digital_t digital;

static void _vblank_out_handler(void*);

static void _menu_input(menu_t* menu);

static void _input_1(void* work, menu_entry_t* menu_entry);
static void _input_2(void* work, menu_entry_t* menu_entry);

static menu_entry_t _menu_entries[] = {
	MENU_ACTION_ENTRY("Item 1", _input_1),
	MENU_ACTION_ENTRY("Item 2", _input_1),
	MENU_ACTION_ENTRY("Item 3", NULL),
	MENU_ACTION_ENTRY("Item 4", NULL),
	MENU_ACTION_ENTRY("Item 5", NULL),
};



void user_init();


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
		.x = SCREEN_WIDTH,
		.y = SCREEN_HEIGHT,
	};
	
	Window_SetInternalResolution(&gw.win, res);
	
	user_init();
	
	
	menu_t state;
	
	menu_init(&state);
	menu_entries_set(&state, _menu_entries, sizeof(menu_entries) / sizeof(*_menu_entries));
	menu_input_set(&state, _menu_input);
	
	state.data = NULL;
	
	state.flags = MENU_ENABLED | MENU_INPUT_ENABLED;
	

	while(GameWindow_HandleEvents()) 
	{
		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &_digital);
		
		dbgio_printf("[H[2J");
		
		menu_update(&state);
		
		dbgio_flush();
		
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
	smpc_peripheral_init();

	vdp2_tvmd_display_res_set(VDP2_TVMD_INTERLACE_NONE, VDP2_TVMD_HORZ_NORMAL_A, VDP2_TVMD_VERT_224);
	
	vdp2_scrn_back_color_set(VDP2_VRAM_ADDR(2, 0x01FFFE), RGB1555(1, 0, 3, 15));
	
	vdp_sync_vblank_out_set(_vblank_out_handler, NULL);
	
	vdp2_tvmd_display_set();
}


void _menu_input(menu_t* menu)
{
	if((_digital.held.button.down) != 0)
	{
		menu_cursor_down_move(menu);
	}
	else if((_digital.held.button.up) != 0)
	{
		menu_cursor_up_move(menu);
	}
	else if((_digital.held.button.a) != 0)
	{
		menu_action_call(menu);
	}
}


void _input_1(void* state, menu_entry_t* menu_entry)
{
	dbgio_printf("Input 1 pressed\n");
}


void _input_2(void* state, menu_entry_t* menu_entry)
{
	dbgio_printf("Input 2 pressed\n");
}
