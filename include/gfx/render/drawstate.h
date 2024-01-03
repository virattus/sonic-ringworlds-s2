#ifndef __FAKEYAUL_DRAWSTATE_H__
#define __FAKEYAUL_DRAWSTATE_H__



#include <gamemath/vector.h>
#include <gfx/vram.h>
#include <gfx/render/displaylist.h>


typedef enum _drawColDepth
{
	COL_ARGB_1555,
	COL_RGB_888,
	COL_RGBA_8888,
	
} DrawColDepth;


typedef struct DRAW_STATE
{
	VRAM* buffer;
	VRAM* textureBuffer;
	int16_vec2_t bufferSize;
	int16_vec2_t textureBufferSize;
	DrawColDepth colDepth;
	int16_vec2_t SystemClipMin, SystemClipMax;
	int16_vec2_t UserClipMin, UserClipMax;
	
} DrawState_t;



#endif
