#ifndef __FAKEYAUL_WINDOW_H__
#define __FAKEYAUL_WINDOW_H__



#include <gamemath/vector.h>
#include <core/controller.h>
#include <gfx/vram.h>


#include <SDL2/SDL.h>
#include <stdlib.h>


typedef struct _WINDOW
{
	SDL_Window* win;
	int32_t width, height, scale;
} WINDOW;


int32_t Window_Init();
void Window_Cleanup();

int32_t Window_PollEvents();

void Window_BlitBuffer(WINDOW* w, SDL_Surface* surface);
void Window_BlitVRAM(WINDOW* w, const VRAM* ram, const int16_vec2_t* dimensions);

void Window_ClearColour(WINDOW* w, uint32_t colour);

int32_t Window_Open(WINDOW* w, const char* title, int32_t width, int32_t height, int32_t scale);
void Window_Close(WINDOW* w);

void Window_SetTitle(WINDOW* w, const char* title);

#endif
