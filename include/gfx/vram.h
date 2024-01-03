#ifndef __FAKEYAUL_VRAM_H__
#define __FAKEYAUL_VRAM_H__



#include <gamemath/vector.h>


#include <stdint.h>


/*
 * Saturn VRAM is always read as 16 bits, at least I think, might be different for VDP2?
 * If this holds up, we'll probably rewrite this to always be 16 bit aligned, which would be
 * nice and clean up the implementation
 */


typedef struct _VRAM
{
	void* Memory;
	uint32_t Size;
} VRAM;


void VRAM_Init(VRAM* ram, uint32_t sizeInBytes, uint32_t val);
void VRAM_Free(VRAM* ram);

void VRAM_Fill(VRAM* ram, uint32_t start, uint32_t end, uint32_t val);
void VRAM_Blit(VRAM* dest, VRAM* src, int16_vec2_t rect, int16_vec2_t offset);

void VRAM_WriteInt(VRAM* ram, uint32_t offset, uint32_t val);
void VRAM_WriteShort(VRAM* ram, uint32_t offset, uint16_t val);
void VRAM_WriteByte(VRAM* ram, uint32_t offset, uint8_t val);

uint32_t VRAM_ReadInt(const VRAM* ram, uint32_t offset);
uint16_t VRAM_ReadShort(const VRAM* ram, uint32_t offset);
uint8_t VRAM_ReadByte(const VRAM* ram, uint32_t offset);



#endif
