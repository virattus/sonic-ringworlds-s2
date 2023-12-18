#ifndef __FAKEYAUL_CAMERA_H__
#define __FAKEYAUL_CAMERA_H__


#include <gamemath/vector.h>
#include <gamemath/mat43.h>


typedef enum _camera_type
{
	CAMERA_TYPE_ORTHOGRAPHIC,
	CAMERA_TYPE_PERSPECTIVE
} camera_type_t;


typedef struct _CAMERA
{
	fix16_vec3_t position;
	fix16_vec3_t target;
	fix16_vec3_t up;
} camera_t;


void camera_type_set(camera_type_t type);
void camera_lookat(const camera_t* camera);

//this only moves the camera's position, and not it's target
void camera_moveto(const camera_t* camera);
fix16_mat43_t* camera_matrix_get(void);


#endif
