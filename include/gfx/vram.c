#include <gfx/vram.h>

#include <gfx/render/render.h>
#include <gfx/texture.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define SDL_A_MASK 0x000000FF
#define SDL_R_MASK 0xFF000000
#define SDL_G_MASK 0x00FF0000
#define SDL_B_MASK 0x0000FF00


void VRAM_Init(VRAM* ram, uint32_t sizeInBytes, uint32_t colour)
{
	
	
	ram->Size = sizeInBytes;
	ram->Memory = malloc(sizeInBytes);
	printf("allocated vram of size %u\n", sizeInBytes);
	assert(sizeInBytes < 2000000);
	//ram->RAMSurface = SDL_CreateRGBSurface(0, dimensions->x, dimensions->y, 32, SDL_R_MASK, SDL_G_MASK, SDL_B_MASK, SDL_A_MASK);
	VRAM_Fill(ram, 0, sizeInBytes, colour);
}


void VRAM_Free(VRAM* ram)
{
	free(ram->Memory);
	ram->Size = 0;
}


void VRAM_Fill(VRAM* ram, uint32_t start, uint32_t end, uint32_t colour)
{
	assert(ram != NULL);
	assert(end - 1 < ram->Size);
	
	memset(ram->Memory + start, colour, end - start);
}


void VRAM_Blit(VRAM* dest, VRAM* src, int16_vec2_t rect, int16_vec2_t offset)
{
	assert(dest != NULL);
	assert(src != NULL);
	
	for(int x = 0; x < rect.x; x++)
	{
		for(int y = 0; y < rect.y; y++)
		{
			uint32_t pos = (y * rect.x) + x;
			VRAM_WritePixel(dest, pos, VRAM_ReadPixel(src, pos));//0xFF00FFFF + (0x00010000 * x));
		}
	}
	
}


void VRAM_WritePixel(VRAM* ram, uint32_t offset, uint32_t colour)
{
	assert(ram != NULL);
	assert((offset) < ram->Size);
	
	memcpy(ram->Memory + offset, &colour, 4);
}


uint32_t VRAM_ReadPixel(const VRAM* ram, uint32_t offset)
{
	assert(ram != NULL);
	assert((offset) < ram->Size);
	
	uint8_t* pixels = (uint8_t*)ram->Memory;
	return pixels[offset];
}
