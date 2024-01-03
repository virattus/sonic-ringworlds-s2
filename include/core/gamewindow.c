#include <core/gamewindow.h>

#include <core/timer.h>
#include <gfx/vdp1/vdp1.h>
#include <gfx/vdp1/vdp1_render.h>
#include <gfx/vdp2/vdp2.h>



int GameWindow_Open(gamewindow_t* gw, const char* title)
{
	//Initialise VDP1 and 2 if they haven't already
	VDP1_Init();
	VDP2_Init();
	
	strcpy(gw->title, title);
	
	Window_Open(&gw->win, gw->title, gw->width, gw->height);

	
	return 1;
}


void GameWindow_Close(gamewindow_t* gw)
{
	Window_Close(&gw->win);
}


int GameWindow_HandleEvents()
{
	if(!Window_PollEvents())
	{
		return 0;
	}
	
	return 1;
}


void GameWindow_Update(gamewindow_t* gw)
{
	
	
	if(gw->displayFramerate && Timer_UpdateFPS())
	{
		char newTitle[GAMEWINDOW_TITLE_MAX];
		sprintf(newTitle, "%s (FPS: %d)", gw->title, Timer_GetFPS());
		Window_SetTitle(&gw->win, newTitle); 
	}
	
	vdp2_state_t* vdp2State = _state_vdp2();
	
	
	Window_BlitVRAM(&gw->win, VDP2_GetDisplayOutput());
	
	//printf("Starting frame rendering\n");
}
