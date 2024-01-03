#ifndef __FAKEYAUL_INT_VECTOR_H__
#define __FAKEYAUL_INT_VECTOR_H__


#include <core/preprocessor.h>
#include <gamemath/vector.h>

#include <stdint.h>
#include <stddef.h>



void int16_vec2_add(int16_vec2_t* v0, const int16_vec2_t* v1);
void int32_vec2_add(int32_vec2_t* v0, const int16_vec2_t* v1);

void int16_vec3_add(int16_vec3_t* v0, const int16_vec3_t* v1);
void int32_vec3_add(int32_vec3_t* v0, const int32_vec3_t* v1);


static inline int16_t __always_inline 
int16_interpolate(int16_t s0, int16_t s1, uint16_t numerator, uint16_t denominator)
{
	return fix16_to_int(fix16_lerp16(fix16_from_int(s0), fix16_from_int(s1), (numerator << 16) / denominator));
}


static inline int32_t __always_inline
int32_interpolate(int32_t i0, int32_t i1, uint16_t numerator, uint16_t denominator)
{
	return fix16_to_int(fix16_lerp16(fix16_from_int(i0), fix16_from_int(i1), (numerator << 16) / denominator));
}

int16_vec2_t int16_vec2_interpolate(const int16_vec2_t* v0, const int16_vec2_t* v1, uint16_t numerator, uint16_t denominator);
int16_vec3_t int16_vec3_interpolate(const int16_vec3_t* v0, const int16_vec3_t* v1, uint16_t numerator, uint16_t denominator);

int32_vec2_t int32_vec2_interpolate(const int32_vec2_t* v0, const int32_vec2_t* v1, uint16_t numerator, uint16_t denominator);
int32_vec3_t int32_vec3_interpolate(const int32_vec3_t* v0, const int32_vec3_t* v1, uint16_t numerator, uint16_t denominator);

size_t int16_str(int16_t i0, char* buffer, int32_t decimals);
size_t int32_str(int32_t i0, char* buffer, int32_t decimals);

size_t int16_vec2_str(const int16_vec2_t* v0, char* buffer, int32_t decimals);
size_t int32_vec2_str(const int32_vec2_t* v0, char* buffer, int32_t decimals);

size_t int16_vec3_str(const int16_vec3_t* v0, char* buffer, int32_t decimals);
size_t int32_vec3_str(const int32_vec3_t* v0, char* buffer, int32_t decimals);


#endif
