#ifndef __FAKEYAUL_VDP1_VRAM_H__
#define __FAKEYAUL_VDP1_VRAM_H__


#include <core/colour.h>

#include <stdint.h>


#define VDP1_VRAM_DEFAULT_CMDT_COUNT 	(2048)
#define VDP1_VRAM_DEFAULT_TEXTURE_SIZE	(442336) //bytes
#define VDP1_VRAM_DEFAULT_GOURAUD_COUNT (1024)
#define	VDP1_VRAM_DEFAULT_CLUT_COUNT 	(256)


#define VDP1_VRAM_SIZE			524288 //bytes

#define VDP1_CMD_TABLE(x, y)	VDP1_VRAM(((x) << 5) + (((y) << 1) & 0x1F))


typedef uint32_t vdp1_vram_t;

typedef struct VDP1_CMDT vdp1_cmdt_t;


typedef struct VDP1_GOURAUD_TABLE
{
	rgb1555_t colors[4];
	
} vdp1_gouraud_table_t;


typedef struct VDP1_CLUT
{
} vdp1_clut_t;


typedef struct VDP1_VRAM_PARTITIONS
{
	vdp1_cmdt_t* cmdt_base;
	uint32_t cmdt_size;
	
	void* texture_base;
	uint32_t texture_size;
	
	vdp1_gouraud_table_t* gouraud_base;
	uint32_t gouraud_size;
	
	vdp1_clut_t* clut_base;
	uint32_t clut_size;
	
	vdp1_vram_t* remaining_base;
	uint32_t remaining_size;
	
} vdp1_vram_partitions_t;


void vdp1_vram_partitions_get(vdp1_vram_partitions_t* partition);
void vdp1_vram_partitions_set(uint32_t cmdt_count, uint32_t texture_size, uint32_t gouraud_count, uint32_t clut_count);


void* VDP1_VRAM(uint32_t offset);


#endif
