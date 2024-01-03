#ifndef __FAKEYAUL_COLOUR_H__
#define __FAKEYAUL_COLOUR_H__


#include <core/preprocessor.h>
#include <gamemath/vector.h>

#include <stdint.h>


#define RGB1555_MSB_MASK	0x8000
#define RGB1555_R_MASK		31 //0x1F
#define RGB1555_G_MASK		992 //fucking wonky
#define RGB1555_B_MASK		31744 //0x7C00


#define RGB1555( _msb, _r, _g, _b )		(rgb1555_t){ (_msb), (_b), (_g), (_r) }
#define CVECTOR( _r, _g, _b, _a )		(CVECTOR){ (_r), (_g), (_b), (_a) }		

union rgb1555
{
	struct
	{
		unsigned int msb:1;
		unsigned int b:5;
		unsigned int g:5;
		unsigned int r:5;
	};
	
	uint16_t raw;
};

typedef union rgb1555 rgb1555_t;


typedef union COLOUR_RGBA8888
{
	struct
	{
		uint8_t a;
		uint8_t b;
		uint8_t g;
		uint8_t r;
	};
	
	uint32_t raw;
	
} rgba8888_t;


typedef struct _CVECTOR
{
	uint8_t r, g, b, a;
} CVECTOR;


static inline rgb1555_t RGB1555_from_uint32(uint32_t col)
{
	return RGB1555(1, (col & SDL_R_MASK) >> 27, (col & SDL_G_MASK) >> 19, (col & SDL_B_MASK) >> 11);
}


static inline uint32_t RGB1555_to_uint32(rgb1555_t col)
{
	return (col.r << 27) + (col.g << 19) + (col.b << 11) + (0xFF);
}


static inline CVECTOR RGB1555_to_CVector(rgb1555_t col)
{
	return CVECTOR((col.raw & RGB1555_R_MASK) * 255 / 31,
		((col.raw & RGB1555_G_MASK) >> 5) * 255 / 31,
		((col.raw & RGB1555_B_MASK) >> 10) * 255 / 31,
		(col.raw & RGB1555_MSB_MASK) >> 15);
}

rgb1555_t Interpolate_RGB1555(rgb1555_t c0, rgb1555_t c1, uint16_t numerator, uint16_t denominator);


static inline uint32_t CVector_to_uint32(CVECTOR v0)
{
	return (v0.r << 24) + (v0.g << 16) + (v0.b << 8) + 0xFF;
}


static inline CVECTOR Uint32_to_CVector(uint32_t col)
{
	return CVECTOR(
		(col & SDL_R_MASK) >> 24,
		(col & SDL_G_MASK) >> 16,
		(col & SDL_B_MASK) >> 8,
		(col & SDL_A_MASK));
}


static inline rgb1555_t CVector_to_rgb1555(CVECTOR col)
{
	return RGB1555(1, col.r >> 3, col.g >> 3, col.b >> 3);
}


CVECTOR Interpolate_CVECTOR(const CVECTOR* v0, const CVECTOR* v1, uint16_t numerator, uint16_t denominator);

CVECTOR Add_CVECTOR(CVECTOR v0, CVECTOR v1);
CVECTOR Sub_CVECTOR(CVECTOR v0, CVECTOR v1);
CVECTOR Scale_CVECTOR(CVECTOR v0, fix16_t multiplier);
CVECTOR Mul_CVECTOR(const CVECTOR* v0, const CVECTOR* v1);
CVECTOR Div_CVECTOR(CVECTOR v0, fix16_t divisor);

CVECTOR CVECTOR_Average(const CVECTOR* v0, const CVECTOR* v1, uint8_t ratio);

#endif
