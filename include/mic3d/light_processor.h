#ifndef __FAKEYAUL_MIC3D_LIGHT_PROCESSOR_H__
#define __FAKEYAUL_MIC3D_LIGHT_PROCESSOR_H__


#include <mic3d/state.h>

#include <gamemath/mat33.h>
#include <gfx/vdp1/vdp1_vram.h>

#include <stdint.h>


#define LIGHT_COUNT	(3)


typedef void (*light_polygon_processor_t)(void);


typedef struct LIGHT_NODE
{
	//pools
	struct 
	{
		rgb1555_t* colors_pool;
	};
	
	vdp1_gouraud_table_t* gouraud_tables;
	uint32_t count;
	void* vram_base;
	uint16_t slot_base;
	
	//matrices
	struct
	{
		fix16_mat33_t* inv_world_matrix;
		fix16_mat33_t* world_light_matrix;
		//Light matrix represents the direction of each light
		fix16_mat33_t* light_matrix;
		//colour matrix represents each light colour (column vectors)
		fix16_mat33_t* color_matrix;
		fix16_mat33_t* intensity_matrix; //presumably the strength?
	};
	
	//number of lights
	uint32_t light_count;
	//count of used GSTs
	uint32_t gst_count;
	
} light_t;


static inline uint16_t __light_shading_slot_calculate(gst_slot_t gst_slot)
{
	light_t* const light = __state.light;
	
	return (light->slot_base + gst_slot);
}


static inline gst_slot_t __light_gst_alloc(void)
{
	light_t* const light = __state.light;
	
	const gst_slot_t gst_slot = light->gst_count++;
	
	return gst_slot;
}


static inline vdp1_gouraud_table_t* __light_gst_get(gst_slot_t gst_slot)
{
	light_t* const light = __state.light;
	
	return &light->gouraud_tables[gst_slot];
}


void __light_init(void);
void __light_transform(light_polygon_processor_t* processor);
void __light_gst_put(void);


#endif
