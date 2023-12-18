#ifndef __FAKEYAUL_LIGHT_PROCESSOR_H__
#define __FAKEYAUL_LIGHT_PROCESSOR_H__


#include <core/colour.h>
#include <gamemath/fix16_vector.h>
#include <gamemath/mat33.h>
#include <gamemath/mat43.h>
#include <gfx/vdp1/vdp1_vram.h>
#include <gfx/vdp1/vdp1_cmdt.h>

#include <stdint.h>


#define LIGHT_COUNT	(3)


typedef struct LIGHT_NODE
{
	//pools
	struct 
	{
		rgb1555_t* colors_pool;
	};
	
	vdp1_gouraud_table_t* gouraud_tables;
	uint32_t count;
	vdp1_vram_t vram_base;
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


typedef struct LIGHT_POLYGON_PROCESSOR
{
	
	
} light_polygon_processor_t;


uint16_t __light_shading_slot_calculate(gst_slot_t gst_slot);
gst_slot_t __light_gst_alloc(void);
vdp1_gouraud_table_t* __light_gst_get(gst_slot_t gst_slot);


void __light_init(void);
void __light_transform(light_polygon_processor_t* processor);
void __light_gst_put(void);


#endif
