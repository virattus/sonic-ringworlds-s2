#include <gfx/render/software/draw.h>

#include <core/colour.h>
#include <core/window.h>
#include <gfx/vdp1/vdp1.h>
#include <gamemath/fix16_vector.h>
#include <gamemath/int_vector.h>
#include <mic3d/texture.h>
#include <mic3d/state.h>

#include <stddef.h>
#include <assert.h>
#include <stdbool.h>


/*
 * it depends on color mode you use but the best way to convert color to "saturn" palette is to use these macros :

#define RGB( r, g, b ) (0x8000U|((b) << 10)|((g) << 5 )|(r))
#define RGB16_COLOR(r,g,b) ()(((b)<<10) + ((g)<<5) + (r) + 0x8000)
//Make 16 bit RGB Color
#define RGB32_COLOR(r,g,b) (Rgb32)(((b)<<16) + ((g)<<8) + (r) + 0x80000000)

from https://segaxtreme.net/threads/saturn-color-palette.23902/
might help out, I haven't read it all yet to fully understand what it is. 
Outside of that, from what I can gather, there are two different modes of coloring.
VDP1: 256 to 32,768
VDP2: 16 colors (4-bit) to 16,777,216 colors
(from https://segaretro.org/Sega_Saturn/Technical_specifications) (edited)


*/


//For now we'll expect a 32 bit framebuffer
#define BYTES_PER_PIXEL		4

#define SCANBUFFER_MAX_VAL	2048



static inline int32_t RangeTest(int32_t value, int32_t min, int32_t max)
{
	if(value < min || value > max)
	{
		return 0;
	}
	
	return 1;
}



static inline float lerpStepSize(float x0, float x1, int16_t rStart, int16_t rEnd)
{
	return ((x1 - x0) / (rEnd - rStart));
}

static inline float lerp(float x0, float x1, int16_t rStart, int16_t rEnd, int16_t intersection)
{
	return (float)x0 + (lerpStepSize(x0, x1, rStart, rEnd) * (float)(intersection - rStart));
}

static inline int16_t BresenhamIntersection(int16_t y, int16_t x0, int16_t x1, int16_t y0, int16_t y1)
{	
	return roundf(x0 + (((float)(x1 - x0) * (float)(y - y0)) / (float)(y1 - y0)));
	//return roundf(v0->x + (lerpStepSize(v0->x, v1->x, v0->y, v1->y) * (float)(y - v0->y)));
}



static void BresenhamSearch(VertexPair_t* scanbuffer, const Vertex_t* v0, const Vertex_t* v1, const int16_vec2_t* range)
{	
	int x = v0->x, y = v0->y;
	int dx = v1->x >= v0->x ? v1->x - v0->x : v0->x - v1->x;
	int dy = v1->y >= v0->y ? v0->y - v1->y : v1->y - v0->y;
	int sx = v0->x < v1->x ? 1 : -1;
	int sy = v0->y < v1->y ? 1 : -1;
	int err = dx + dy;
	int e2;
	
	int xMin = MIN(v0->x, v1->x), yMin = MIN(v0->y, v1->y);
	int xMax = MAX(v0->x, v1->x), yMax = MAX(v0->y, v1->y);

	int numerator = 0;	
	int denominator = MAX(xMax - xMin, yMax - yMin);

	//TODO clamp search to screen space
	
	while(1)
	{		
		if(RangeTest(y, range->x, range->y - 1))
		{
			Vertex_t newVert = {
				.x = x,
				.y = y,
				.u = (uint16_t)lerp(v0->u, v1->u, 0, denominator, numerator),
				.v = (uint16_t)lerp(v0->v, v1->v, 0, denominator, numerator),
				.r = (uint8_t)lerp(v0->r, v1->r, 0, denominator, numerator),
				.g = (uint8_t)lerp(v0->g, v1->g, 0, denominator, numerator),
				.b = (uint8_t)lerp(v0->b, v1->b, 0, denominator, numerator),
			};
						
			if(x <= scanbuffer[y].min.x)
			{
				scanbuffer[y].min = newVert;
			}
			if(x >= scanbuffer[y].max.x)
			{
				scanbuffer[y].max = newVert;
			}
		}		

		numerator++;
		
		if((x == v1->x) && (y == v1->y))
			break;
		
		e2 = 2 * err;
		
		if(e2 >= dy)
		{
			err += dy;
			x += sx;
		}
		if(e2 <= dx)
		{
			err += dx;
			y += sy;
		}
	}
}


static void DrawPixel(DrawState_t* state, const PrimitiveFlags_t* flags, const Vertex_t* vert)
{	
	const uint32_t offset = (vert->y * _state_vdp1()->framebufferDimensions.x) + vert->x;
	//printf("Writing pixel to X: %d Y: %d Offset: %d\n", vert->x, vert->y, offset);

	if(flags->DrawMesh)
	{
		if((vert->x & 0x0001) != (vert->y & 0x0001))
			return;
	}

	rgba8888_t colour = {
		.r = vert->r,
		.g = vert->g,
		.b = vert->b,
		.a = 0xFF
	};
	
	if(flags->DrawTransparent)
	{
		colour.r >>= 1;
		colour.g >>= 1;
		colour.b >>= 1;
		colour.a >>= 1;
		
		rgba8888_t source;
		source.raw = VRAM_ReadInt(state->buffer, offset);
		
		source.r >>= 1;
		source.g >>= 1;
		source.b >>= 1;
		source.a >>= 1;
		
		colour.raw += source.raw;
	}
	
	VRAM_WriteInt(state->buffer, offset, colour.raw);
}



void DrawLine(DrawState_t* state, const PrimitiveFlags_t* flags, const Vertex_t* v0, const Vertex_t* v1)
{	
	//Copy pasted, figure out how to reuse the bresenham search func
	
	int x = v0->x, y = v0->y;
	int dx = v1->x >= v0->x ? v1->x - v0->x : v0->x - v1->x;
	int dy = v1->y >= v0->y ? v0->y - v1->y : v1->y - v0->y;
	int sx = v0->x < v1->x ? 1 : -1;
	int sy = v0->y < v1->y ? 1 : -1;
	int err = dx + dy;
	int e2;
	
	int xMin = MIN(v0->x, v1->x), yMin = MIN(v0->y, v1->y);
	int xMax = MAX(v0->x, v1->x), yMax = MAX(v0->y, v1->y);

	int numerator = 0;	
	int denominator = MAX(xMax - xMin, yMax - yMin);
	
	if(denominator == 0)
	{
		//Line with length 0
		return;
	}
	
	int16_vec2_t clipMin = state->SystemClipMin;
	int16_vec2_t clipMax = state->SystemClipMax;
	
	if(flags->UserClipParameters)
	{
		clipMin = state->UserClipMin;
		clipMax = state->UserClipMax;
	}
	
	
	while(1)
	{		
		if(RangeTest(x, clipMin.x, clipMax.x - 1) &&
			(RangeTest(y, clipMin.y, clipMax.y - 1)))
		{
			Vertex_t newVert = {
				.x = x,
				.y = y,
				.u = (uint16_t)lerp(v0->u, v1->u, 0, denominator, numerator),
				.v = (uint16_t)lerp(v0->v, v1->v, 0, denominator, numerator),
				.r = (uint8_t)lerp(v0->r, v1->r, 0, denominator, numerator),
				.g = (uint8_t)lerp(v0->g, v1->g, 0, denominator, numerator),
				.b = (uint8_t)lerp(v0->b, v1->b, 0, denominator, numerator),
			};
						
			DrawPixel(state, flags, &newVert);
		}		

		numerator++;
		
		if((x == v1->x) && (y == v1->y))
			break;
		
		e2 = 2 * err;
		
		if(e2 >= dy)
		{
			err += dy;
			x += sx;
		}
		if(e2 <= dx)
		{
			err += dx;
			y += sy;
		}
	}
}



void DrawSpan(DrawState_t* state, const PrimitiveFlags_t* flags, const Vertex_t* v0, const Vertex_t* v1, int16_t y)
{
	/*printf("Span Y: %d X:(%d-%d) R:(%d-%d) G:(%d-%d) B:(%d-%d)\n",
		y, 
		v0->x, v1->x,
		v0->r, v1->r,
		v0->g, v1->g,
		v0->b, v1->b);
		*/	
	
	
	int16_vec2_t clipMin = state->SystemClipMin;
	int16_vec2_t clipMax = state->SystemClipMax;
	
	if(flags->UserClipParameters)
	{
		clipMin = state->UserClipMin;
		clipMax = state->UserClipMax;
	}
	
	int16_vec2_t range = {
		.x = v0->x,
		.y = v1->x,
	};
	
	//Off to right side
	if(range.x > clipMax.x)
	{
		return;
	}
	
	//Off to left side
	if(range.y < clipMin.x)
	{
		return;
	}	
	
	float r = v0->r, g = v0->g, b = v0->b;
	float u = v0->u, v = v0->v;
	
	const float rStep = lerpStepSize(v0->r, v1->r, v0->x, v1->x);
	const float gStep = lerpStepSize(v0->g, v1->g, v0->x, v1->x);
	const float bStep = lerpStepSize(v0->b, v1->b, v0->x, v1->x);
	
	const float uStep = lerpStepSize(v0->u, v1->u, v0->x, v1->x);
	const float vStep = lerpStepSize(v0->v, v1->v, v0->x, v1->x);
	
	//If the polygon is valid, but off the side of the screen, then we need to 
	//preload their starting values so that we don't waste time iterating over
	//pixels we can't see
	if(range.x < clipMin.x)
	{
		r += rStep * (-range.x);
		g += gStep * (-range.x);
		b += bStep * (-range.x);
		u += uStep * (-range.x);
		v += vStep * (-range.x);
		
		range.x = clipMin.x;
	}
	
	if(range.y > clipMax.x)
	{
		range.y = clipMax.x;
	}
			
	for(int i = range.x; i < range.y; i++)
	{
		r += rStep;
		g += gStep;
		b += bStep;
		u += uStep;
		v += vStep;
		
		Vertex_t vert = {
			.x = i,
			.y = y,
			.r = (uint8_t)r,
			.g = (uint8_t)g,
			.b = (uint8_t)b,
			.u = (int16_t)u,
			.v = (int16_t)v,
		};
		
		DrawPixel(state, flags, &vert);
	}
}


void DrawPolyLine(DrawState_t* state, const PrimitiveFlags_t* flags, const Vertex_t* v0, const Vertex_t* v1, const Vertex_t* v2, const Vertex_t* v3)
{
	DrawLine(state, flags, v0, v1);
	DrawLine(state, flags, v1, v2);
	DrawLine(state, flags, v2, v3);
	DrawLine(state, flags, v3, v0);
}


/*
 * You moron, yes there's a divide by zero using the bresenham intersection math, but why does it matter
 * The ENTIRE POINT is to find the leftmost and rightmost points that the vertices describe, that's themselves!
 * They are the fucking left and rightmost points! Rewrite this to use the FindSpan func that you wrote in December 
 */
void DrawQuad(DrawState_t* state, const PrimitiveFlags_t* flags, const Vertex_t* v0, const Vertex_t* v1, const Vertex_t* v2, const Vertex_t* v3)
{	
	int16_vec2_t totalRange = {
		.x = MIN(MIN(MIN(v0->y, v1->y), v2->y), v3->y),
		.y = MAX(MAX(MAX(v0->y, v1->y), v2->y), v3->y),
	};
	
	if(totalRange.x == totalRange.y)
	{
		//Zero size polygon
		return;
	}
	
	
	int16_vec2_t clipMin = state->SystemClipMin;
	int16_vec2_t clipMax = state->SystemClipMax;
	
	if(flags->UserClipParameters)
	{
		clipMin = state->UserClipMin;
		clipMax = state->UserClipMax;
	}
	
	if(totalRange.x < clipMin.y)
	{
		totalRange.x = clipMin.y;
	}
	
	if(totalRange.y > clipMax.y)
	{
		totalRange.y = clipMax.y;
	}
		
	//printf("Total Range: %d to %d\n", totalRange.x, totalRange.y);
	
	const int32_t bufferSize = clipMax.y - clipMin.y;
	
	VertexPair_t spanVerts[bufferSize + 1]; //fuck it easy way to deal with stack smashing
	for(int i = 0; i < bufferSize; i++)
	{
		spanVerts[i] = (VertexPair_t){
			.min = { .x = SCANBUFFER_MAX_VAL, },
			.max = { .x = -SCANBUFFER_MAX_VAL, },
		};
	}
	
	
	BresenhamSearch(spanVerts, v0, v1, &totalRange);
	BresenhamSearch(spanVerts, v1, v2, &totalRange);
	BresenhamSearch(spanVerts, v2, v3, &totalRange);
	BresenhamSearch(spanVerts, v3, v0, &totalRange);
	
	
	for(int i = totalRange.x; i < totalRange.y; i++)
	{
		DrawSpan(state, flags, &spanVerts[i].min, &spanVerts[i].max, i);
	}	
}
