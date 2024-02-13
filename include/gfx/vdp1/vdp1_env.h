#ifndef __FAKEYAUL_VDP1_ENV_H__
#define __FAKEYAUL_VDP1_ENV_H__


#include <core/colour.h>
#include <gamemath/vector.h>
#include <gfx/vdp1/vdp1_cmdt.h>
#include <gfx/vdp2/vdp2_sprite.h>



typedef enum VDP1_ENV_BPP
{
	VDP1_ENV_BPP_16 = 0,
	VDP1_ENV_BPP_8 = 1,
	
} vdp1_env_bpp_t;


typedef enum VDP1_ENV_ROTATION
{
	VDP1_ENV_ROTATION_0 = 0,
	VDP1_ENV_ROTATION_90 = 1
	
} vdp1_env_rotation_t;


typedef enum VDP1_ENV_COLOUR_MODE
{
	VDP1_ENV_COLOR_MODE_PALETTE = 0,
	VDP1_ENV_COLOR_MODE_RGB_PALETTE = 1,
	
} vdp1_env_color_mode_t;


typedef struct VDP1_ENV
{
	vdp1_env_bpp_t bpp;
	vdp1_env_rotation_t rotation;
	vdp1_env_color_mode_t color_mode;
	vdp2_sprite_type_t sprite_type;
	
	rgb1555_t erase_color;
	
	int16_vec2_t erase_points[2]; //yep it's min and max
	
} vdp1_env_t;


typedef struct VDP1_ENV_COORDS
{
	int16_vec2_t system_clip;
	int16_vec2_t user_clip_ul; //min
	int16_vec2_t user_clip_lr; //max
	int16_vec2_t local;
	
} vdp1_env_coords_t;


void vdp1_env_default_init(vdp1_env_t* env);
const vdp1_env_t* vdp1_env_get(void);
void vdp1_env_default_set(void);
void vdp1_env_set(const vdp1_env_t* env);
void vdp1_env_stop(void);

//Initialise the first four cmdts as system clip, user clip, local coord, and end command using env_coords
void vdp1_env_preamble_populate(vdp1_cmdt_t* cmdts, const vdp1_env_coords_t* env_coords);




#endif
