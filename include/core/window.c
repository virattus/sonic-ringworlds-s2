#include <core/window.h>

#include <gfx/vdp1/vdp1.h>
#include <gfx/vdp2/vdp2.h>

#include <assert.h>


#define WINDOW_INTERNAL_FB_SIZE_X	320
#define WINDOW_INTERNAL_FB_SIZE_Y	240
#define WINDOW_INTERNAL_FB_BPP		4


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
	//SDL_Rect rect = { 0, 0, w->width * w->scale, w->height * w->scale };
	//SDL_BlitScaled(surf, NULL, SDL_GetWindowSurface(w->win), &rect);

	//SDL_UpdateWindowSurface(w->win);	
}


void Window_BlitVRAM(WINDOW* w, const VRAM* ram)
{
	
	//Supposedly the below is the correct way to do things, locking the texture should place
	//the location of the texture's pixel data in texPixels, but no matter how I pass it in, 
	//I get a segfault no matter what. Funny enough, I was originally passing in the VRAM's 
	//memory directly, and that actually worked! However, it is "unreliable" to lock and unlock
	//the texture immediately, so that's not really a solution. We'll look into this later.
	/*
	void* texPixels = NULL;
	
	if(SDL_LockTexture(w->win_texture, NULL, (void**)&texPixels, 
		WINDOW_INTERNAL_FB_SIZE_X * WINDOW_INTERNAL_FB_BPP) != 0)
	{
		printf("Unable to lock window internal texture! %s\n", SDL_GetError());
		return;
	}
	
	memcpy(texPixels, &ram->Memory, WINDOW_INTERNAL_FB_SIZE_X * WINDOW_INTERNAL_FB_SIZE_Y * WINDOW_INTERNAL_FB_BPP);
	SDL_UnlockTexture(w->win_texture);
	*/
	
	SDL_UpdateTexture(w->win_texture, NULL, ram->Memory, WINDOW_INTERNAL_FB_SIZE_X * WINDOW_INTERNAL_FB_BPP);
	SDL_RenderCopy(w->ren, w->win_texture, NULL, NULL);
	SDL_RenderPresent(w->ren);
}


void Window_SetClearColour(WINDOW* w, uint32_t colour)
{
	assert(w);
	assert(w->ren);
	
	w->clearColour = colour;
	
	SDL_SetRenderDrawColor(w->ren, (colour & 0xFF000000) >> 24, (colour & 0x00FF0000) >> 16, (colour & 0x0000FF00) >> 8, (colour & 0x000000FF));
}


void Window_Clear(WINDOW* w)
{
	assert(w);
	assert(w->ren);
	
	SDL_RenderClear(w->ren);
}


void Window_SetInternalResolution(WINDOW* w, int16_vec2_t resolution)
{
	assert(w);
	assert(w->ren);
	
	if(w->win_texture)
	{
		SDL_DestroyTexture(w->win_texture);
	}
	
	w->internalDimensions = resolution;
	
	SDL_RenderSetLogicalSize(w->ren, resolution.x, resolution.y);
	
	w->win_texture = SDL_CreateTexture(w->ren, 
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
		resolution.x, resolution.y);
}


int32_t Window_Open(WINDOW* w, const char* title, int32_t width, int32_t height)
{
	assert(w);

	w->win = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

	if(!w->win)
	{
		printf("Failed to create window: %s\n", title);
		return 0;
	}
	
	w->dimensions.x = width;
	w->dimensions.y = height;
	
	uint32_t renderFlags = SDL_RENDERER_ACCELERATED;
	if(w->flags.vsync)
	{
		renderFlags |= SDL_RENDERER_PRESENTVSYNC;
	}

	w->ren = SDL_CreateRenderer(w->win, -1, renderFlags);

	if(!w->ren)
	{
		printf("Failed to create renderer for window: %s\n", title);
		return 0;
	}

	SDL_RenderSetIntegerScale(w->ren, w->flags.integerScale); //1 is a boolean
	if(w->flags.integerScale)
	{
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	}
	else
	{
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	}

	w->win_texture = NULL;
	Window_SetInternalResolution(w, INT16_VEC2_INITIALIZER(WINDOW_INTERNAL_FB_SIZE_X, WINDOW_INTERNAL_FB_SIZE_Y));

	Window_SetClearColour(w, 0);

	if(!w->win_texture)
	{
		printf("Failed to create streaming texture for window: %s\n", title);
		return 0;
	}
	
	printf("Window created with dimensions %dx%d\n", width, height);

	return 1;
}


void Window_Close(WINDOW* w)
{
	assert(w);
	
	SDL_DestroyTexture(w->win_texture);
	SDL_DestroyRenderer(w->ren);
	SDL_DestroyWindow(w->win);
	
	w->win_texture = NULL;
	w->ren = NULL;
	w->win = NULL;
}


void Window_SetTitle(WINDOW* w, const char* title)
{
	assert(w);
	
	SDL_SetWindowTitle(w->win, title);
}
