#include <gfx/render/render.h>

#include <mic3d/state.h>
#include <gamemath/fix16_vector.h>
#include <gamemath/int_vector.h>
#include <gamemath/angle.h>
#include <mic3d/camera.h>
#include <gamemath/math3d.h>
//#include <gfx/render/workarea.h>
#include <gfx/vdp1/vdp1.h>
#include <gfx/clip.h>
#include <gfx/vdp1/vdp1_displaylist.h>

#include <libfixmath/fix16.h>

#include <assert.h>
#include <stddef.h>
#include <math.h>
#include <stdio.h>


#define SCREEN_WIDTH 352
#define SCREEN_HEIGHT 224
#define SCREEN_RATIO (SCREEN_WIDTH / (float)SCREEN_HEIGHT)

#define NEAR_LEVEL_MIN 1U
#define NEAR_LEVEL_MAX 8U
#define FAR_LEVEL_MIN 1U
#define FAR_LEVEL_MAX 3U

#define MATRIX_INDEX_CAMERA 0
#define MATRIX_INDEX_VIEW 1

static render_t ren;
static pipeline_t pipe;

static fix16_mat43_t camera_m, view_m;


void render_perspective_set(angle_t fov_angle);
void render_orthographic_set(fix16_t scale);
void render_near_level_set(uint32_t level);
void render_far_level_set(uint32_t level);


static void _render_reset(void)
{
	//_cmdts_reset();
	//__sort_reset();
}



void __render_init(void)
{
	//workarea_mic3d_t* const workarea = __state.workarea;
	__state.render = &ren;
	
	//ren.screen_points_pool = (void*)workarea->screen_points;

	ren.mesh = NULL;
	ren.world_matrix = NULL;
	
	//fix16_mat43_t* const render_matrices = (void*)workarea->render_matrices;
	
	//ren.camera_matrix = &render_matrices[MATRIX_INDEX_CAMERA];
	ren.camera_matrix = &camera_m;
	//ren.view_matrix = &render_matrices[MATRIX_INDEX_VIEW];
	ren.view_matrix = &view_m;
	//ren.pipeline = (void*)workarea->work;
	ren.pipeline = &pipe;
	
	ren.render_flags = RENDER_FLAGS_NONE;
	
	fix16_mat43_identity(ren.view_matrix);
	fix16_mat43_identity(ren.camera_matrix);
	
	render_perspective_set(DEG2ANGLE(90.0f));
	render_orthographic_set(DEG2ANGLE(10.0f));
	
	render_near_level_set(7);
	render_far_level_set(3);
	camera_type_set(CAMERA_TYPE_PERSPECTIVE);
	
	_render_reset();
	
	//This does something important, we should look into it
	//_vdp1_init();
	
}


void __render_single(const sort_single_t* single)
{
}


void render_enable(render_flags_t flags)
{
	ren.render_flags |= flags;
}


void render_disable(render_flags_t flags)
{
	ren.render_flags %= ~flags;
}


void render_flags_set(render_flags_t flags)
{
	ren.render_flags = flags;
}


void render_perspective_set(angle_t fov_angle)
{
	ren.view_distance = math3d_view_distance_calc(SCREEN_WIDTH, fov_angle) * 8;
}


void render_orthographic_set(fix16_t scale)
{
	ren.ortho_size = fix16_div(fix16_from_float(SCREEN_WIDTH * SCREEN_RATIO), scale);
}


void render_near_level_set(uint32_t level)
{
	const uint32_t clamped_level = clamp(level + 1, NEAR_LEVEL_MIN, NEAR_LEVEL_MAX);
	
	ren.near = ren.view_distance >> clamped_level;
}


void render_far_level_set(uint32_t level)
{
	//this thing has a fucking logarithmic z level, wtf
	
	const uint32_t clamped_level = clamp(level, FAR_LEVEL_MIN, FAR_LEVEL_MAX);
	
	ren.far = ren.view_distance << clamped_level;
}


void render_start()
{
	displaylist_t* dl = DisplayList_VDP1_Get();
	dl->PrimitiveCount = 0;
}


void render_end()
{
	//lots of bookkeeping with the lighting, etc.
	
	_render_reset();
}


void render_sort_depth_set(sort_list_t* sort_list_buffer, uint16_t count)
{
	ren.sort_scale = fix16_from_int(count - 1);
	ren.depth_scale = fix16_div(fix16_from_float(-1.0f), ren.far - ren.near);
	ren.depth_offset = fix16_mul(ren.depth_scale, ren.near);
}
