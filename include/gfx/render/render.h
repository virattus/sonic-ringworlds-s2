#ifndef __FAKEYAUL_RENDER_H__
#define __FAKEYAUL_RENDER_H__


#include <core/types.h>
#include <mic3d/camera.h>
#include <mic3d/sort.h>
#include <gamemath/matrix.h>
#include <gfx/texture.h>
#include <gfx/mesh.h>


typedef enum _RENDER_FLAGS
{
	RENDER_FLAGS_NONE = 0,
	RENDER_FLAGS_LIGHTING = 1 << 0,
	RENDER_FLAGS_NO_CLEAR = 1 << 1,
	RENDER_FLAGS_ALL = RENDER_FLAGS_LIGHTING | RENDER_FLAGS_NO_CLEAR,
} render_flags_t;


//this is what became of the render_transform
typedef struct _PIPELINE
{
	attribute_t attribute;
	polygon_t polygon;
	int16_vec3_t screen_points[4];
	//bunch of clip flags

} pipeline_t;


typedef struct _RENDER
{
	//we're separating these into structs now
	struct { //pools
		int16_vec3_t* screen_points_pool;
		//vdp1_cmdt_t* cmdts_pool;
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
		//vrp1_cmdt_t* cmdts;
	};
	
	struct { //matrices
		fix16_mat43_t* camera_matrix;
		fix16_mat43_t* view_matrix;
	};
	
	//sorting struct that we're not using yet
} render_t;



void __render_init(void);

void __render_single(const sort_single_t* single);

void render_start();
void render_end();

void render_enable(render_flags_t flags);
void render_disable(render_flags_t flags);

void render_sort_depth_set(sort_list_t* sort_list_buffer, uint16_t count);

#endif
