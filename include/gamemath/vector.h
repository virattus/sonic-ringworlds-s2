#ifndef __FAKEYAUL_VECTOR_H__
#define __FAKEYAUL_VECTOR_H__


#include <core/preprocessor.h>
#include <libfixmath/fix16.h>


//#define FIX16(x)						fix16_from_float(x)

#define fix16_int32_from(x)				(x << 16)
#define fix16_int32_to(x)				(x >> 16)

#define FIX16(x) ((fix16_t)(((x) >= 0) \
    ? ((double)(x) * 65536.0 + 0.5) \
    : ((double)(x) * 65536.0 - 0.5)))

#define FIX16_VEC3_INITIALIZER(x, y, z)	{ FIX16(x), FIX16(y), FIX16(z) }


#define INT16_VEC2_INITIALIZER(x, y)		(int16_vec2_t){ (x), (y) }

#define INT16_VEC3_INITIALIZER(x, y, z)		(int16_vec3_t){ (x), (y), (z) }


typedef struct _VECTOR2
{
	int32_t x, y;
} fix16_vec2_t, int32_vec2_t;


typedef struct _VECTOR3
{
	int32_t x, y, z;
} fix16_vec3_t, int32_vec3_t;


typedef struct _VECTOR4
{
	int32_t x, y, z, w;
} fix16_vec4_t, int32_vec4_t;


typedef struct _SVECTOR2
{
	int16_t x, y;
} int16_vec2_t;


typedef struct _SVECTOR3
{
	int16_t x, y, z;
} int16_vec3_t;


typedef struct _BVECTOR2
{
	int8_t x, y;
} int8_vec2_t;


typedef struct _BVECTOR3
{
	int8_t x, y, z;
} int8_vec3_t;


#endif
