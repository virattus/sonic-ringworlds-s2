#include <gfx/vram.h>


#include <mic3d/texture.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define SDL_A_MASK 0x000000FF
#define SDL_R_MASK 0xFF000000
#define SDL_G_MASK 0x00FF0000
#define SDL_B_MASK 0x0000FF00


void VRAM_Init(VRAM* ram, uint32_t sizeInBytes, uint32_t val)
{
	ram->Size = sizeInBytes;
	ram->Memory = malloc(sizeInBytes);
	//printf("allocated vram of size %u\n", sizeInBytes);
	assert(sizeInBytes < 2000000);
	//ram->RAMSurface = SDL_CreateRGBSurface(0, dimensions->x, dimensions->y, 32, SDL_R_MASK, SDL_G_MASK, SDL_B_MASK, SDL_A_MASK);
	VRAM_Fill(ram, 0, sizeInBytes, val);
}


void VRAM_Free(VRAM* ram)
{
	assert(ram);
	
	if(ram->Size != 0)
	{
		free(ram->Memory);
		ram->Size = 0;
	}
}


void VRAM_Fill(VRAM* ram, uint32_t start, uint32_t end, uint32_t val)
{
	assert(ram != NULL);
	assert(end - 1 < ram->Size);
	
	memset(ram->Memory + start, val, end - start);
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
			VRAM_WriteInt(dest, pos, VRAM_ReadInt(src, pos));//0xFF00FFFF + (0x00010000 * x));
		}
	}
}


void VRAM_WriteInt(VRAM* ram, uint32_t offset, uint32_t val)
{
	assert(ram != NULL);
	assert((offset) < (ram->Size >> 2));
	
	//memcpy(ram->Memory + offset, &val, 4);
	
	uint32_t* pixels = (uint32_t*)ram->Memory;
	pixels[offset] = val;
}


void VRAM_WriteShort(VRAM* ram, uint32_t offset, uint16_t val)
{
	assert(ram != NULL);
	assert((offset) < (ram->Size >> 1));
	
	//memcpy(ram->Memory + offset, &val, 2);
	
	uint16_t* pixels = (uint16_t*)ram->Memory;
	pixels[offset] = val;
}


void VRAM_WriteByte(VRAM* ram, uint32_t offset, uint8_t val)
{
	assert(ram != NULL);
	assert((offset) < ram->Size);
	
	uint8_t* pixels = (uint8_t*)ram->Memory;
	pixels[offset] = val;
}


uint32_t VRAM_ReadInt(const VRAM* ram, uint32_t offset)
{
	assert(ram != NULL);
	assert((offset) < (ram->Size >> 2));
	
	uint32_t* pixels = (uint32_t*)ram->Memory;
	return pixels[offset];
}


uint16_t VRAM_ReadShort(const VRAM* ram, uint32_t offset)
{
	assert(ram != NULL);
	assert((offset) < (ram->Size >> 1));
	
	uint16_t* pixels = (uint16_t*)ram->Memory;
	return pixels[offset];
}


uint8_t VRAM_ReadByte(const VRAM* ram, uint32_t offset)
{
	assert(ram != NULL);
	assert((offset) < ram->Size);
	
	uint8_t* pixels = (uint8_t*)ram->Memory;
	return pixels[offset];
}
