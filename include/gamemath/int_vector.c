#include <gamemath/int_vector.h>

#include <gamemath/fix16_vector.h>

#include <stdio.h>



void int16_vec2_add(int16_vec2_t* v0, const int16_vec2_t* v1)
{
	v0->x += v1->x;
	v0->y += v1->y;
}


void int32_vec2_add(int32_vec2_t* v0, const int16_vec2_t* v1)
{
	v0->x += v1->x;
	v0->y += v1->y;
}


void int16_vec3_add(int16_vec3_t* v0, const int16_vec3_t* v1)
{
	v0->x += v1->x;
	v0->y += v1->y;
	v0->z += v1->z;
}


void int32_vec3_add(int32_vec3_t* v0, const int32_vec3_t* v1)
{
	v0->x += v1->x;
	v0->y += v1->y;
	v0->z += v1->z;
}

/*
int16_t int16_interpolate(int16_t s0, int16_t s1, uint16_t numerator, uint16_t denominator)
{
	return fix16_to_int(fix16_lerp16(fix16_from_int(s0), fix16_from_int(s1), (numerator << 16) / denominator));
}
*/

/*
int32_t int32_interpolate(int32_t i0, int32_t i1, uint16_t numerator, uint16_t denominator)
{
	return fix16_to_int(fix16_lerp16(fix16_from_int(i0), fix16_from_int(i1), (numerator << 16) / denominator));
}
*/

int16_vec2_t int16_vec2_interpolate(const int16_vec2_t* v0, const int16_vec2_t* v1, uint16_t numerator, uint16_t denominator)
{
	int16_vec2_t retval = {
		.x = int16_interpolate(v0->x, v1->x, numerator, denominator),
		.y = int16_interpolate(v0->y, v1->y, numerator, denominator),
	};
	
	return retval;
}


int16_vec3_t int16_vec3_interpolate(const int16_vec3_t* v0, const int16_vec3_t* v1, uint16_t numerator, uint16_t denominator)
{
	int16_vec3_t retval = {
		.x = int16_interpolate(v0->x, v1->x, numerator, denominator),
		.y = int16_interpolate(v0->y, v1->y, numerator, denominator),
		.z = int16_interpolate(v0->z, v1->z, numerator, denominator),
	};
	
	return retval;
}


int32_vec2_t int32_vec2_interpolate(const int32_vec2_t* v0, const int32_vec2_t* v1, uint16_t numerator, uint16_t denominator)
{
	int32_vec2_t v = {
		int32_interpolate(v0->x, v1->x, numerator, denominator),
		int32_interpolate(v0->y, v1->y, numerator, denominator),
	};

	return v;
}


int32_vec3_t int32_vec3_interpolate(const int32_vec3_t* v0, const int32_vec3_t* v1, uint16_t numerator, uint16_t denominator)
{
	int32_vec3_t v = {
		int32_interpolate(v0->x, v1->x, numerator, denominator),
		int32_interpolate(v0->y, v1->y, numerator, denominator),
		int32_interpolate(v0->z, v1->z, numerator, denominator),
	};
	
	return v;
}


size_t int16_vec2_str(const int16_vec2_t* v0, char* buffer, int32_t decimals)
{
	char *buffer_ptr;
    buffer_ptr = buffer;
    
	buffer_ptr += sprintf(buffer_ptr, "(%d, %d)", v0->x, v0->y);

    return (buffer_ptr - buffer);
}


size_t int32_vec2_str(const int32_vec2_t* v0, char* buffer, int32_t decimals)
{
	char *buffer_ptr;
    buffer_ptr = buffer;
    
	buffer_ptr += sprintf(buffer_ptr, "(%d, %d)", v0->x, v0->y);

    return (buffer_ptr - buffer);
}


size_t int16_vec3_str(const int16_vec3_t* v0, char* buffer, int32_t decimals)
{
	char *buffer_ptr;
    buffer_ptr = buffer;
    
	buffer_ptr += sprintf(buffer_ptr, "(%d, %d, %d)", v0->x, v0->y, v0->z);

    return (buffer_ptr - buffer);
}


size_t int32_vec3_str(const int32_vec3_t* v0, char* buffer, int32_t decimals)
{
	char *buffer_ptr;
    buffer_ptr = buffer;
    
	buffer_ptr += sprintf(buffer_ptr, "(%d, %d, %d)", v0->x, v0->y, v0->z);

    return (buffer_ptr - buffer);
}
