#ifndef __FAKEYAUL_FIX16_VECTOR_H__
#define __FAKEYAUL_FIX16_VECTOR_H__



#include <gamemath/vector.h>

#include <libfixmath/fix16.h>

#include <stdint.h>
#include <stddef.h>





static inline fix16_t fix16_interpolate(fix16_t f0, fix16_t f1, uint16_t numerator, uint16_t denominator);


void fix16_vec2_zero(fix16_vec2_t* dest);
fix16_t fix16_vec2_length(const fix16_vec2_t* v);
fix16_t fix16_vec2_dot(const fix16_vec2_t* v0, const fix16_vec2_t* v1);
fix16_vec2_t fix16_vec2_interpolate(const fix16_vec2_t* v0, const fix16_vec2_t* v1, uint16_t numerator, uint16_t denominator);


void fix16_vec3_zero(fix16_vec3_t* dest);

void fix16_vec3_add(const fix16_vec3_t* v0, const fix16_vec3_t* v1, fix16_vec3_t* dest);
void fix16_vec3_sub(const fix16_vec3_t* v0, const fix16_vec3_t* v1, fix16_vec3_t* dest);
void fix16_vec3_dup(const fix16_vec3_t* v0, fix16_vec3_t* dest);

fix16_t fix16_vec3_dot(const fix16_vec3_t* v0, const fix16_vec3_t* v1);
void fix16_vec3_cross(const fix16_vec3_t* v0, const fix16_vec3_t* v1, fix16_vec3_t* dest);
fix16_t fix16_vec3_length(const fix16_vec3_t* v);
fix16_t fix16_vec3_sqr_length(const fix16_vec3_t* v);
void fix16_vec3_normalize(fix16_vec3_t* v);

size_t fix16_vec3_str(const fix16_vec3_t* v0, char* buffer, int32_t decimals);






//I think these are for getting the halves of the fix16
int32_t fix16_high_mul(fix16_t a, fix16_t b);
int32_t fix16_low_mul(fix16_t a, fix16_t b);


#endif
