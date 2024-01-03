#include <mic3d/gst.h>

#include <gfx/vram.h>
#include <gfx/vdp1/vdp1.h>

#include <string.h>



gst_slot_t __gst_slot_calculate(gst_slot_t gst_slot)
{
	gst_t* const gst = __state.gst;
	
	return (gst->slot_base + gst_slot);
}



void __gst_init(void)
{
	gst_unset();
}


/* TODO: figure out the correct math for the actual transfer */
void __gst_put(const vdp1_gouraud_table_t* gouraud_tables, vdp1_vram_t vram_base, uint32_t put_count)
{
	//here we actually upload the data
	if(put_count == 0)
	{
		return;
	}
	
	gst_t* const gst = __state.gst;
	
	//copy over gouraud_tables to vram_base position
	
	VRAM* ram = VDP1_GetTextureBuffer();
	
	//figure out exactly how these values should be interpreted
	memcpy(ram->Memory + vram_base, gouraud_tables, put_count); 
}



void gst_set(vdp1_vram_t vram_base)
{
	gst_t* const gst = __state.gst;
	
	gst->vram_base = vram_base;
	gst->slot_base = vram_base >> 3;
}


void gst_unset(void)
{
	gst_set(0x00000000);
}


void gst_put(const vdp1_gouraud_table_t* gouraud_tables, uint32_t put_count)
{
	gst_t* const gst = __state.gst;
	
	__gst_put(gouraud_tables, gst->vram_base, put_count);
}


