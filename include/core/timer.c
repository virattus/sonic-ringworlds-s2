#include <core/timer.h>

#include <SDL2/SDL.h>


#define TICKS_PER_SECOND 1000

uint64_t FPS_LastTime = 0;
uint32_t FrameCounter = 0;
uint32_t FPS = 0;


void cpu_frt_init(uint8_t clock_div)
{
	
}


void cpu_frt_ovi_set(cpu_frt_ihr_t ihr)
{
}


void cpu_frt_count_set(uint16_t count)
{
}


uint32_t cpu_frt_count_get(void)
{
}




uint32_t Timer_GetFPS()
{
	return FPS;
}


int32_t Timer_UpdateFPS()
{
	FrameCounter++;
	
	if(FPS_LastTime < (SDL_GetTicks64() - 1000))
	{
		FPS_LastTime = SDL_GetTicks64();
		FPS = FrameCounter;
		FrameCounter = 0;
		return 1;
	}
	
	return 0;
}
