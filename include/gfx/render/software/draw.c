#include <gfx/render/software/draw.h>

#include <core/colour.h>
#include <core/window.h>
#include <gfx/vdp1/vdp1.h>
#include <gamemath/fix16_vector.h>
#include <gamemath/int_vector.h>
#include <gfx/render/render.h>
#include <mic3d/state.h>
#include <gfx/texture.h>

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



static int32_t RangeTest(const int32_t value, const int32_t min, const int32_t max)
{
	if(value < min || value > max)
	{
		return 0;
	}
	
	return 1;
}



void DrawLine(const DrawState_t* state, const PRIMITIVE_FLAGS* flags, const VERTEX* v0, const VERTEX* v1)
{	
	int x, y;
	int dx, dy;
	int sx, sy;
	int err, e2;
	
	int xMin = min(v0->pos.x, v1->pos.x), yMin = min(v0->pos.y, v1->pos.y);
	int xMax = max(v0->pos.x, v1->pos.x), yMax = max(v0->pos.y, v1->pos.y);

	int numerator = 0;	
	int denominator = max(xMax - xMin, yMax - yMin) + 1;
	
	dx = v1->pos.x >= v0->pos.x ? v1->pos.x - v0->pos.x : v0->pos.x - v1->pos.x;
	dy = v1->pos.y >= v0->pos.y ? v0->pos.y - v1->pos.y : v1->pos.y - v0->pos.y;
	sx = v0->pos.x < v1->pos.x ? 1 : -1;
	sy = v0->pos.y < v1->pos.y ? 1 : -1;
	
	err = dx + dy;
	
	x = v0->pos.x;
	y = v0->pos.y;
	
	while(1)
	{
		int32_t offset = y * state->frameBuffer_size.x + x;
		
		//we only draw if it's inside the framebuffer, don't fuck with this you idiot	
		if(RangeTest(offset, 0, state->frameBuffer_size.x * state->frameBuffer_size.y - 1))
		{
			CVECTOR col = Interpolate_CVECTOR(&v0->col, &v1->col, numerator, denominator);
			VRAM_WritePixel(state->frameBuffer, offset, CVector_to_uint32(col));
		}
		
		numerator++;

		
		if((x == v1->pos.x) && (y == v1->pos.y))
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


void DrawSpan(const DrawState_t* state, const PRIMITIVE_FLAGS* flags, const VERTEX* v0, const VERTEX* v1)
{	
	//Off to right side of frame
	if(v0->pos.x > state->frameBuffer_size.x)
		return;
	
	//off to left side of frame
	if(v1->pos.x < 0)
		return;
	
	int16_t distance = v1->pos.x - v0->pos.x;
	
	//Doesn't span any pixels
	if(distance <= 0)
		return;
	
	VERTEX newV0 = *v0, newV1 = *v1;
	
	//Extends beyond left side
	if(newV0.pos.x < 0)
	{
		newV0.col = Interpolate_CVECTOR(&v0->col, &v1->col, -newV0.pos.x, distance);
		newV0.uv = int16_vec2_interpolate(&v0->uv, &v1->uv, -newV0.pos.x, distance);
		distance += newV0.pos.x;
		newV0.pos.x = 0;
	}
	
	//Extends beyond right side, clip at bounary
	if(newV1.pos.x >= state->frameBuffer_size.x)
	{
		int16_t newPos = state->frameBuffer_size.x - 1;
						
		newV1.col = Interpolate_CVECTOR(&newV0.col, &newV1.col, newPos, newV0.pos.x + distance);		
		newV1.uv = int16_vec2_interpolate(&newV0.uv, &newV1.uv, newPos, newV0.pos.x + distance);
		
		/*
		printf("oldpos: %d newDist: %d distance %d\n", v1->pos.x, newPos, distance);
		printf("v0col: %d %d %d oldcol: %d %d %d newcol: %d %d %d\n", 
			v0->col.r, v0->col.g, v0->col.b, 
			v1->col.r, v1->col.g, v1->col.b, 
			newV1.col.r, newV1.col.g, newV1.col.b);
		*/
		
		distance = state->frameBuffer_size.x - newV0.pos.x;
		
		//technically we don't need this
		newV1.pos.x = newPos;
	}
	
	/*
	printf("x0: %d x1: %d y: %d distance %d col0: %d %d %d col1: %d %d %d\n\n", 
		newV0.pos.x, newV1.pos.x, newV0.pos.y, distance,
		newV0.col.r, newV0.col.g, newV0.col.b,
		newV1.col.r, newV1.col.g, newV1.col.b);
	*/
	
	texture_t* tex = NULL;
	int16_vec2_t texSize = { 0, 0 };
	if(flags->use_texture)
	{
		tex = (texture_t*)(&__state.tlist->textures[flags->textureID]);
		texSize = TEXTURE_SIZE_TO_VEC(tex->size);
		//printf("using texture id %d\n", state->texture);
	}
	
	
	//printf("begin for loop\n");
	for(int i = 0; i < distance; i++)
	{
		int16_vec2_t newPos = { newV0.pos.x + i, newV0.pos.y };
		
		assert(newPos.x < state->frameBuffer_size.x);
		assert(newPos.y < state->frameBuffer_size.y);
		
		if(flags->draw_mesh)
		{
			if(!(newPos.x & 1) && (newPos.y & 1))
				continue;
			if((newPos.x & 1) && !(newPos.y & 1))
				continue;
		}
		
		//printf("mesh check\n");
		
		CVECTOR newColour = Interpolate_CVECTOR(&newV0.col, &newV1.col, i, distance);
		
		//printf("colour interpolation\n");
			
		if(flags->use_texture)
		{
			int16_vec2_t newUV = int16_vec2_interpolate(&newV0.uv, &newV1.uv, i, distance);
			newUV.x &= texSize.x - 1;
			newUV.y &= texSize.y - 1;
			
			//Getting absolutely ludicrous numbers, but remember to change this if you use larger amounts of ram
			assert(newUV.x < 512);
			assert(newUV.y < 512);

			uint32_t texColour = VRAM_ReadPixel(state->textureRam, TextureMapper_GetVRAMOffset(tex, &newUV));
		
			//temp until we learn how to blend
			//vertex colours are add/sub not mul
			//newColour = Add_CVECTOR(Scale_CVECTOR(newColour, 32767), Scale_CVECTOR(Uint32_to_CVector(texColour), 32767));
			
			newColour = Uint32_to_CVector(texColour);
		}
		
		//printf("get texture colour\n");
		
		uint32_t intCol = CVector_to_uint32(newColour);
		
		//if(state->colDepth == COL_RGBA_1555)
		{
			intCol = RGB1555_to_uint32(RGB1555_from_uint32(intCol));
		}
		
		uint32_t offset = newPos.y * state->frameBuffer_size.x + newPos.x;
				
		//printf("newpos x: %d y: %d offset: %d colour: %u\n", newPos.x, newPos.y, offset, intCol);
		
		VRAM_WritePixel(state->frameBuffer, (offset * BYTES_PER_PIXEL), intCol);
	}
}


void DrawPolyLine(const DrawState_t* state, const PRIMITIVE_FLAGS* flags, const VERTEX* v0, const VERTEX* v1, const VERTEX* v2, const VERTEX* v3)
{
	DrawLine(state, flags, v0, v1);
	DrawLine(state, flags, v1, v2);
	DrawLine(state, flags, v2, v3);
	DrawLine(state, flags, v3, v0);
}


////DRAW POLYGON HELPER FUNCTIONS ////


typedef struct _VertSpan
{
	VERTEX min, max;
	bool touched;
} VertSpan;


static void BresenhamFindNodes(const VERTEX* v0, const VERTEX* v1, VertSpan* entries, const int16_vec2_t range)
{	
	int x, y;
	int dx, dy;
	int sx, sy;
	int err, e2;
	
	int xMin = min(v0->pos.x, v1->pos.x), yMin = min(v0->pos.y, v1->pos.y);
	int xMax = max(v0->pos.x, v1->pos.x), yMax = max(v0->pos.y, v1->pos.y);

	int numerator = 0;	
	int denominator = max(xMax - xMin, yMax - yMin) + 1;
	
	dx = v1->pos.x >= v0->pos.x ? v1->pos.x - v0->pos.x : v0->pos.x - v1->pos.x;
	dy = v1->pos.y >= v0->pos.y ? v0->pos.y - v1->pos.y : v1->pos.y - v0->pos.y;
	sx = v0->pos.x < v1->pos.x ? 1 : -1;
	sy = v0->pos.y < v1->pos.y ? 1 : -1;
	
	err = dx + dy;
	
	x = v0->pos.x;
	y = v0->pos.y;
	
	while(1)
	{
		int16_vec2_t pos = { x, y };
		int16_vec2_t uv = int16_vec2_interpolate(&v0->uv, &v1->uv, numerator, denominator);
		CVECTOR col = Interpolate_CVECTOR(&v0->col, &v1->col, numerator, denominator);
		
		VERTEX vert = { 
			pos, uv, col 
		};

		if(RangeTest(y, range.x, range.y))
		{
			//printf("entryID: %d\n", y);
			
			if(x <= entries[y].min.pos.x)
			{
				entries[y].min = vert;
				entries[y].min.pos.y = y;
				entries[y].touched = true;
			}
			if(x >= entries[y].max.pos.x)
			{
				entries[y].max = vert;
			}
		}		

		numerator++;
		
		if((x == v1->pos.x) && (y == v1->pos.y))
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

/////// END POLYGON DRAW HELPER FUNCTIONS ///////////////


void DrawPolygon(const DrawState_t* state, const PRIMITIVE_FLAGS* flags, const VERTEX* v0, const VERTEX* v1, const VERTEX* v2, const VERTEX* v3)
{	
	VertSpan scanbuffer[state->frameBuffer_size.y];
	for(int i = 0; i < state->frameBuffer_size.y; i++)
	{
		scanbuffer[i].min.pos.x = 2048;
		scanbuffer[i].max.pos.x = -2048;
		scanbuffer[i].touched = false;
	}
	
	const int16_vec2_t range = { 0, state->frameBuffer_size.y - 1 };
	
	//printf("Line 0 and 1\n");
	BresenhamFindNodes(v0, v1, scanbuffer, range);
	
	//printf("Line 1 and 2\n");
	BresenhamFindNodes(v1, v2, scanbuffer, range);
	
	//printf("Line 2 and 3\n");
	BresenhamFindNodes(v2, v3, scanbuffer, range);
	
	//printf("Line 3 and 0\n");
	BresenhamFindNodes(v3, v0, scanbuffer, range);
	

	for(int i = 0; i < state->frameBuffer_size.y; i++)
	{		
		//printf("YPos: %d xMin: %d xMax: %d\n", scanbuffer[i].min.pos.y, scanbuffer[i].min.pos.x, scanbuffer[i].max.pos.x);

		if(scanbuffer[i].touched)
		{
			//printf("YPos: %d xMin: %d xMax: %d\n", scanbuffer[i].min.pos.y, scanbuffer[i].min.pos.x, scanbuffer[i].max.pos.x);
			DrawSpan(state, flags, &scanbuffer[i].min, &scanbuffer[i].max);
		}		
	}
}
