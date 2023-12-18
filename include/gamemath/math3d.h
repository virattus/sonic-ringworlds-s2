#ifndef __FAKEYAUL_MATH_3D_H__
#define __FAKEYAUL_MATH_3D_H__


#include <gamemath/vector.h>
#include <gamemath/angle.h>

#include <libfixmath/fix16.h>


typedef struct LOOKAT
{
	const fix16_vec3_t* from;
	const fix16_vec3_t* to;
	const fix16_vec3_t* up;
	
	fix16_vec3_t* basis_right;
	fix16_vec3_t* basis_up;
	fix16_vec3_t* basis_forward;
} lookat_t;



void math3d_lookat(const lookat_t* lookat);

fix16_t math3d_view_distance_calc(int16_t screen_width, angle_t fov_angle);

#endif
