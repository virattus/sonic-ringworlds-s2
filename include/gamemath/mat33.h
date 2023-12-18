#ifndef __FAKEYAUL_MAT_33_H__
#define __FAKEYAUL_MAT_33_H__


#include <gamemath/vector.h>

#include <stddef.h>


#define FIX16_MAT33_COLUMNS		(3)
#define FIX16_MAT33_ROWS		(3)
#define FIX16_MAT33_ARR_COUNT 	(FIX16_MAT33_COLUMNS * FIX16_MAT33_ROWS)


typedef struct _MAT_33
{
	fix16_vec3_t row[FIX16_MAT33_ROWS];
} fix16_mat33_t;


void fix16_mat33_zero(fix16_mat33_t* m0);
void fix16_mat33_identity(fix16_mat33_t* m0);
void fix16_mat33_dup(const fix16_mat33_t* m0, fix16_mat33_t* dest);
void fix16_mat33_transpose(const fix16_mat33_t* m0, fix16_mat33_t* dest);
void fix16_mat33_inplace_transpose(fix16_mat33_t* m0);

void fix16_mat33_lookat(const fix16_vec3_t* from, const fix16_vec3_t* to, const fix16_vec3_t* up, fix16_mat33_t* dest);

void fix16_mat33_mul(const fix16_mat33_t* m0, const fix16_mat33_t* m1, fix16_mat33_t* dest);
void fix16_mat33_vec3_mul(const fix16_mat33_t* m0, const fix16_vec3_t* v, fix16_vec3_t* dest);

void fix16_mat33_x_rotate(const fix16_mat33_t* m0, fix16_t angle, fix16_mat33_t* dest);
void fix16_mat33_y_rotate(const fix16_mat33_t* m0, fix16_t angle, fix16_mat33_t* dest);
void fix16_mat33_z_rotate(const fix16_mat33_t* m0, fix16_t angle, fix16_mat33_t* dest);

void fix16_mat33_x_rotation_create(fix16_t angle, fix16_mat33_t* dest);
void fix16_mat33_y_rotation_create(fix16_t angle, fix16_mat33_t* dest);
void fix16_mat33_z_rotation_create(fix16_t angle, fix16_mat33_t* dest);

void fix16_mat33_rotation_create(fix16_t rx, fix16_t ry, fix16_t rz, fix16_mat33_t* dest);


size_t fix16_mat33_str(const fix16_mat33_t* m0, char* buffer, int32_t decimals);

#endif
