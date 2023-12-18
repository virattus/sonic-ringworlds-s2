#include <gamemath/mat33.h>

#include <gamemath/math3d.h>
#include <gamemath/fix16_vector.h>

#include <assert.h>


void fix16_mat33_zero(fix16_mat33_t* m0)
{
	fix16_t* arr_ptr = (fix16_t*)m0;
	
	*arr_ptr++ = 0;
	*arr_ptr++ = 0;
	*arr_ptr++ = 0;
	
	*arr_ptr++ = 0;
	*arr_ptr++ = 0;
	*arr_ptr++ = 0;
	
	*arr_ptr++ = 0;
	*arr_ptr++ = 0;
	*arr_ptr = 0;
}


void fix16_mat33_identity(fix16_mat33_t* m0)
{
	fix16_t* arr_ptr = (fix16_t*)m0;
	
	*arr_ptr++ = fix16_one;
	*arr_ptr++ = 0;
	*arr_ptr++ = 0;
	
	*arr_ptr++ = 0;
	*arr_ptr++ = fix16_one;
	*arr_ptr++ = 0;
	
	*arr_ptr++ = 0;
	*arr_ptr++ = 0;
	*arr_ptr = fix16_one;
	
}


void fix16_mat33_dup(const fix16_mat33_t* m0, fix16_mat33_t* dest)
{
	fix16_t* arr_ptr = (fix16_t*)m0;
	fix16_t* dest_ptr = (fix16_t*)dest;
	
	*dest_ptr++ = *arr_ptr++;
	*dest_ptr++ = *arr_ptr++;
	*dest_ptr++ = *arr_ptr++;
	
	*dest_ptr++ = *arr_ptr++;
	*dest_ptr++ = *arr_ptr++;
	*dest_ptr++ = *arr_ptr++;
	
	*dest_ptr++ = *arr_ptr++;
	*dest_ptr++ = *arr_ptr++;
	*dest_ptr = *arr_ptr;
}


void fix16_mat33_transpose(const fix16_mat33_t* m0, fix16_mat33_t* dest)
{
	dest->row[0].x = m0->row[0].x;
	dest->row[0].y = m0->row[1].x;
	dest->row[0].z = m0->row[2].x;
	
	dest->row[1].x = m0->row[0].y;
	dest->row[1].y = m0->row[1].y;
	dest->row[1].z = m0->row[2].y;
	
	dest->row[2].x = m0->row[0].z;
	dest->row[2].y = m0->row[1].z;
	dest->row[2].z = m0->row[2].z;
}


void fix16_mat33_inplace_transpose(fix16_mat33_t* m0)
{
	const fix16_t m01 = m0->row[0].y;
	const fix16_t m02 = m0->row[0].z;
	
	m0->row[0].y = m0->row[1].x;
	m0->row[0].z = m0->row[2].x;
	
	const fix16_t m12 = m0->row[1].z;
	
	m0->row[1].x = m01;
	m0->row[1].z = m0->row[2].y;
	
	m0->row[2].x = m02;
	m0->row[2].y = m12;
}


void fix16_mat33_lookat(const fix16_vec3_t* from, const fix16_vec3_t* to, const fix16_vec3_t* up, fix16_mat33_t* dest)
{
	const lookat_t lookat = {
		.from = from,
		.to = to,
		.up = up,
		.basis_right = &dest->row[0],
		.basis_up = &dest->row[1],
		.basis_forward = &dest->row[2],
	};
	
	math3d_lookat(&lookat);
}


void fix16_mat33_mul(const fix16_mat33_t* m0, const fix16_mat33_t* m1, fix16_mat33_t* dest)
{
	fix16_mat33_t transpose;
	fix16_mat33_transpose(m1, &transpose);
	
	dest->row[0].x = fix16_vec3_dot(&m0->row[0], &transpose.row[0]);
	dest->row[0].y = fix16_vec3_dot(&m0->row[0], &transpose.row[1]);
	dest->row[0].z = fix16_vec3_dot(&m0->row[0], &transpose.row[2]);
	
	dest->row[1].x = fix16_vec3_dot(&m0->row[1], &transpose.row[0]);
	dest->row[1].y = fix16_vec3_dot(&m0->row[1], &transpose.row[1]);
	dest->row[1].z = fix16_vec3_dot(&m0->row[1], &transpose.row[2]);
	
	dest->row[2].x = fix16_vec3_dot(&m0->row[2], &transpose.row[0]);
	dest->row[2].y = fix16_vec3_dot(&m0->row[2], &transpose.row[1]);
	dest->row[2].z = fix16_vec3_dot(&m0->row[2], &transpose.row[2]);	
	
}


void fix16_mat33_vec3_mul(const fix16_mat33_t* m0, const fix16_vec3_t* v, fix16_vec3_t* dest)
{
	dest->x = fix16_vec3_dot(&m0->row[0], v);
	dest->y = fix16_vec3_dot(&m0->row[1], v);
	dest->z = fix16_vec3_dot(&m0->row[2], v);
}


void fix16_mat33_x_rotate(const fix16_mat33_t* m0, fix16_t angle, fix16_mat33_t* dest)
{
	//angle_t thing is only 16 bit, figure out what to do about this
	fix16_t sin = fix16_sin(angle);
	fix16_t cos = fix16_cos(angle);
	
	const fix16_t m01 = m0->row[0].y;
	const fix16_t m02 = m0->row[0].z;
	const fix16_t m11 = m0->row[1].y;
	const fix16_t m12 = m0->row[1].z;
	const fix16_t m21 = m0->row[2].y;
	const fix16_t m22 = m0->row[2].z;
	
	dest->row[0].y = fix16_mul(m01, cos) + fix16_mul(m02, sin);
	dest->row[0].z = -fix16_mul(m01, sin) + fix16_mul(m02, cos);
	dest->row[1].y = fix16_mul(m11, cos) + fix16_mul(m12, sin);
	dest->row[1].z = -fix16_mul(m11, sin) + fix16_mul(m12, cos);
	dest->row[2].y = fix16_mul(m21, cos) + fix16_mul(m22, sin);
	dest->row[2].z = -fix16_mul(m21, sin) + fix16_mul(m22, cos);

}


void fix16_mat33_y_rotate(const fix16_mat33_t* m0, fix16_t angle, fix16_mat33_t* dest)
{
	//angle_t thing is only 16 bit, figure out what to do about this
	fix16_t sin = fix16_sin(angle);
	fix16_t cos = fix16_cos(angle);
	
	const fix16_t m00 = m0->row[0].x;
	const fix16_t m02 = m0->row[0].z;
	const fix16_t m10 = m0->row[1].x;
	const fix16_t m12 = m0->row[1].z;
	const fix16_t m20 = m0->row[2].x;
	const fix16_t m22 = m0->row[2].z;
	
	
	dest->row[0].x = fix16_mul(m00, cos) + fix16_mul(m02, sin);
	dest->row[0].z = fix16_mul(m00, sin) + fix16_mul(m02, cos);
	dest->row[1].x = fix16_mul(m10, cos) + fix16_mul(m12, sin);
	dest->row[1].z = fix16_mul(m10, sin) + fix16_mul(m12, cos);
	dest->row[2].x = fix16_mul(m20, cos) + fix16_mul(m22, sin);
	dest->row[2].z = fix16_mul(m20, sin) + fix16_mul(m22, cos);
}


void fix16_mat33_z_rotate(const fix16_mat33_t* m0, fix16_t angle, fix16_mat33_t* dest)
{
	//angle_t thing is only 16 bit, figure out what to do about this
	fix16_t sin = fix16_sin(angle);
	fix16_t cos = fix16_cos(angle);
	
	const fix16_t m00 = m0->row[0].x;
	const fix16_t m01 = m0->row[0].y;
	const fix16_t m10 = m0->row[1].x;
	const fix16_t m11 = m0->row[1].y;
	const fix16_t m20 = m0->row[2].x;
	const fix16_t m21 = m0->row[2].y;
	
	
	dest->row[0].x = fix16_mul(m00, cos) + fix16_mul(m01, sin);
	dest->row[0].y = -fix16_mul(m00, sin) + fix16_mul(m01, cos);
	dest->row[1].x = fix16_mul(m10, cos) + fix16_mul(m11, sin);
	dest->row[1].y = -fix16_mul(m10, sin) + fix16_mul(m11, cos);
	dest->row[2].x = fix16_mul(m20, cos) + fix16_mul(m21, sin);
	dest->row[2].y = -fix16_mul(m20, sin) + fix16_mul(m21, cos);
}


void fix16_mat33_x_rotation_create(fix16_t angle, fix16_mat33_t* dest)
{
	//angle_t thing is only 16 bit, figure out what to do about this
	fix16_t sin = fix16_sin(angle);
	fix16_t cos = fix16_cos(angle);
	
	dest->row[0].x = fix16_one;
	dest->row[0].y = 0;
	dest->row[0].z = 0;
	
	dest->row[1].x = 0;
	dest->row[1].y = cos;
	dest->row[1].z = sin;

	dest->row[2].x = 0;
	dest->row[2].y = -sin;
	dest->row[2].z = cos;	
}


void fix16_mat33_y_rotation_create(fix16_t angle, fix16_mat33_t* dest)
{
	//angle_t thing is only 16 bit, figure out what to do about this
	fix16_t sin = fix16_sin(angle);
	fix16_t cos = fix16_cos(angle);
	
	dest->row[0].x = cos;
	dest->row[0].y = 0;
	dest->row[0].z = -sin;
	
	dest->row[1].x = 0;
	dest->row[1].y = fix16_one;
	dest->row[1].z = 0;

	dest->row[2].x = sin;
	dest->row[2].y = 0;
	dest->row[2].z = cos;	
}


void fix16_mat33_z_rotation_create(fix16_t angle, fix16_mat33_t* dest)
{
	//angle_t thing is only 16 bit, figure out what to do about this
	fix16_t sin = fix16_sin(angle);
	fix16_t cos = fix16_cos(angle);
	
	dest->row[0].x = cos;
	dest->row[0].y = sin;
	dest->row[0].z = 0;
	
	dest->row[1].x = -sin;
	dest->row[1].y = cos;
	dest->row[1].z = 0;

	dest->row[2].x = 0;
	dest->row[2].y = 0;
	dest->row[2].z = 1;	
}


void fix16_mat33_rotation_create(fix16_t rx, fix16_t ry, fix16_t rz, fix16_mat33_t* dest)
{
	fix16_mat33_t mat_a;
	fix16_mat33_x_rotation_create(rx, &mat_a);
	
	fix16_mat33_t mat_b;
	fix16_mat33_y_rotation_create(ry, &mat_b);
	
	fix16_mat33_t mat_c;
	fix16_mat33_z_rotation_create(rz, &mat_c);
	
	fix16_mat33_t mat_tmp;
	fix16_mat33_mul(&mat_a, &mat_b, &mat_tmp);
	fix16_mat33_mul(&mat_tmp, &mat_c, dest);
	
}


size_t fix16_mat33_str(const fix16_mat33_t* m0, char* buffer, int32_t decimals)
{
    char* buffer_ptr;
    buffer_ptr = buffer;

    *buffer_ptr++ = '|';
      buffer_ptr += fix16_vec3_str(&m0->row[0], buffer_ptr, decimals);
    *buffer_ptr++ = '|';
    *buffer_ptr++ = '\n';

    *buffer_ptr++ = '|';
      buffer_ptr += fix16_vec3_str(&m0->row[1], buffer_ptr, decimals);
    *buffer_ptr++ = '|';
    *buffer_ptr++ = '\n';

    *buffer_ptr++ = '|';
      buffer_ptr += fix16_vec3_str(&m0->row[2], buffer_ptr, decimals);
    *buffer_ptr++ = '|';

	*buffer_ptr = '\n';
    *buffer_ptr   = '\0';

    return (buffer_ptr - buffer);
}
