#include <gamemath/mat43.h>

#include <gamemath/vector.h>
#include <gamemath/fix16_vector.h>





void fix16_mat43_zero(fix16_mat43_t* m0)
{
	fix16_mat33_zero(&m0->rotation);
	fix16_vec3_zero(&m0->translation);
}


void fix16_mat43_identity(fix16_mat43_t* m0)
{
	fix16_mat33_identity(&m0->rotation);
	fix16_vec3_zero(&m0->translation);
}


void fix16_mat43_dup(const fix16_mat43_t* m0, fix16_mat43_t* dest)
{
	fix16_mat33_dup(&m0->rotation, &dest->rotation);
	fix16_vec3_dup(&m0->translation, &dest->translation);
}


void fix16_mat43_invert(const fix16_mat43_t* m0, fix16_mat43_t* dest)
{
	const fix16_vec3_t* const m00 = &m0->rotation.row[0];
	const fix16_vec3_t* const m01 = &m0->rotation.row[1];
	const fix16_vec3_t* const m02 = &m0->rotation.row[2];
	
	fix16_mat33_transpose(&m0->rotation, &dest->rotation);
	
	dest->translation.x = -fix16_vec3_dot(m00, &m0->translation);
	dest->translation.y = -fix16_vec3_dot(m01, &m0->translation);
	dest->translation.z = -fix16_vec3_dot(m02, &m0->translation);
}


void fix16_mat43_inplace_invert(fix16_mat43_t* m0)
{
	const fix16_vec3_t* const m00 = &m0->rotation.row[0];
	const fix16_vec3_t* const m01 = &m0->rotation.row[1];
	const fix16_vec3_t* const m02 = &m0->rotation.row[2];

	m0->translation.x = -fix16_vec3_dot(m00, &m0->translation);
	m0->translation.y = -fix16_vec3_dot(m01, &m0->translation);
	m0->translation.z = -fix16_vec3_dot(m02, &m0->translation);
	
	fix16_mat33_inplace_transpose(&m0->rotation);
}


void fix16_mat43_lookat(const fix16_vec3_t* from, const fix16_vec3_t* to, const fix16_vec3_t* up, fix16_mat43_t* dest)
{
	fix16_mat33_lookat(from, to, up, &dest->rotation);
	fix16_vec3_dup(from, &dest->translation);
}


void fix16_mat43_mul(const fix16_mat43_t* m0, const fix16_mat43_t* m1, fix16_mat43_t* dest)
{
	fix16_mat33_mul(&m0->rotation, &m1->rotation, &dest->rotation);
	fix16_mat43_pos3_mul(m0, &m1->translation, &dest->translation);
}


void fix16_mat43_pos3_mul(const fix16_mat43_t* m0, const fix16_vec3_t* v,  fix16_vec3_t* dest)
{
	fix16_mat43_vec3_mul(m0, v, dest);
	
	dest->x += m0->translation.x;
	dest->y += m0->translation.y;
	dest->z += m0->translation.z;
}


void fix16_mat43_vec3_mul(const fix16_mat43_t* m0, const fix16_vec3_t* v,  fix16_vec3_t* dest)
{
	fix16_mat33_vec3_mul(&m0->rotation, v, dest);
}




void fix16_mat43_x_rotate(const fix16_mat43_t* m0, fix16_mat43_t* dest, fix16_t angle)
{
	fix16_mat33_x_rotate(&m0->rotation, angle, &dest->rotation);
	fix16_mat43_pos3_mul(dest, &m0->translation, &dest->translation);
}


void fix16_mat43_y_rotate(const fix16_mat43_t* m0, fix16_mat43_t* dest, fix16_t angle)
{
	fix16_mat33_y_rotate(&m0->rotation, angle, &dest->rotation);
	fix16_mat43_pos3_mul(dest, &m0->translation, &dest->translation);
}


void fix16_mat43_z_rotate(const fix16_mat43_t* m0, fix16_mat43_t* dest, fix16_t angle)
{
	fix16_mat33_z_rotate(&m0->rotation, angle, &dest->rotation);
	fix16_mat43_pos3_mul(dest, &m0->translation, &dest->translation);
}


void fix16_mat43_x_rotation_set(fix16_t rx, fix16_mat43_t* dest)
{
	fix16_mat33_x_rotation_create(rx, &dest->rotation);
}


void fix16_mat43_y_rotation_set(fix16_t ry, fix16_mat43_t* dest)
{
	fix16_mat33_y_rotation_create(ry, &dest->rotation);
}


void fix16_mat43_z_rotation_set(fix16_t rz, fix16_mat43_t* dest)
{
	fix16_mat33_z_rotation_create(rz, &dest->rotation);
}
 

size_t fix16_mat43_str(const fix16_mat43_t* m0, char* buffer, int32_t decimals)
{
    char* buffer_ptr;
    buffer_ptr = buffer;

    buffer_ptr += fix16_mat33_str(&m0->rotation, buffer_ptr, decimals);

    *buffer_ptr++ = '\n';
    *buffer_ptr++ = '|';
    buffer_ptr += fix16_vec3_str(&m0->translation, buffer_ptr, decimals);
    *buffer_ptr++ = '|';

	*buffer_ptr = '\n';
    *buffer_ptr = '\0';

    return (buffer_ptr - buffer);
}
