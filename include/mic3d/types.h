#ifndef __FAKEYAUL_MIC3D_TYPES_H__
#define __FAKEYAUL_MIC3D_TYPES_H__


#include <gfx/vdp1/vdp1_cmdt.h>


typedef enum _RENDER_FLAGS
{
	RENDER_FLAGS_NONE = 0,
	RENDER_FLAGS_LIGHTING = 1 << 0,
	RENDER_FLAGS_NO_CLEAR = 1 << 1,
	RENDER_FLAGS_ALL = RENDER_FLAGS_LIGHTING | RENDER_FLAGS_NO_CLEAR,
} render_flags_t;


typedef uint16_t texture_slot_t;
typedef uint16_t gst_slot_t;




#endif
