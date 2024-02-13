#include <core/gamewindow.h>

#include <core/timer.h>
#include <gfx/vdp1/vdp1.h>
#include <gfx/vdp1/vdp1_render.h>
#include <gfx/vdp2/vdp2.h>


#define FRAMERATE 			(60)
#define TICKS_PER_FRAME 	(1000 / FRAMERATE + 1)


int GameWindow_Open(gamewindow_t* gw, const char* title)
{
	//Initialise VDP1 and 2 if they haven't already
	VDP2_Init();
	VDP1_Init(); //Confirmed that we init VDP1 second
	
	strcpy(gw->title, title);
	
	Window_Open(&gw->win, gw->title, gw->resolution.x, gw->resolution.y);

	gw->frameTicks = SDL_GetTicks64();
	
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
	if((vdp2State->tv.resolution.x != gw->vdpResolution.x) ||
		(vdp2State->tv.resolution.y != gw->vdpResolution.y))
	{
		gw->vdpResolution = vdp2State->tv.resolution;
		
		Window_SetInternalResolution(&gw->win, gw->vdpResolution);
	}
	
	
	
	Window_Clear(&gw->win);
	Window_BlitVRAM(&gw->win, VDP2_GetDisplayOutput());
	
	VDP1_SwapBuffers();
	VDP2_Render();
	
	
	if(gw->limitFramerate)
	{
		uint64_t timeDelta = SDL_GetTicks64();
		
		if((timeDelta - gw->frameTicks) < TICKS_PER_FRAME)
		{
			SDL_Delay(TICKS_PER_FRAME - (timeDelta - gw->frameTicks));
		}
		
		timeDelta = SDL_GetTicks64();
		//printf("Time Elapsed: %ld\n", (timeDelta - gw->frameTicks));
		gw->frameTicks = timeDelta;
	}
}
