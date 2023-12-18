#ifndef __FAKEYAUL_PRIMITIVE_H__
#define __FAKEYAUL_PRIMITIVE_H__



#include <gamemath/vector.h>
#include <core/colour.h>

#include <libfixmath/fix16.h>

#include <stdbool.h>


typedef struct _RECT
{
	int16_vec2_t min;
	int16_vec2_t max;
} RECT;


typedef struct _DRAWPOLY_VERTEX
{
	int16_vec2_t pos;
	int16_vec2_t uv;
	CVECTOR col;
} VERTEX;


typedef struct _DRAWPOLY_FLAGS
{
	bool draw_mesh;
	bool draw_paletted;
	bool draw_transparent;
	bool highSpeedShrink;
	bool use_texture;
	uint16_t textureID;
	
} PRIMITIVE_FLAGS;


typedef struct _DRAWPOLY_PRIMITIVE
{
	int16_t zSort;
	PRIMITIVE_FLAGS flags;
	VERTEX v[4];
} PRIMITIVE;


#endif
