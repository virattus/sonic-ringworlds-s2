#ifndef __FAKEYAUL_RENDER_PRIMITIVE_H__
#define __FAKEYAUL_RENDER_PRIMITIVE_H__



#include <gamemath/vector.h>
#include <core/colour.h>

#include <libfixmath/fix16.h>
#include <gamemath/vector.h>

#include <stdbool.h>



typedef struct DRAW_VERTEX
{
	int16_t x, y;
	int16_t u, v;
	uint8_t r, g, b;
	
} Vertex_t;


typedef struct DRAW_VERTEX_TUPLE
{
	Vertex_t min, max;
	
} VertexPair_t;


typedef enum DRAW_PRIMITIVE_TYPE
{
	DRAW_LINE = 0,
	DRAW_POLYLINE = 1,
	DRAW_POLYGON = 2,
	
} PrimType_t;


typedef struct DRAW_FLAGS
{
	PrimType_t DrawPrimType;
	bool DrawMesh;
	bool DrawPaletted;
	bool DrawTransparent;
	bool DrawGouraud;
	bool DrawTextured;
	bool UserClipParameters;
	uint16_t TextureOffset;
	uint16_t TextureDimensions;
	
	
} PrimitiveFlags_t;


typedef struct DRAW_PRIMITIVE
{
	PrimitiveFlags_t flags;
	Vertex_t v[4];
} Primitive_t;

#endif
