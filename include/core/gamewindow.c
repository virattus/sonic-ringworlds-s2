#include <core/gamewindow.h>

#include <core/timer.h>
#include <gfx/vdp2/vdp2.h>


int GameWindow_Open(gamewindow_t* win, const char* title)
{
	strcpy(win->title, title);
	
	Window_Open(&win->gw, win->title, win->width, win->height, win->scale);

	
	return 1;
}


void GameWindow_Close(gamewindow_t* win)
{
	Window_Close(&win->gw);
}


int GameWindow_HandleEvents()
{
	if(!Window_PollEvents())
	{
		return 0;
	}
	
	return 1;
}


void GameWindow_Update(gamewindow_t* win)
{
	if(Timer_UpdateFPS())
	{
		char newTitle[TITLE_MAX];
		sprintf(newTitle, "%s (FPS: %d)", win->title, Timer_GetFPS());
		Window_SetTitle(&win->gw, newTitle); 
	}
	
	Window_ClearColour(&win->gw, 0xFFFF00FF);
	const int16_vec2_t FramebufferDimensions = { win->width, win->height };
	Window_BlitVRAM(&win->gw, VDP2_GetDisplayOutput(), &FramebufferDimensions);	
}
