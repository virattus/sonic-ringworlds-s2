#include <gamemath/math3d.h>

#include <gamemath/fix16_vector.h>
#include <gamemath/angle.h>

#include <assert.h>


#define MIN_FOV_ANGLE 	DEG2ANGLE( 20.0f)
#define MAX_FOV_ANGLE 	DEG2ANGLE(120.0f)


void math3d_lookat(const lookat_t* lookat)
{
	    /* forward = normalize(forward)
     * right   = normalize(cross(up, forward))
     * up      = cross(forward, right)
     *
     * LookAt matrix:
     *   rightx   righty   rightz
     *   upx      upy      upz
     *   forwardx forwardy forwardz */
	
	
	fix16_vec3_sub(lookat->from, lookat->to, lookat->basis_forward);
	fix16_vec3_normalize(lookat->basis_forward);
	
	fix16_vec3_cross(lookat->up, lookat->basis_forward, lookat->basis_right);
	fix16_vec3_normalize(lookat->basis_right);
	
	fix16_vec3_cross(lookat->basis_forward, lookat->basis_right, lookat->basis_up);
}


fix16_t math3d_view_distance_calc(int16_t screen_width, angle_t fov_angle)
{
	//No point in screwing with this, the warped look is being done by something else
	fov_angle = clamp(fov_angle, MIN_FOV_ANGLE, MAX_FOV_ANGLE);
	assert(fov_angle != MIN_FOV_ANGLE);
	assert(fov_angle != MAX_FOV_ANGLE);
	
	const angle_t hfov_angle = fov_angle >> 1;
	const fix16_t screen_scale = fix16_from_int(screen_width) >> 1;
	const fix16_t tan = fix16_tan(hfov_angle);
	
	return fix16_mul(screen_scale, tan);
	
}
