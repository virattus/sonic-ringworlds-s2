#ifndef __FAKEYAUL_MAT_43_H__
#define __FAKEYAUL_MAT_43_H__


#include <gamemath/mat33.h>
#include <gamemath/vector.h>

#include <libfixmath/fix16.h>

#include <stdint.h>
#include <stddef.h>



#define FIX16_MAT43_COLUMNS 	(4)
#define FIX16_MAT43_ROWS		(3)
#define FIX16_MAT43_ARR_COUNT	(FIX16_MAT43_COLUMNS * FIX16_MAT43_ROWS)


//ROW MAJOR MATRIX
typedef struct _fix16_mat43
{
	fix16_mat33_t rotation;
	fix16_vec3_t translation;
} fix16_mat43_t;


void fix16_mat43_zero(fix16_mat43_t* m0);
void fix16_mat43_identity(fix16_mat43_t* m0);
void fix16_mat43_dup(const fix16_mat43_t* m0, fix16_mat43_t* dest);
void fix16_mat43_invert(const fix16_mat43_t* m0, fix16_mat43_t* dest);
void fix16_mat43_inplace_invert(fix16_mat43_t* m0);
void fix16_mat43_lookat(const fix16_vec3_t* from, const fix16_vec3_t* to, const fix16_vec3_t* up, fix16_mat43_t* dest);



void fix16_mat43_mul(const fix16_mat43_t* m0, const fix16_mat43_t* m1, fix16_mat43_t* dest);
void fix16_mat43_pos3_mul(const fix16_mat43_t* m0, const fix16_vec3_t* v, fix16_vec3_t* dest);
void fix16_mat43_vec3_mul(const fix16_mat43_t* m0, const fix16_vec3_t* v, fix16_vec3_t* dest);

void fix16_mat43_x_rotate(const fix16_mat43_t* m0, fix16_mat43_t* dest, fix16_t angle);
void fix16_mat43_y_rotate(const fix16_mat43_t* m0, fix16_mat43_t* dest, fix16_t angle);
void fix16_mat43_z_rotate(const fix16_mat43_t* m0, fix16_mat43_t* dest, fix16_t angle);

void fix16_mat43_x_rotation_set(fix16_t rx, fix16_mat43_t* dest);
void fix16_mat43_y_rotation_set(fix16_t ry, fix16_mat43_t* dest);
void fix16_mat43_z_rotation_set(fix16_t rz, fix16_mat43_t* dest);

void fix16_mat43_rotation_set(fix16_t rx, fix16_t ry, fix16_t rz, fix16_mat43_t* dest);

//writes out the mat43 to a string for debugging?
size_t fix16_mat43_str(const fix16_mat43_t* m0, char* buffer, int32_t decimals);

#endif
