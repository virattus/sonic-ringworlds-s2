#ifndef __FAKEYAUL_COLOUR_H__
#define __FAKEYAUL_COLOUR_H__


#include <gamemath/vector.h>

#include <stdint.h>

#define RGB1555(_msb, _r, _g, _b)  (rgb1555_t){ _msb, _b, _g, _r }

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


typedef struct _CVECTOR
{
	uint8_t r, g, b, a;
} CVECTOR;


rgb1555_t RGB1555_from_uint32(uint32_t col);
uint32_t RGB1555_to_uint32(rgb1555_t col);
CVECTOR RGB1555_to_CVector(rgb1555_t col);

rgb1555_t Interpolate_RGB1555(rgb1555_t c0, rgb1555_t c1, uint16_t numerator, uint16_t denominator);


uint32_t CVector_to_uint32(CVECTOR col);
CVECTOR Uint32_to_CVector(uint32_t col);
rgb1555_t CVector_to_rgb1555(CVECTOR col);


CVECTOR Interpolate_CVECTOR(const CVECTOR* v0, const CVECTOR* v1, uint16_t numerator, uint16_t denominator);

CVECTOR Add_CVECTOR(CVECTOR v0, CVECTOR v1);
CVECTOR Sub_CVECTOR(CVECTOR v0, CVECTOR v1);
CVECTOR Scale_CVECTOR(CVECTOR v0, fix16_t multiplier);
CVECTOR Mul_CVECTOR(const CVECTOR* v0, const CVECTOR* v1);
CVECTOR Div_CVECTOR(CVECTOR v0, fix16_t divisor);


#endif
