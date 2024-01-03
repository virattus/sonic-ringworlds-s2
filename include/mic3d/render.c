#include <mic3d/render.h>

#include <core/preprocessor.h>
#include <gamemath/int_vector.h>
#include <gamemath/angle.h>
#include <gamemath/math3d.h>
#include <mic3d/internal.h>
#include <mic3d/sort.h>
#include <mic3d/texture.h>
#include <gfx/vdp1/vdp1.h>

#include <assert.h>
#include <stddef.h>
#include <math.h>
#include <stdio.h>


#define SCREEN_WIDTH	352
#define SCREEN_HEIGHT	224
#define SCREEN_RATIO	(SCREEN_WIDTH / (float)SCREEN_HEIGHT)

#define NEAR_LEVEL_MIN	1U
#define NEAR_LEVEL_MAX	8U
#define FAR_LEVEL_MIN	1U
#define FAR_LEVEL_MAX	3U

#define ORDER_SYSTEM_CLIP_COORDS_INDEX	0
#define ORDER_LOCAL_COORDS_INDEX		1
#define ORDER_CLEAR_POLYGON_INDEX		2
#define ORDER_SUBR_INDEX				2
#define ORDER_DRAW_END_INDEX			3
#define ORDER_INDEX						4

#define SCREEN_CLIP_LEFT	(-SCREEN_WIDTH >> 1)
#define SCREEN_CLIP_RIGHT	(SCREEN_WIDTH >> 1)
#define SCREEN_CLIP_TOP		(-SCREEN_HEIGHT >> 1)
#define SCREEN_CLIP_BOTTOM	(SCREEN_HEIGHT >> 1)

#define MATRIX_INDEX_CAMERA 0
#define MATRIX_INDEX_VIEW 1


void render_perspective_set(angle_t fov_angle);
void render_orthographic_set(fix16_t scale);
void render_near_level_set(uint32_t level);
void render_far_level_set(uint32_t level);


static void _cmdts_reset(void)
{
	render_t* const render = __state.render;
	
	render->cmdts = render->cmdts_pool;
}


static uint32_t _cmdts_count_get(void)
{
	render_t* const render = __state.render;
	
	return (render->cmdts - render->cmdts_pool);
}


static void _render_reset(void)
{
	_cmdts_reset();
	__sort_reset();
}


static void _vdp1_init(void)
{
	static const int16_vec2_t system_clip_coord = 
		INT16_VEC2_INITIALIZER(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);

	static const int16_vec2_t local_coord_centre = 
		INT16_VEC2_INITIALIZER(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		
	static const vdp1_cmdt_draw_mode_t polygon_draw_mode = {
		.pre_clipping_disable = true,
	};
	
	static int16_vec2_t polygon_points[] = {
		INT16_VEC2_INITIALIZER(-SCREEN_WIDTH / 2, 		0),
		INT16_VEC2_INITIALIZER((SCREEN_WIDTH / 2) - 1, 	0),
		INT16_VEC2_INITIALIZER((SCREEN_WIDTH / 2) - 1,	(SCREEN_HEIGHT / 2) - 1),
		INT16_VEC2_INITIALIZER(-SCREEN_WIDTH / 2, 		(SCREEN_HEIGHT / 2 - 1)),
	};

	//this is hardcoded for 352x224, look into for other res
	polygon_points[0].y = 70;
	polygon_points[1].y = 70;
	
	vdp1_cmdt_t* const cmdts = (vdp1_cmdt_t*)VDP1_CMD_TABLE(0, 0);
	
	vdp1_cmdt_system_clip_coord_set(&cmdts[ORDER_SYSTEM_CLIP_COORDS_INDEX]);
	vdp1_cmdt_vtx_system_clip_coord_set(&cmdts[ORDER_SYSTEM_CLIP_COORDS_INDEX], system_clip_coord);
	
	vdp1_cmdt_local_coord_set(&cmdts[ORDER_LOCAL_COORDS_INDEX]);
	vdp1_cmdt_vtx_local_coord_set(&cmdts[ORDER_LOCAL_COORDS_INDEX], local_coord_centre);

	vdp1_cmdt_polygon_set(&cmdts[ORDER_CLEAR_POLYGON_INDEX]);
	vdp1_cmdt_draw_mode_set(&cmdts[ORDER_CLEAR_POLYGON_INDEX], polygon_draw_mode);
	vdp1_cmdt_colour_set(&cmdts[ORDER_CLEAR_POLYGON_INDEX], RGB1555(0, 0, 0, 0));
	vdp1_cmdt_vtx_set(&cmdts[ORDER_CLEAR_POLYGON_INDEX], polygon_points);
	
	vdp1_cmdt_end_set(&cmdts[ORDER_DRAW_END_INDEX]);
}



void __render_init(void)
{
	workarea_mic3d_t* const workarea = __state.workarea;
	render_t* const render = __state.render;
	
	render->z_values_pool = (void*)workarea->z_values;
	render->screen_points_pool = (void*)workarea->screen_points;
	render->depth_values_pool = (void*)workarea->depth_values;
	render->cmdts_pool = (void*)workarea->cmdts;

	render->mesh = NULL;
	render->world_matrix = NULL;
	
	fix16_mat43_t* const render_matrices = (void*)workarea->render_matrices;
	
	render->camera_matrix = &render_matrices[MATRIX_INDEX_CAMERA];
	render->view_matrix = &render_matrices[MATRIX_INDEX_VIEW];
	render->pipeline = (void*)workarea->work;
	
	render->render_flags = RENDER_FLAGS_NONE;
	
	fix16_mat43_identity(render->view_matrix);
	fix16_mat43_identity(render->camera_matrix);
	
	render_perspective_set(DEG2ANGLE(90.0f));
	render_orthographic_set(DEG2ANGLE(10.0f));
	
	render_near_level_set(7);
	render_far_level_set(3);
	camera_type_set(CAMERA_TYPE_PERSPECTIVE);
	
	_render_reset();
	
	//This does something important, we should look into it
	_vdp1_init();
	
}


void __render_single(const sort_single_t* single)
{
	render_t* const render = __state.render;
	sort_t* const sort = __state.sort;
	
	const vdp1_link_t link = single - (sort->singles_pool + 1);
	
	vdp1_cmdt_link_set(render->sort_cmdt, link + render->sort_link);
	
	render->sort_cmdt = &render->cmdts_pool[link];
}


void render_enable(render_flags_t flags)
{
	render_t* const render = __state.render;
	
	render->render_flags |= flags;
}


void render_disable(render_flags_t flags)
{
	render_t* const render = __state.render;
	
	render->render_flags &= ~flags;
}


void render_flags_set(render_flags_t flags)
{
	render_t* const render = __state.render;
	
	render->render_flags = flags;
}


void render_perspective_set(angle_t fov_angle)
{
	render_t* const render = __state.render;
	
	render->view_distance = math3d_view_distance_calc(SCREEN_WIDTH, fov_angle);
}


void render_orthographic_set(fix16_t scale)
{
	render_t* const render = __state.render;
	
	render->ortho_size = fix16_div(fix16_from_float(SCREEN_WIDTH * SCREEN_RATIO), scale);
}


void render_near_level_set(uint32_t level)
{
	render_t* const render = __state.render;
	
	const uint32_t clamped_level = CLAMP(level + 1, NEAR_LEVEL_MIN, NEAR_LEVEL_MAX);
	
	uint32_t near_value;
	near_value = render->view_distance;
	
	for(int32_t i = clamped_level; i > 0; i--)
	{
		near_value >>= 1;
	}
	
	render->near = near_value;
}


void render_far_level_set(uint32_t level)
{
	//this thing has a fucking logarithmic z level, wtf
	
	render_t* const render = __state.render;
	
	const uint32_t clamped_level = CLAMP(level, FAR_LEVEL_MIN - 1, FAR_LEVEL_MAX);
	
	uint32_t far_value;
	far_value = render->view_distance;
	
	for(int32_t i = clamped_level; i > 0; i--)
	{
		far_value <<= 1;
	}
	
	render->far = far_value;
}


void render_start()
{

}


void render_end()
{
	//lots of bookkeeping with the lighting, etc.
	render_t* const render = __state.render;
	
	vdp1_cmdt_t* const subr_cmdt = (vdp1_cmdt_t*)VDP1_CMD_TABLE(ORDER_SUBR_INDEX, 0);
	
	const uint32_t cmdt_count = _cmdts_count_get();
	
	if(cmdt_count == 0)
	{
		vdp1_cmdt_link_type_set(subr_cmdt, VDP1_CMDT_LINK_TYPE_JUMP_NEXT);
		
		vdp1_sync_force_put();
		
		return;
	}
	
	render->sort_cmdt = subr_cmdt;
	render->sort_link = ORDER_INDEX;
	
	
	if(RENDER_FLAG_TEST(NO_CLEAR))
	{
		vdp1_cmdt_link_type_set(subr_cmdt, VDP1_CMDT_LINK_TYPE_SKIP_CALL);
		
		vdp1_sync_mode_set(VDP1_SYNC_MODE_CHANGE_ONLY);
	}
	else
	{
		vdp1_cmdt_link_type_set(subr_cmdt, VDP1_CMDT_LINK_TYPE_JUMP_CALL);
		
		vdp1_sync_mode_set(VDP1_SYNC_MODE_ERASE_CHANGE);
	}
	
	vdp1_cmdt_t* const last_cmdt = render->sort_cmdt;
	
	vdp1_sync_cmdt_put(render->cmdts_pool, cmdt_count, render->sort_link);
	
	__light_gst_put();
	
	_render_reset();
}


void render_sort_depth_set(sort_list_t* sort_list_buffer, uint16_t count)
{
	assert(sort_list_buffer != NULL);
	assert((count & (count - 1)) == 0);
	
	render_t* const render = __state.render;
	sort_t* const sort = __state.sort;
	
	sort->sort_lists_pool = sort_list_buffer;
	
	//copypaste from yaul
    /* To normalize Z [0,1]:            a=-1/(f-n)    The - is to negate the Z-axis
     *                                  b=a*n
     *                       normalized_z=(a*z)+b
     *
     * We also at the same time need to normalize to the sort scale */
	
	
	render->sort_scale = fix16_from_int(count - 1);
	render->depth_scale = fix16_div(fix16_from_float(-1.0f), render->far - render->near);
	render->depth_offset = fix16_mul(render->depth_scale, render->near);
}


void _clip_flags_lrtb_calculate(const int16_vec2_t screen_point, clip_flags_t* clip_flag)
{
	*clip_flag = (screen_point.x < SCREEN_CLIP_LEFT) << CLIP_BIT_LEFT;
	*clip_flag |= (screen_point.x > SCREEN_CLIP_RIGHT) << CLIP_BIT_RIGHT;
	
	*clip_flag |= (screen_point.y < SCREEN_CLIP_TOP) << CLIP_BIT_TOP;
	*clip_flag |= (screen_point.y > SCREEN_CLIP_BOTTOM) << CLIP_BIT_BOTTOM;
}


void _screen_points_swap(int16_vec2_t* screen_points, uint32_t i, uint32_t j)
{
	const int16_vec2_t temp = screen_points[i];
	
	screen_points[i] = screen_points[j];
	screen_points[j] = temp;
}
