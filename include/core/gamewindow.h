#ifndef __FAKEYAUL_GAMEWINDOW_H__
#define __FAKEYAUL_GAMEWINDOW_H__


//Manager for handling a defacto game display window

#include <core/window.h>
#include <gamemath/int_vector.h>

#include <stdint.h>
#include <stdbool.h>


#define GAMEWINDOW_TITLE_MAX		128


typedef struct GAMEWINDOW
{
	WINDOW win;
	
	char title[GAMEWINDOW_TITLE_MAX];
	int16_vec2_t resolution;
	int16_vec2_t vdpResolution;
	bool displayFramerate;
	bool limitFramerate;
	uint32_t frameCount;
	uint64_t frameTicks;
	
} gamewindow_t;


int GameWindow_Open(gamewindow_t* win, const char* title);
void GameWindow_Close(gamewindow_t* win);


int GameWindow_HandleEvents();

void GameWindow_Update(gamewindow_t* win);




#endif
