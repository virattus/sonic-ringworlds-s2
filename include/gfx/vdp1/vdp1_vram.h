#ifndef __FAKEYAUL_VDP1_VRAM_H__
#define __FAKEYAUL_VDP1_VRAM_H__


#include <core/colour.h>
#include <gfx/vdp1/vdp1_cmdt.h>

#include <stdint.h>


typedef uintptr_t vdp1_vram_t;


typedef struct _vdp1_gouraud_table
{
	rgb1555_t colors[4];
	
} vdp1_gouraud_table_t;


typedef struct _vdp1_clut
{
} vdp1_clut_t;


typedef struct _vdp1_vram_partitions
{
	vdp1_cmdt_t *cmdt_base;
	uint32_t cmdt_size;

	void *texture_base;
	uint32_t texture_size;

	vdp1_gouraud_table_t *gouraud_base;
	uint32_t gouraud_size;

	vdp1_clut_t *clut_base;
	uint32_t clut_size;

	vdp1_vram_t *remaining_base;
	uint32_t remaining_size;
} vdp1_vram_partitions_t;



void vdp1_vram_partitions_get(vdp1_vram_partitions_t* partition);
void vdp1_vram_partitions_set(uint32_t cmdt_count, uint32_t texture_size, uint32_t gouraud_count, uint32_t clut_count);


#endif
