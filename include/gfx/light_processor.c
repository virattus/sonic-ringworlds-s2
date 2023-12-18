#include <gfx/light_processor.h>

#include <mic3d/state.h>



#define MATRIX_INDEX_INVERSE_WORLD	0
#define MATRIX_INDEX_WORLD_LIGHT 	1
#define MATRIX_INDEX_LIGHT 			2
#define MATRIX_INDEX_COLOR			3
#define MATRIX_INDEX_INTENSITY		4


uint16_t __light_shading_slot_calculate(gst_slot_t gst_slot)
{
	light_t* const light = __state.light;
	
	return (light->slot_base + gst_slot);
}


gst_slot_t __light_gst_alloc(void)
{
	light_t* const light = __state.light;
	
	const gst_slot_t gst_slot = light->gst_count++;
	
	return gst_slot;
}


vdp1_gouraud_table_t* __light_gst_get(gst_slot_t gst_slot)
{
	light_t* const light = __state.light;
	
	return &light->gouraud_tables[gst_slot];
}



static void _polygon_process(void)
{
}


static void _polygon_passthrough_process(void)
{
}


void __light_init(void)
{
}


static void _world_matrix_invert(void)
{
}


void __light_transform(light_polygon_processor_t* processor)
{
}


void __light_gst_put(void)
{
}


void light_gst_set(vdp1_gouraud_table_t* gouraud_tables, uint32_t count, vdp1_vram_t vram_base)
{
}
