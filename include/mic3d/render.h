#ifndef __FAKEYAUL_MIC3D_RENDER_H__
#define __FAKEYAUL_MIC3D_RENDER_H__


#include <core/types.h>
#include <mic3d/camera.h>
#include <mic3d/sort.h>
#include <gamemath/matrix.h>
#include <mic3d/mesh.h>
#include <mic3d/gst.h>
#include <mic3d/sort.h>


//no fucking clue what this'll be for
#define RENDER_FLAG_TEST(x)		((__state.render->render_flags & __CONCAT(RENDER_FLAGS_, x)) == __CONCAT(RENDER_FLAGS_, x))


typedef enum CLIP_BITMAP
{
	CLIP_BIT_LEFT = 0,
	CLIP_BIT_RIGHT = 1,
	CLIP_BIT_TOP = 2,
	CLIP_BIT_BOTTOM = 3
} clip_bitmap_t;


typedef enum CLIP_FLAGS
{
	CLIP_FLAGS_NONE = 0,
	CLIP_FLAGS_LEFT = 1 << CLIP_BIT_LEFT,
	CLIP_FLAGS_RIGHT = 1 << CLIP_BIT_RIGHT,
	CLIP_FLAGS_TOP = 1 << CLIP_BIT_TOP,
	CLIP_FLAGS_BOTTOM = 1 << CLIP_BIT_BOTTOM,
	
	CLIP_FLAGS_LR = CLIP_FLAGS_LEFT | CLIP_FLAGS_RIGHT,
	CLIP_FLAGS_TB = CLIP_FLAGS_TOP | CLIP_FLAGS_BOTTOM,
	
} clip_flags_t;


//this is what became of the render_transform
typedef struct _PIPELINE
{
	attribute_t attribute;
	polygon_t polygon;
	int16_vec2_t screen_points[4];
	int16_t z_values[4];
	clip_flags_t clip_flags[4];
	clip_flags_t and_flags;
	clip_flags_t or_flags;

} pipeline_t;


typedef struct _RENDER
{
	//we're separating these into structs now
	struct { //pools
		int16_vec2_t* screen_points_pool;
		int16_t* z_values_pool;
		fix16_t* depth_values_pool;
		vdp1_cmdt_t* cmdts_pool;
	};
	
	struct { //settings
		camera_type_t camera_type;
		fix16_t view_distance;
		fix16_t near;
		fix16_t far;
		fix16_t ortho_size;
		int32_t sort_scale;
		fix16_t depth_scale;
		fix16_t depth_offset;	
	};
	
	struct { //rendering
		const mesh_t* mesh;
		const fix16_mat43_t* world_matrix;
		pipeline_t* pipeline;
		render_flags_t render_flags;
		vdp1_cmdt_t* cmdts;
	};
	
	struct { //matrices
		fix16_mat43_t* camera_matrix;
		fix16_mat43_t* view_matrix;
	};
	
	struct { //Sorting
		vdp1_cmdt_t* sort_cmdt;
		vdp1_link_t sort_link;
	};
} render_t;



void __render_init(void);

void __render_single(const sort_single_t* single);

void render_start();
void render_end();

void render_enable(render_flags_t flags);
void render_disable(render_flags_t flags);

void render_sort_depth_set(sort_list_t* sort_list_buffer, uint16_t count);

void _screen_points_swap(int16_vec2_t* screen_points, uint32_t i, uint32_t j);

#endif
