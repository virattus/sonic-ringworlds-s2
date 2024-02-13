#include <gfx/vdp1/vdp1_env.h>

#include <gfx/vdp1/vdp1.h>
#include <gfx/vdp2/vdp2.h>

#include <assert.h>
#include <stdio.h>


static vdp1_env_t defaultVDP1_env = {
	.bpp = VDP1_ENV_BPP_16,
	.rotation = VDP1_ENV_ROTATION_0,
	.color_mode = VDP1_ENV_COLOR_MODE_RGB_PALETTE,
	.erase_color = 0,
	.erase_points[0] = { 0, 0 },
	.erase_points[1] = { 0, 0 },
};


static vdp1_env_t currentVDP1_env;


static inline void _env_current_update(const vdp1_env_t* env)
{
	assert(env != NULL);
	
	currentVDP1_env = *env;
}


static void _env_default_erase_update(void)
{
	defaultVDP1_env.erase_points[1].x = _state_vdp2()->tv.resolution.x;
	defaultVDP1_env.erase_points[1].y = _state_vdp2()->tv.resolution.y;		
}


void __vdp1_env_init(void)
{
	_state_vdp1()->current_env = &currentVDP1_env;
	
	_env_current_update(&defaultVDP1_env);
	
	vdp1_env_stop();
	vdp1_env_default_set();
}


void vdp1_env_default_init(vdp1_env_t* env)
{
	assert(env != NULL);
	
	_env_default_erase_update();
	
	*env = defaultVDP1_env;
}


const vdp1_env_t* vdp1_env_get(void)
{
	return &currentVDP1_env;
}


void vdp1_env_default_set(void)
{
	_env_default_erase_update();
	
	vdp1_env_set(&defaultVDP1_env);
}


void vdp1_env_set(const vdp1_env_t* env)
{
	assert(env != NULL);
	
	
	currentVDP1_env = *env;		
}


void vdp1_env_stop(void)
{
	
}


void vdp1_env_preamble_populate(vdp1_cmdt_t* cmdts, const vdp1_env_coords_t* env_coords)
{
	assert(cmdts != NULL);
	
	vdp1_cmdt_t* cmdt;
	cmdt = cmdts;
	
	vdp1_cmdt_system_clip_coord_set(cmdt);
	vdp1_cmdt_vtx_system_clip_coord_set(cmdt, env_coords->system_clip);
	cmdt++;
	
	vdp1_cmdt_user_clip_coord_set(cmdt);
	vdp1_cmdt_vtx_user_clip_coord_set(cmdt, env_coords->user_clip_ul, env_coords->user_clip_lr);
	cmdt++;
	
	vdp1_cmdt_local_coord_set(cmdt);
	vdp1_cmdt_vtx_local_coord_set(cmdt, env_coords->local);
	cmdt++;
	
	vdp1_cmdt_end_set(cmdt);
}
