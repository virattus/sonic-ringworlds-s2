#ifndef __FAKEYAUL_WINDOW_H__
#define __FAKEYAUL_WINDOW_H__



#include <gamemath/vector.h>
#include <gamemath/int_vector.h>
#include <gfx/vram.h>


#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct WINDOW_FLAGS
{
	bool fullscreen;
	bool vsync;
	bool integerScale;
} win_flags_t;


typedef struct _WINDOW
{
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_Texture* win_texture;
	int16_vec2_t dimensions;
	int16_vec2_t internalDimensions;
	uint32_t clearColour;
	win_flags_t flags;
} WINDOW;


int32_t Window_Init();
void Window_Cleanup();

int32_t Window_PollEvents();

void Window_BlitBuffer(WINDOW* w, SDL_Surface* surface);
void Window_BlitVRAM(WINDOW* w, const VRAM* ram);

void Window_SetClearColour(WINDOW* w, uint32_t colour);

void Window_Clear(WINDOW* w);

void Window_SetInternalResolution(WINDOW* w, int16_vec2_t resolution);

int32_t Window_Open(WINDOW* w, const char* title, int32_t width, int32_t height);
void Window_Close(WINDOW* w);

void Window_SetTitle(WINDOW* w, const char* title);

#endif
