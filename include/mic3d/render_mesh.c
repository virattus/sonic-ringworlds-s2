#include <mic3d/render_mesh.h>

#include <gamemath/vector.h>
#include <gamemath/fix16_vector.h>
#include <gamemath/int_vector.h>
#include <mic3d/types.h>
#include <mic3d/state.h>
#include <gfx/vdp1/vdp1.h>
#include <gfx/vdp1/vdp1_cmdt.h>
#include <mic3d/light_processor.h>


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


static vdp1_cmdt_t* _cmdts_alloc(void)
{
	
}


static void _cmdt_process(vdp1_cmdt_t* cmdt)
{
	
}


static inline int32_t _depth_normalize(fix16_t z)
{
	render_t* const render = __state.render;
	
	const fix16_t scaled_z = 
		fix16_mul(render->depth_scale, z) + render->depth_offset;

	return fix16_high_mul(render->sort_scale, scaled_z);
	//int32_t retval = fix16_to_int(fix16_mul(render->sort_scale, scaled_z));
	
	/*
	printf("depth scale: %f depth offset %f z %f scaledZ %f sort scale %f retval %d\n", 
		fix16_to_float(render->depth_scale), 
		fix16_to_float(render->depth_offset), 
		fix16_to_float(z), 
		fix16_to_float(scaled_z), 
		fix16_to_float(render->sort_scale), 
		retval); 
	*/
	
	//return retval;
}


static int16_t _depth_min_calculate(const int16_t* z_values)
{
	return MIN(MIN(z_values[0], z_values[1]), MIN(z_values[2], z_values[3]));
}


static int16_t _depth_max_calculate(const int16_t* z_values)
{
	return MAX(MAX(z_values[0], z_values[1]), MAX(z_values[2], z_values[3]));
}


static int16_t _depth_centre_calculate(const int16_t* z_values)
{
	return ((z_values[0] + z_values[2]) >> 1);
}


static bool _pipeline_backface_cull_test(void)
{		
	render_t* const render = __state.render;
	pipeline_t* const pipeline = render->pipeline;
	
	const int32_vec2_t a = {
		.x = pipeline->screen_points[2].x - pipeline->screen_points[0].x,
		.y = pipeline->screen_points[2].y - pipeline->screen_points[0].y,
	};
	
	const int32_vec2_t b = {
		.x = pipeline->screen_points[1].x - pipeline->screen_points[0].x,
		.y = pipeline->screen_points[1].y - pipeline->screen_points[0].y,
	};
	
	const int32_t z = (a.x * b.y) - (a.y * b.x);
	
	return (z <= 0);
}


static void _clip_flags_calculate(const int16_vec2_t* screen_points,
	clip_flags_t* clip_flags, clip_flags_t* and_flags, clip_flags_t* or_flags)
{
	_clip_flags_lrtb_calculate(screen_points[0], &clip_flags[0]);
	_clip_flags_lrtb_calculate(screen_points[1], &clip_flags[1]);
	_clip_flags_lrtb_calculate(screen_points[2], &clip_flags[2]);
	_clip_flags_lrtb_calculate(screen_points[3], &clip_flags[3]);

	*and_flags = clip_flags[0] & clip_flags[1] & clip_flags[2] & clip_flags[3];
	*or_flags = clip_flags[0] | clip_flags[1] | clip_flags[2] | clip_flags[3];
}


static void _indices_swap(uint32_t i, uint32_t j)
{
	render_t* const render = __state.render;
	pipeline_t* const pipeline = render->pipeline;
	
	const uint16_t tmp = pipeline->polygon.indices.p[i];
	
	pipeline->polygon.indices.p[i] = pipeline->polygon.indices.p[i];
	pipeline->polygon.indices.p[j] = tmp;
}


static void _pipeline_polygon_orient(void)
{
	render_t* const render = __state.render;
	pipeline_t* const pipeline = render->pipeline;
	
	//This entire thing is done just for performance reasons, might be able to remove it
	if((pipeline->clip_flags[0] & CLIP_FLAGS_LR) != CLIP_FLAGS_NONE)
	{
		_indices_swap(0, 1);
		_indices_swap(3, 2);
		
		_screen_points_swap(pipeline->screen_points, 0, 1);
		_screen_points_swap(pipeline->screen_points, 2, 3);
		
		pipeline->attribute.control.raw ^= VDP1_CMDT_CHAR_FLIP_H;
	}
	
	if((pipeline->clip_flags[0] & CLIP_FLAGS_TB) != CLIP_FLAGS_NONE)
	{
		_indices_swap(0, 3);
		_indices_swap(1, 2);
		
		_screen_points_swap(pipeline->screen_points, 0, 3);
		_screen_points_swap(pipeline->screen_points, 1, 2);
		
		pipeline->attribute.control.raw ^= VDP1_CMDT_CHAR_FLIP_V;
	}	
}


/*
static void UV_Swap(int16_vec2_t* t0, int16_vec2_t* t1)
{
	int16_vec2_t temp = *t0;
	*t0 = *t1;
	*t1 = temp;
}


static void UpdateUVs(PRIMITIVE* p, read_dir_t dir)
{
	if(dir == READ_DIR_H)
	{
		UV_Swap(&p->v[0].uv, &p->v[1].uv);
		UV_Swap(&p->v[2].uv, &p->v[3].uv);
	}	
	else if(dir == READ_DIR_V)
	{
		UV_Swap(&p->v[0].uv, &p->v[3].uv);
		UV_Swap(&p->v[2].uv, &p->v[1].uv);
	}
	else if(dir == READ_DIR_HV)
	{
		UV_Swap(&p->v[0].uv, &p->v[2].uv);
		UV_Swap(&p->v[3].uv, &p->v[1].uv);
	}
}
*/



void _perspective_transform(void)
{	
	render_t* const render = __state.render;
	pipeline_t* const pipeline = render->pipeline;
	
	const fix16_mat43_t* world_matrix = render->world_matrix;
	
	//char buffer[1024];
	//fix16_mat43_str(world_matrix, buffer, 1);
	//printf("World Matrix:\n%s", buffer);
	
	fix16_mat43_t* const view_matrix = render->view_matrix;
	fix16_mat43_t* const camera_matrix = render->camera_matrix;
	
	
	//fix16_mat43_str(camera_matrix, buffer, 1);
	//printf("Camera Matrix:\n%s\n", buffer);
	
	fix16_mat43_mul(camera_matrix, world_matrix, view_matrix);
	
	//fix16_mat43_str(view_matrix, buffer, 1);
	//printf("Transformed View Matrix:\n%s\n", buffer);
	
	const fix16_vec3_t* const m0 = &view_matrix->rotation.row[0];
	const fix16_vec3_t* const m1 = &view_matrix->rotation.row[1];
	const fix16_vec3_t* const m2 = &view_matrix->rotation.row[2];
	
	/*
	fix16_vec3_str(m0, buffer, 1);
	printf("M0 %s\n", buffer);
	fix16_vec3_str(m1, buffer, 1);
	printf("M1 %s\n", buffer);
	fix16_vec3_str(m2, buffer, 1);
	printf("M2 %s\n", buffer);
	*/
	
	const fix16_vec3_t* const points = render->mesh->points;
	int16_vec2_t* const screen_points = render->screen_points_pool;
	int16_t* const z_values = render->z_values_pool;
	
	//printf("number of points to transform: %d\n", render->mesh->points_count);
	for(uint32_t i = 0; i < render->mesh->points_count; i++)
	{

		//fix16_vec3_str(&points[i], buffer, 1);
		//printf("\nVertex Pre Transform:\n%s", buffer);

		
		fix16_vec3_t p = {
			.x = fix16_vec3_dot(m0, &points[i]) + view_matrix->translation.x,
			.y = fix16_vec3_dot(m1, &points[i]) + view_matrix->translation.y,
			.z = fix16_vec3_dot(m2, &points[i]) + view_matrix->translation.z,
		};
		
		//fix16_vec3_str(&p, buffer, 1);
		//printf("\nVertex MID Transform:\n%s", buffer);
		
		const fix16_t depth_value = fix16_div(render->view_distance, p.z);
		//printf(" view distance: %d(%d) ", fix16_to_int(render->view_distance), render->view_distance);
		
		//printf("  Depth Value: %d(%d)\n", fix16_to_int(depth_value), depth_value);
		
		screen_points[i].x = fix16_to_int(fix16_mul(  (depth_value), p.x));
		screen_points[i].y = fix16_to_int(fix16_mul( -(depth_value), p.y));
		z_values[i] = _depth_normalize(p.z);
		
		//int16_vec3_str(&screen_points_pool[i], buffer, 1);
		//printf("\nVertex post Transform:\n%s\n", buffer);
		
	}
}


void _orthogonal_transform(void)
{
	render_t* const render = __state.render;
	
	const fix16_mat43_t* const world_matrix = render->world_matrix;
	fix16_mat43_t* const view_matrix = render->view_matrix;
	fix16_mat43_t* const camera_matrix = render->camera_matrix;
	
	fix16_mat43_mul(camera_matrix, world_matrix, view_matrix);
	
	const fix16_vec3_t* const points = render->mesh->points;
	int16_vec2_t* const screen_points = render->screen_points_pool;
	int16_t* const z_values = render->z_values_pool;
	
	//good news, for this one there's no custom assembly
	for(uint32_t i = 0; i < render->mesh->points_count; i++)
	{
		fix16_vec3_t p;
		fix16_mat43_pos3_mul(view_matrix, &points[i], &p);
		
		screen_points[i].x = fix16_high_mul(render->ortho_size, p.x);
		screen_points[i].y = fix16_high_mul(-render->ortho_size, p.y);
		z_values[i] = _depth_normalize(p.z);
	}
}



void render_mesh_xform(mesh_t* mesh, const fix16_mat43_t* world_mat)
{	
	assert(mesh != NULL);
	assert(world_mat != NULL);
	
	render_t* const render = __state.render;
	pipeline_t* const pipeline = render->pipeline;
	
	light_polygon_processor_t light_polygon_processor;
	
	render->mesh = mesh;
	render->world_matrix = world_mat;
	
	if(render->camera_type == CAMERA_TYPE_PERSPECTIVE)
	{
		_perspective_transform();
	}
	else if(render->camera_type == CAMERA_TYPE_ORTHOGRAPHIC)
	{
		_orthogonal_transform();
	}
	
	__light_transform(&light_polygon_processor);
	
	
	int16_t* const z_values = render->z_values_pool;
	int16_vec2_t* const screen_points = render->screen_points_pool;
	const polygon_t* const polygons = render->mesh->polygons;
	
	int polycount = 0;
	//printf("number of faces to draw: %d\n", mesh->polygons_count);
	for(uint32_t i = 0; i < mesh->polygons_count; i++)
	//for(int i = 0; i < 3; i++) 
	{		
		polygon_t polygon = polygons[i];
		
		assert(polygon.indices.p0 < mesh->points_count);
		assert(polygon.indices.p1 < mesh->points_count);
		assert(polygon.indices.p2 < mesh->points_count);
		assert(polygon.indices.p3 < mesh->points_count);

		pipeline->screen_points[0] = screen_points[pipeline->polygon.indices.p0];
		pipeline->screen_points[1] = screen_points[pipeline->polygon.indices.p1];
		pipeline->screen_points[2] = screen_points[pipeline->polygon.indices.p2];
		pipeline->screen_points[3] = screen_points[pipeline->polygon.indices.p3];
		
		if(pipeline->polygon.flags.plane_type != PLANE_TYPE_DOUBLE)
		{
			if(_pipeline_backface_cull_test()) 
				continue; 
		}
		
		pipeline->z_values[0] = z_values[pipeline->polygon.indices.p0];
		pipeline->z_values[1] = z_values[pipeline->polygon.indices.p1];
		pipeline->z_values[2] = z_values[pipeline->polygon.indices.p2];
		pipeline->z_values[3] = z_values[pipeline->polygon.indices.p3];
		
        const int16_t min_depth_z = _depth_min_calculate(pipeline->z_values);		
		if(min_depth_z < 0)
		{
			continue;
		}

		const int16_t max_depth_z = _depth_max_calculate(pipeline->z_values);
		if(max_depth_z > render->sort_scale)
		{
			continue;
		}
		
		//printf("minZ %d maxZ %d\n", min_depth_z, max_depth_z);
		
		
		_clip_flags_calculate(pipeline->screen_points, pipeline->clip_flags, &pipeline->and_flags, &pipeline->or_flags);
		if(pipeline->and_flags != CLIP_FLAGS_NONE)
		{
			continue;
		}
		
		int32_t depth_z;
		
		switch(pipeline->polygon.flags.sort_type)
		{
			default:
			case SORT_TYPE_CENTER:
				depth_z = _depth_centre_calculate(pipeline->z_values);
				break;
			case SORT_TYPE_MIN:
				depth_z = min_depth_z;
				break;
			case SORT_TYPE_MAX:
				depth_z = max_depth_z;
				break;
		}
		
		__sort_insert(depth_z);
		
		attribute_t attr = render->mesh->attributes[i];
		
		if(pipeline->or_flags == CLIP_FLAGS_NONE)
		{
			//Yaul disables pre-clipping here. We don't care
			//because we won't ever risk corrupting memory by disabling it
			pipeline->attribute.draw_mode.pre_clipping_disable = true;
		}
		else
		{
			_pipeline_polygon_orient();
		}
		
		light_polygon_processor();
		
		
		vdp1_cmdt_t* const cmdt = _cmdts_alloc();
		
		_cmdt_process(cmdt);
	}	
}




