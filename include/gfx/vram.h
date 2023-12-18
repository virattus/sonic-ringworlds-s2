#ifndef __FAKEYAUL_VRAM_H__
#define __FAKEYAUL_VRAM_H__



#include <gamemath/vector.h>
#include <core/colour.h>

#include <stdint.h>



typedef struct _VRAM
{
	void* Memory;
	uint32_t Size;
} VRAM;


void VRAM_Init(VRAM* ram, uint32_t sizeInBytes, uint32_t colour);
void VRAM_Free(VRAM* ram);

void VRAM_Fill(VRAM* ram, uint32_t start, uint32_t end, uint32_t colour);
void VRAM_Blit(VRAM* dest, VRAM* src, int16_vec2_t rect, int16_vec2_t offset);

void VRAM_WritePixel(VRAM* ram, uint32_t offset, uint32_t colour);

uint32_t VRAM_ReadPixel(const VRAM* ram, uint32_t offset);
uint16_t VRAM_ReadShort(const VRAM* ram, uint32_t offset);
uint8_t VRAM_ReadByte(const VRAM* ram, uint32_t offset);

CVECTOR VRAM_ReadPixel_CVEC(const VRAM* ram, uint32_t offset);



#endif
