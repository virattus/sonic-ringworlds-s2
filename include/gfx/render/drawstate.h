#ifndef __FAKEYAUL_DRAWSTATE_H__
#define __FAKEYAUL_DRAWSTATE_H__



#include <gamemath/vector.h>
#include <gfx/vram.h>
#include <gfx/render/displaylist.h>


typedef enum DRAW_COL_DEPTH
{
	COL_RGB_1555,
	COL_RGBA_8888,
	
} DrawColDepth_t;


typedef struct DRAW_STATE
{
	VRAM* buffer;
	VRAM* textureBuffer;
	int16_vec2_t bufferSize;
	int16_vec2_t textureBufferSize;
	DrawColDepth_t colDepth;
	int16_vec2_t SystemClipMin;
	int16_vec2_t SystemClipMax;
	
} DrawState_t;


#endif
