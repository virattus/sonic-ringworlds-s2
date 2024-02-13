#ifndef __FAKEYAUL_RENDER_PRIMITIVE_H__
#define __FAKEYAUL_RENDER_PRIMITIVE_H__



#include <core/colour.h>
#include <gamemath/vector.h>

#include <libfixmath/fix16.h>

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


typedef enum DRAW_COLOUR_MODE
{
	COLOUR_BANK_16 = 0,
	COLOUR_LOOKUP_16 = 1,
	COLOUR_BANK_64 = 2,
	COLOUR_BANK_128 = 3,
	COLOUR_BANK_256 = 4,
	COLOUR_RGB = 5,
	
} PrimColourMode_t;


typedef struct DRAW_FLAGS
{
	PrimType_t DrawPrimType;
	PrimColourMode_t ColourMode;
	bool DrawMesh;
	bool DrawTransparent;
	bool DrawHalfLuminance;
	bool DrawGouraud;
	bool DrawTextured;
	bool IgnoreTextureTransparency; //This is for paletted textures apparently?
	uint16_t TextureOffset;
	uint16_t TextureDimensions;
	int16_vec2_t ClipMin, ClipMax;
	
} PrimitiveFlags_t;


typedef struct DRAW_PRIMITIVE
{
	PrimitiveFlags_t flags;
	Vertex_t v[4];
} Primitive_t;

#endif
