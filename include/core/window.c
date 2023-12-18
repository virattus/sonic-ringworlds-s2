#include <core/window.h>

#include <gfx/vdp1/vdp1.h>
#include <gfx/vdp2/vdp2.h>

#include <assert.h>


int32_t Window_Init()
{
	if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER ) < 0)
	{
		printf("Failed to init SDL. Exitting.\n");
		return -1;
	}

	return 0;
}


void Window_Cleanup()
{
	Controller_Delete();
	
	VDP2_Delete();
	VDP1_Delete();

	SDL_Quit();
}


int32_t Window_PollEvents()
{	
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0)
	{
		if(e.type == SDL_QUIT)
		{
			return 0;
		}
		if(e.type == SDL_WINDOWEVENT
			&& e.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			return 0;
		}
	}
	
	return 1;
}


void Window_BlitBuffer(WINDOW* w, SDL_Surface* surf)
{
	SDL_Rect rect = { 0, 0, w->width * w->scale, w->height * w->scale };
	SDL_BlitScaled(surf, NULL, SDL_GetWindowSurface(w->win), &rect);

	SDL_UpdateWindowSurface(w->win);	
}


void Window_BlitVRAM(WINDOW* w, const VRAM* ram, const int16_vec2_t* dimensions)
{
	SDL_Surface* vramSurface = SDL_CreateRGBSurfaceFrom(
		ram->Memory, 
		dimensions->x, 
		dimensions->y, 
		32, 
		dimensions->x * 4, 
		SDL_R_MASK,
		SDL_G_MASK,
		SDL_B_MASK,
		SDL_A_MASK);
		
	if(!vramSurface)
	{
		printf("Failed to create surface from VRAM\n");
		return;
	}
	
	Window_BlitBuffer(w, vramSurface);
	SDL_FreeSurface(vramSurface);
}


void Window_ClearColour(WINDOW* w, uint32_t colour)
{
	SDL_FillRect(SDL_GetWindowSurface(w->win), NULL, colour);	
}


int32_t Window_Open(WINDOW* w, const char* title, int32_t width, int32_t height, int32_t scale)
{
	assert(w);

	w->win = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width * scale, height * scale, SDL_WINDOW_SHOWN);

	if(!w->win)
	{
		printf("Failed to create window with title %s", title);
		return -1;
	}
	
	w->width = width;
	w->height = height;
	w->scale = scale;
}


void Window_Close(WINDOW* w)
{
	SDL_DestroyWindow(w->win);
}


void Window_SetTitle(WINDOW* w, const char* title)
{
	SDL_SetWindowTitle(w->win, title);
}
