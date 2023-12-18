#ifndef __FAKEYAUL_DRAWSTATE_H__
#define __FAKEYAUL_DRAWSTATE_H__



#include <gamemath/vector.h>
#include <gfx/vram.h>


typedef enum _drawColDepth
{
	COL_RGBA_1555,
	COL_RGB_888,
	COL_RGBA_8888,
	
} DrawColDepth;


typedef struct _drawstate
{
	VRAM* textureRam;
	VRAM* frameBuffer;
	int16_vec2_t frameBuffer_size;
	DrawColDepth colDepth;
} DrawState_t;



#endif
