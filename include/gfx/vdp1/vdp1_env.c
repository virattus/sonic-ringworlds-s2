#include <gfx/vdp1/vdp1_env.h>


vdp1_env_t defaultVDP1_env = {
	.erase_points[0] = { 0, 319 },
	.erase_points[1] = { 0, 223 },
};



vdp1_env_t ActiveVDP1_env;




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
