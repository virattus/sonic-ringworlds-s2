#include <gfx/render/render_mesh.h>

#include <gamemath/vector.h>
#include <gamemath/fix16_vector.h>
#include <gamemath/int_vector.h>
#include <mic3d/state.h>
#include <gfx/clip.h>
#include <gfx/vdp1/vdp1.h>
#include <gfx/vdp1/vdp1_displaylist.h>


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


#define POLYGON_COUNT	2048
#define SCREEN_POINTS_SIZE	POLYGON_COUNT * 4


int16_vec3_t screen_points_pool[SCREEN_POINTS_SIZE];


//put this into a better place for dynamic resolutions
RECT clipRect = {
	.min = { -160, -112 },
	.max = { 159, 111 },
};



int32_t _depth_normalize(fix16_t z)
{
	render_t* const render = __state.render;
	
	const fix16_t scaled_z = fix16_mul(render->depth_scale, z) + render->depth_offset;

	int32_t retval = fix16_to_int(fix16_mul(render->sort_scale, scaled_z));
	
	/*
	printf("depth scale: %f depth offset %f z %f scaledZ %f sort scale %f retval %d\n", 
		fix16_to_float(render->depth_scale), 
		fix16_to_float(render->depth_offset), 
		fix16_to_float(z), 
		fix16_to_float(scaled_z), 
		fix16_to_float(render->sort_scale), 
		retval); 
	*/
	
	return retval;
}


static int16_t _depth_min_calculate(const int16_vec3_t* points)
{
	return min(min(points[0].z, points[1].z), min(points[2].z, points[3].z));
}


static int16_t _depth_max_calculate(const int16_vec3_t* points)
{
	return max(max(points[0].z, points[1].z), max(points[2].z, points[3].z));
}


static int16_t _depth_centre_calculate(const int16_vec3_t* points)
{
	return ((points[0].z + points[2].z) >> 1);
}


static int32_t _backface_cull_test(const int16_vec3_t* screen_points)
{		
	const int16_vec2_t a = {
		.x = screen_points[2].x - screen_points[0].x,
		.y = screen_points[2].y - screen_points[0].y,
	};
	
	const int16_vec2_t b = {
		.x = screen_points[1].x - screen_points[0].x,
		.y = screen_points[1].y - screen_points[0].y,
	};
	
	const int16_t z = (a.x * b.y) - (a.y * b.x);
	
	return (z <= 0);
}


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




void _perspective_transform(void)
{	
	render_t* const render = __state.render;
	pipeline_t* const pipeline = render->pipeline;
	
	const fix16_mat43_t* world_matrix = render->world_matrix;
	
	char buffer[1024];
	//fix16_mat43_str(world_matrix, buffer, 1);
	//printf("World Matrix:\n%s", buffer);
	
	fix16_mat43_t* const view_matrix = render->view_matrix;
	fix16_mat43_t* const camera_matrix = render->camera_matrix;
	
	
	fix16_mat43_str(camera_matrix, buffer, 1);
	//printf("Camera Matrix:\n%s\n", buffer);
	
	fix16_mat43_mul(camera_matrix, world_matrix, view_matrix);
	
	fix16_mat43_str(view_matrix, buffer, 1);
	//printf("Transformed View Matrix:\n%s\n", buffer);
	
	const fix16_vec3_t* const m0 = &view_matrix->rotation.row[0];
	const fix16_vec3_t* const m1 = &view_matrix->rotation.row[1];
	const fix16_vec3_t* const m2 = &view_matrix->rotation.row[2];
	
	fix16_vec3_str(m0, buffer, 1);
	//printf("M0 %s\n", buffer);

	fix16_vec3_str(m1, buffer, 1);
	//printf("M1 %s\n", buffer);

	fix16_vec3_str(m2, buffer, 1);
	//printf("M2 %s\n", buffer);
	
	const fix16_vec3_t* const points = render->mesh->points;
	
	//printf("number of points to transform: %d\n", render->mesh->points_count);
	for(int i = 0; i < render->mesh->points_count; i++)
	{

		fix16_vec3_str(&points[i], buffer, 1);
		//printf("\nVertex Pre Transform:\n%s", buffer);

		
		fix16_vec3_t p = {
			.x = fix16_vec3_dot(m0, &points[i]) + view_matrix->translation.x,
			.y = fix16_vec3_dot(m1, &points[i]) + view_matrix->translation.y,
			.z = fix16_vec3_dot(m2, &points[i]) + view_matrix->translation.z,
		};
		
		fix16_vec3_str(&p, buffer, 1);
		//printf("\nVertex MID Transform:\n%s", buffer);
		
		const fix16_t depth_value = fix16_div(render->view_distance, p.z);
		//printf(" view distance: %d(%d) ", fix16_to_int(render->view_distance), render->view_distance);
		
		//printf("  Depth Value: %d(%d)\n", fix16_to_int(depth_value), depth_value);
		
		screen_points_pool[i].x = fix16_to_int(fix16_mul(  (depth_value), p.x));
		screen_points_pool[i].y = fix16_to_int(fix16_mul( -(depth_value), p.y));
		screen_points_pool[i].z = _depth_normalize(p.z);
		
		int16_vec3_str(&screen_points_pool[i], buffer, 1);
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
	int16_vec3_t* const screen_points = render->screen_points_pool;
	
	//good news, for this one there's no custom assembly
	for(uint32_t i = 0; i < render->mesh->points_count; i++)
	{
		fix16_vec3_t p;
		fix16_mat43_pos3_mul(view_matrix, &points[i], &p);
		
		screen_points_pool[i].x = fix16_high_mul(render->ortho_size, p.x);
		screen_points_pool[i].y = fix16_high_mul(render->ortho_size, p.y);
		screen_points_pool[i].z = _depth_normalize(p.z);
	}
}



void render_mesh_xform(mesh_t* mesh, const fix16_mat43_t* world_mat)
{	
	assert(mesh != NULL);
	assert(world_mat != NULL);
	
	render_t* render = __state.render;
	pipeline_t* pipeline = render->pipeline;
	
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

		int16_vec3_t screen_points[4] = {
			screen_points_pool[polygon.indices.p0],
			screen_points_pool[polygon.indices.p1],
			screen_points_pool[polygon.indices.p2],
			screen_points_pool[polygon.indices.p3],
		};
		
		
		if(polygon.flags.plane_type != PLANE_TYPE_DOUBLE)
		{
			if(_backface_cull_test(screen_points)) 
				continue; 
		}
		

		
        const int16_t min_depth_z = _depth_min_calculate(screen_points);
		const int16_t max_depth_z = _depth_max_calculate(screen_points);
		
		//printf("minZ %d maxZ %d\n", min_depth_z, max_depth_z);
		
		
		if(min_depth_z < 0) 
			continue;		
		
		if(max_depth_z > render->sort_scale) 
			continue;
		
		
		if(QuadVisible(screen_points, &clipRect)) 
			continue;
		
		
		attribute_t attr = render->mesh->attributes[i];
		
		//remember all the trouble with flipping the UVs for sprite orientation?
		//Is it because you never reset the UVs after flipping them?
		PRIMITIVE drawComPrim = {
			.zSort = _depth_centre_calculate(screen_points),
			.flags = { 
				.draw_mesh = false, 
				.draw_paletted = false, 
				.highSpeedShrink = false, 
				.textureID = 0,
				.use_texture = false,
			},
			.v[0] = { { screen_points[0].x, screen_points[0].y }, { 0, 0 }, { 255, 255, 255 } },
			.v[1] = { { screen_points[1].x, screen_points[1].y }, { 255, 0 }, { 255, 255, 255 } }, 
			.v[2] = { { screen_points[2].x, screen_points[2].y }, { 255, 255 }, { 255, 255, 255 } },
			.v[3] = { { screen_points[3].x, screen_points[3].y }, { 0, 255 }, { 255, 255, 255 } },
		};


		if(polygon.flags.use_texture)
		{
			//uint16_t* attrbytes = (uint16_t*)(&attr);
			//printf("Attribute contents: %d %d %d %d %d %d\n", attrbytes[0], attrbytes[1], attrbytes[2], attrbytes[3], attrbytes[4], attrbytes[5]);
			

			uint16_t read_direction = (attr.control.raw & 0x0030) >> 4;
			
			assert(attr.texture_slot < __state.tlist->count);
			
			texture_t* tex = &__state.tlist->textures[attr.texture_slot];
			
			//printf("tex %d size %d %d\n", attr.texture_slot, tex->size.x, tex->size.y);
			int16_vec2_t texSize = TEXTURE_SIZE_TO_VEC(tex->size);
			assert(texSize.x < 512); //TODO remember to modify this to support arbitrary vram
			assert(texSize.y < 512);
			
			drawComPrim.flags.use_texture = true;
			drawComPrim.flags.textureID = attr.texture_slot;
			drawComPrim.v[0].uv = (int16_vec2_t){ 0, texSize.y };
			drawComPrim.v[1].uv = (int16_vec2_t){ texSize.x, texSize.y };
			drawComPrim.v[2].uv = (int16_vec2_t){ texSize.x, 0 };
			drawComPrim.v[3].uv = (int16_vec2_t){ 0, 0 };

			UpdateUVs(&drawComPrim, read_direction);
		}
		
		DisplayList_VDP1_AddPrimitive(&drawComPrim);
		polycount++;
	}	
}




