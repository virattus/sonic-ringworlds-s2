#ifndef __FAKEYAUL_VDP1_ENV_H__
#define __FAKEYAUL_VDP1_ENV_H__


#include <gamemath/vector.h>



typedef enum vdp1_env_bpp
{
	VDP1_ENV_BPP_16 = 0,
	
} vdp1_env_bpp_t;


typedef enum vdp1_env_rotation
{
	VDP1_ENV_ROTATION_0 = 0,
	
} vdp1_env_rotation_t;


typedef enum vdp1_env_color_mode
{
	VDP1_ENV_COLOR_MODE_PALETTE = 0,
	VDP1_ENV_COLOR_MODE_RGB_PALETTE = 1,
	
} vdp1_env_color_mode_t;


typedef struct vdp1_env
{
	vdp1_env_bpp_t bpp;
	vdp1_env_rotation_t rotation;
	vdp1_env_color_mode_t color_mode;
	
	uint32_t erase_color;
	
	int16_vec2_t erase_points[2];
	
} vdp1_env_t;


void vdp1_env_default_init(vdp1_env_t* env);
vdp1_env_t* vdp1_env_get(void);
void vdp1_env_default_set(void);
void vdp1_env_set(const vdp1_env_t* env);


#endif
