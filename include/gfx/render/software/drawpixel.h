#ifndef __FAKEYAUL_SOFTWARE_DRAWPIXEL_H__
#define __FAKEYAUL_SOFTWARE_DRAWPIXEL_H__


#include <core/preprocessor.h>
#include <core/colour.h>


static inline void DrawPixel_ApplyGouraud(rgb1555_t* colour, const rgb1555_t* gouraud)
{
	colour->r = CLAMP(colour->r + gouraud->r - 16, 0, 31);
	colour->g = CLAMP(colour->g + gouraud->g - 16, 0, 31);
	colour->b = CLAMP(colour->b + gouraud->b - 16, 0, 31);
}


static inline void DrawPixel_ApplyHalfLuminance(rgb1555_t* colour)
{
	colour->r >>= 1;
	colour->g >>= 1;
	colour->b >>= 1;
}


static inline void DrawPixel_ReadTexture()
{
}


static void DrawPixel_Flat()
{
}


static void DrawPixel_Gouraud()
{
}


static void DrawPixel_Textured()
{
}




#endif

