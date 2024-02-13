#include <gfx/vdp2/vdp2_tilemap.h>


#include <gfx/vdp2/vdp2.h>

#include <assert.h>
#include <stdio.h>


vdp2_tilemap_t test_tilemap;


uint32_t VDP2_Tilemap_GetPixel(const vdp2_tilemap_t* tilemap, int16_vec2_t offset)
{
	const int16_vec2_t tilemapOffset = 
	{
		.x = offset.x >> 3,
		.y = offset.y >> 3,
	};
		
	const int16_vec2_t pixelOffset = 
	{
		.x = offset.x & 0x7,
		.y = offset.y & 0x7,
	};
	
	
	//printf("Tilemap GetPixel: X: %d Y: %d >> X %d Y %d >> PixX %d PixY %d\n", offset.x, offset.y, tilemapOffset.x, tilemapOffset.y, pixelOffset.x, pixelOffset.y);
	
	const uint8_t tile = tilemap->TileMap[tilemapOffset.x][tilemapOffset.y];
	
	uint16_t tileoffset = tilemap->TileOffset[tile];
	
	uint32_t* pixels = (uint32_t*)VDP2_GetTextureBuffer()->Memory;
	
	return pixels[tileoffset + (pixelOffset.y * 8) + pixelOffset.x];
}
