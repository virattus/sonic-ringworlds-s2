#ifndef __FAKEYAUL_COLOUR_H__
#define __FAKEYAUL_COLOUR_H__


#include <core/preprocessor.h>
#include <gamemath/vector.h>
#include <gamemath/int_vector.h>

#include <stdint.h>


#define RGB1555_MSB_MASK	0x8000
#define RGB1555_R_MASK		31 //0x1F
#define RGB1555_G_MASK		992 //fucking wonky
#define RGB1555_B_MASK		31744 //0x7C00




typedef union COLOUR_RGBM1555
{
	struct
	{
		unsigned int msb:1;
		unsigned int b:5;
		unsigned int g:5;
		unsigned int r:5;
	} __packed;
	
	uint16_t raw;
	
} __aligned(2) rgb1555_t;


typedef union COLOUR_RGBA8888
{
	struct
	{
		unsigned int a: 8;
		unsigned int b: 8;
		unsigned int g: 8;
		unsigned int r: 8;
	} __packed;
	
	uint32_t raw;
	
} __aligned(4) rgba8888_t;



#define RGB1555_INITIALIZER( _msb, _r, _g, _b )		(rgb1555_t){ (_msb), (_b), (_g), (_r) }
#define RGB1555( _msb, _r, _g, _b )					\
	RGB1555_INITIALIZER(_msb, _r, _g, _b)


#define RGBA8888_INITIALIZER( _a, _r, _g, _b )		(rgba8888_t){ (_a), (_b), (_g), (_r) }




static inline rgb1555_t RGBA8888_to_RGB1555(rgba8888_t col)
{
	return RGB1555_INITIALIZER(1, (col.r * 31) / 255, (col.g * 31) / 255, (col.b * 31) / 255);
}


static inline rgba8888_t RGB1555_to_RGBA8888(rgb1555_t col)
{
	return RGBA8888_INITIALIZER(255, (col.r * 255) / 31, (col.g * 255) / 31, (col.b * 255) / 31);
}


static inline rgb1555_t Interpolate_RGB1555(rgb1555_t c0, rgb1555_t c1, uint16_t numerator, uint16_t denominator)
{
	return RGB1555_INITIALIZER(
		1, 
		int16_interpolate((c0.r), (c1.r), numerator, denominator),
		int16_interpolate((c0.g), (c1.g), numerator, denominator),
		int16_interpolate((c0.b), (c1.b), numerator, denominator));
}


static inline rgba8888_t Interpolate_RGBA8888(rgba8888_t c0, rgba8888_t c1, uint16_t numerator, uint16_t denominator)
{
	return RGBA8888_INITIALIZER(
		255,
		int16_interpolate(c0.r, c1.r, numerator, denominator),
		int16_interpolate(c0.g, c1.g, numerator, denominator),
		int16_interpolate(c0.b, c1.b, numerator, denominator));
}


#endif
