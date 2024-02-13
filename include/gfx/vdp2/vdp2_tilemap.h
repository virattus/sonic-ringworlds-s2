#ifndef __FAKEYAUL_VDP2_TILEMAP_H__
#define __FAKEYAUL_VDP2_TILEMAP_H__


#include <gamemath/int_vector.h>

#include <stdint.h>


#define TILEMAP_TILE_COUNT		256

#define TILEMAP_DIMENSION		256


typedef struct VDP2_TILEMAP
{
	uint16_t TileOffset[TILEMAP_TILE_COUNT];
	
	uint8_t TileMap[TILEMAP_DIMENSION][TILEMAP_DIMENSION];
	
	int16_vec2_t TileMapScroll;
	
} vdp2_tilemap_t;


extern vdp2_tilemap_t test_tilemap;


uint32_t VDP2_Tilemap_GetPixel(const vdp2_tilemap_t* tilemap, int16_vec2_t offset);




#endif
