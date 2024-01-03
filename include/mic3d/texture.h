#ifndef __FAKEYAUL_MIC3D_TEXTURE_MAPPER_H__
#define __FAKEYAUL_MIC3D_TEXTURE_MAPPER_H__


#include <gamemath/vector.h>
#include <gfx/vram.h>

#include <stdint.h>

/*
 * Texture sizes are as such:
 * 	x must be a multiple of eight, with a max of 31 equaling a max size of 248
 * 	y is a simple range from 1 to 255
 * 
 * x is stored by bit shifting right 3 then shifting left 8
 */


#define TEXTURE_SIZE(w, h)		((uint16_t)((((w) >> 3) << 8) | ((h) & 255)))
#define TEXTURE_SIZE_TO_VEC(x)	(int16_vec2_t){((((x) & 0xFF00) >> 8) << 3), ((x) & 0x00FF)}

#define TEXTURE_VRAM_INDEX(addr)	((uint16_t)((uintptr_t)(addr) >> 3))


typedef struct _palette 
{
    const void* data;
    uint16_t data_size;
    
} palette_t;


typedef struct texture
{
	uint32_t vram_index;
	uint16_t size;
} texture_t;


typedef struct picture
{
	const void* data;
	uint16_t data_size;
	uint16_t width;
	uint16_t height;
	uint16_t palette_index;
} picture_t;


typedef struct texturelist
{
	texture_t* textures;
	uint16_t count;
} tlist_t;


void __tlist_init(void);
const texture_t* tlist_get(void);
void tlist_set(texture_t* textures, uint16_t count);


texture_t* Texture_LoadFromFile(const char* texListFile, VRAM* dest, int32_t* texCount);
texture_t* Texture_LoadFile(const char* filename, VRAM* dest, uint32_t* offset, uint16_t* dimensions);

uint32_t TextureMapper_GetVRAMOffset(const texture_t* tex, const int16_vec2_t* uv);

uint32_t TextureMapper_WriteToVRAM(VRAM* ram, uint32_t offset, int16_t* dimensions, const char* filename);


void TextureMapper_Init();





#endif
