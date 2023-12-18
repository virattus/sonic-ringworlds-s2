#include <core/colour.h>

#include <gamemath/int_vector.h>

#include <assert.h>


#define SDL_A_MASK			0x000000FF
#define SDL_R_MASK			0xFF000000
#define SDL_G_MASK			0x00FF0000
#define SDL_B_MASK			0x0000FF00

#define RGB1555_MSB_MASK	0x8000
#define RGB1555_R_MASK		31 //0x1F
#define RGB1555_G_MASK		992 //fucking wonky
#define RGB1555_B_MASK		31744 //0x7C00



rgb1555_t RGB1555_from_uint32(uint32_t col)
{
	return RGB1555(1, (col & SDL_R_MASK) >> 27, (col & SDL_G_MASK) >> 19, (col & SDL_B_MASK) >> 11);
}


uint32_t RGB1555_to_uint32(rgb1555_t col)
{
	return (col.r << 27) + (col.g << 19) + (col.b << 11) + (0xFF); 
}


CVECTOR RGB1555_to_CVector(rgb1555_t col)
{
	CVECTOR retval;
	retval.r = (col.raw & RGB1555_R_MASK);
	retval.g = (col.raw & RGB1555_G_MASK) >> 5;
	retval.b = (col.raw & RGB1555_B_MASK) >> 10;
	retval.a = (col.raw & RGB1555_MSB_MASK) >> 15;
	
	return retval;
}


rgb1555_t Interpolate_RGB1555(rgb1555_t c0, rgb1555_t c1, uint16_t numerator, uint16_t denominator)
{
	return RGB1555(
		1, 
		(int16_interpolate((c0.r), (c1.r), numerator, denominator)),
		(int16_interpolate((c0.g), (c1.g), numerator, denominator)),
		(int16_interpolate((c0.b), (c1.b), numerator, denominator)));
}



uint32_t CVector_to_uint32(CVECTOR v0)
{
	return (v0.r << 24) + (v0.g << 16) + (v0.b << 8) + 0xFF;
}


CVECTOR Uint32_to_CVector(uint32_t col)
{	
	CVECTOR retval;
	
	retval.r = (col & SDL_R_MASK) >> 24;
	retval.g = (col & SDL_G_MASK) >> 16;
	retval.b = (col & SDL_B_MASK) >> 8;
	retval.a = (col & SDL_A_MASK);
	
	return retval;
}


rgb1555_t CVector_to_rgb1555(CVECTOR col)
{
	return RGB1555(1, col.r >> 3, col.g >> 3, col.b >> 3);
}




CVECTOR Interpolate_CVECTOR(const CVECTOR* v0, const CVECTOR* v1, uint16_t numerator, uint16_t denominator)
{	
	return (CVECTOR){
		.r = int16_interpolate(v0->r, v1->r, numerator, denominator),
		.g = int16_interpolate(v0->g, v1->g, numerator, denominator),
		.b = int16_interpolate(v0->b, v1->b, numerator, denominator),
	};
}


CVECTOR Add_CVECTOR(CVECTOR v0, CVECTOR v1)
{
	v0.r += v1.r;
	v0.g += v1.g;
	v0.b += v1.b;
	
	return v0;
}


CVECTOR Sub_CVECTOR(CVECTOR v0, CVECTOR v1)
{
	v0.r -= v1.r;
	v0.g -= v1.g;
	v0.b -= v1.b;
}


CVECTOR Scale_CVECTOR(CVECTOR v0, fix16_t multiplier)
{
	v0.r = fix16_to_int(fix16_mul(multiplier, fix16_from_int(v0.r)));
	v0.g = fix16_to_int(fix16_mul(multiplier, fix16_from_int(v0.g)));
	v0.b = fix16_to_int(fix16_mul(multiplier, fix16_from_int(v0.b)));
	
	return v0;
}


CVECTOR Mul_CVECTOR(const CVECTOR* v0, const CVECTOR* v1)
{
	return (CVECTOR){
		.r = clamp(v0->r * v1->r, 0, 255),
		.g = clamp(v0->g * v1->g, 0, 255),
		.b = clamp(v0->b * v1->b, 0, 255),
		.a = 255,
	};
	//v0.r = fix16_to_int(fix16_mul(fix16_from_int(v0.r), fix16_from_int(v1.r)));
	//v0.g = fix16_to_int(fix16_mul(fix16_from_int(v0.g), fix16_from_int(v1.g)));
	//v0.b = fix16_to_int(fix16_mul(fix16_from_int(v0.b), fix16_from_int(v1.b)));
	
	//return v0;
}


CVECTOR Div_CVECTOR(CVECTOR v0, fix16_t divisor)
{
	assert(divisor != 0);
	
	v0.r = fix16_to_int(fix16_div(fix16_from_int(v0.r), divisor));
	v0.g = fix16_to_int(fix16_div(fix16_from_int(v0.g), divisor));
	v0.b = fix16_to_int(fix16_div(fix16_from_int(v0.b), divisor));
	
	return v0;
}
