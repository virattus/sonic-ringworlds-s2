#include <gfx/vdp1/vdp1_env.h>

#include <gfx/vdp2/vdp2.h>


static vdp1_env_t defaultVDP1_env = {
	.bpp = VDP1_ENV_BPP_16,
	.rotation = VDP1_ENV_ROTATION_0,
	.color_mode = VDP1_ENV_COLOR_MODE_RGB_PALETTE,
	.erase_color = 0,
	.erase_points[0] = { 0, 0 },
	.erase_points[1] = { 319, 223 },
};



static vdp1_env_t ActiveVDP1_env;


void _env_default_erase_update(void)
{
	defaultVDP1_env.erase_points[1].x = _state_vdp2()->tv.resolution.x;
	defaultVDP1_env.erase_points[1].y = _state_vdp2()->tv.resolution.y;
}


void vdp1_env_default_init(vdp1_env_t* env)
{
	*env = defaultVDP1_env;
}


vdp1_env_t* vdp1_env_get(void)
{
	return &ActiveVDP1_env;
}


void vdp1_env_default_set(void)
{
	ActiveVDP1_env = defaultVDP1_env;
}


void vdp1_env_set(const vdp1_env_t* env)
{
	ActiveVDP1_env = *env;
}
