#include <gfx/vdp1/vdp1_vram.h>


#include <gfx/vdp1/vdp1.h>
#include <gfx/vdp1/vdp1_cmdt.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>


void vdp1_vram_partitions_get(vdp1_vram_partitions_t* partition)
{
	(void)memcpy(partition, &_state_vdp1()->vram_partitions, sizeof(vdp1_vram_partitions_t));
}


void vdp1_vram_partitions_set(uint32_t cmdt_count, uint32_t texture_size, uint32_t gouraud_count, uint32_t clut_count)
{	
	assert(cmdt_count > 0); //must be able to fit at least one quad
	
	const int32_t vram_size = VDP1_VRAM_SIZE - sizeof(vdp1_cmdt_t);
	
	int32_t cmdt_size = cmdt_count * sizeof(vdp1_cmdt_t);
	
	int32_t gouraud_size = gouraud_count * sizeof(vdp1_gouraud_table_t);
	
	int32_t clut_size = clut_count * sizeof(vdp1_clut_t);
	
	int32_t total_size = cmdt_size + texture_size + gouraud_size + clut_size;
	
	int32_t remaining_size = vram_size - total_size;
	
	printf("VDP1 VRAM: %d CMDT: %d TEXTURE: %d GOURAUD: %d CLUT: %d REMAINING: %d\n", 
		vram_size,
		cmdt_size,
		texture_size,
		gouraud_size,
		clut_size,
		remaining_size);
	
	assert(remaining_size >= 0);

	//It looks like the first cmdt should never be written to?
	//Find out what's with this
	uint32_t vram_offset = sizeof(vdp1_cmdt_t);
	
	vdp1_vram_partitions_t* vram_partitions = &_state_vdp1()->vram_partitions;
	
	vram_partitions->cmdt_base = (vdp1_cmdt_t*)(VDP1_VRAM(vram_offset));
	vram_partitions->cmdt_size = cmdt_size;
	vram_offset += cmdt_size;
	
	vram_partitions->texture_base = (void*)(VDP1_VRAM(vram_offset));
	vram_partitions->texture_size = texture_size;
	vram_offset += texture_size;
	
	vram_partitions->gouraud_base = (vdp1_gouraud_table_t*)(VDP1_VRAM(vram_offset));
	vram_partitions->gouraud_size = gouraud_size;
	vram_offset += gouraud_size;
	
	vram_partitions->clut_base = (vdp1_clut_t*)(VDP1_VRAM(vram_offset));
	vram_partitions->clut_size = clut_size;
	vram_offset += clut_size;
	
	if(remaining_size == 0)
	{
		vram_partitions->remaining_base = NULL;
	}
	else
	{
		vram_partitions->remaining_base = VDP1_VRAM(vram_offset);
	}
	
	vram_partitions->remaining_size = remaining_size;
	
}



void* VDP1_VRAM(uint32_t offset)
{
	return VDP1_GetTextureBuffer()->Memory + offset;
}
