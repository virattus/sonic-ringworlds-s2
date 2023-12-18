#include <gamemath/fix16_vector.h>

#include <assert.h>
#include <stdio.h>



static inline fix16_t fix16_interpolate(fix16_t f0, fix16_t f1, uint16_t numerator, uint16_t denominator)
{
	return fix16_lerp16(f0, f1, (numerator << 16) / denominator);
}


fix16_t fix16_vec2_dot(const fix16_vec2_t* v0, const fix16_vec2_t* v1)
{
	assert(v0);
	assert(v1);
	
	return fix16_add(fix16_mul(v0->x, v1->x), fix16_mul(v0->y, v1->y));
}


fix16_t fix16_vec2_length(const fix16_vec2_t* v)
{
	assert(v);
	
	return fix16_sqrt( fix16_mul(v->x, v->x) +
						fix16_mul(v->y, v->y));
}


fix16_vec2_t fix16_vec2_interpolate(const fix16_vec2_t* v0, const fix16_vec2_t* v1, uint16_t numerator, uint16_t denominator)
{
	assert(v0);
	assert(v1);
	
	fix16_vec2_t retval = {
		.x = fix16_interpolate(v0->x, v1->x, numerator, denominator),
		.y = fix16_interpolate(v0->y, v1->y, numerator, denominator),
	};
	
	return retval;
}



void fix16_vec3_zero(fix16_vec3_t* dest)
{
	dest->x = 0;
	dest->y = 0;
	dest->z = 0;
}


void fix16_vec3_add(const fix16_vec3_t* v0, const fix16_vec3_t* v1, fix16_vec3_t* dest)
{
	assert(v0);
	assert(v1);
	assert(dest);
	
	dest->x = v0->x + v1->x;
	dest->y = v0->y + v1->y;
	dest->z = v0->z + v1->z;
}


void fix16_vec3_sub(const fix16_vec3_t* v0, const fix16_vec3_t* v1, fix16_vec3_t* dest)
{
	assert(v0);
	assert(v1);
	assert(dest);
	
	dest->x = fix16_sub(v0->x, v1->x);
	dest->y = fix16_sub(v0->y, v1->y);
	dest->z = fix16_sub(v0->z, v1->z);	
}


void fix16_vec3_dup(const fix16_vec3_t* v0, fix16_vec3_t* dest)
{
	dest->x = v0->x;
	dest->y = v0->y;
	dest->z = v0->z;
}


fix16_t fix16_vec3_dot(const fix16_vec3_t* v0, const fix16_vec3_t* v1)
{
	assert(v0);
	assert(v1);
	
	return fix16_mul(v0->x, v1->x) + fix16_mul(v0->y, v1->y) + fix16_mul(v0->z, v1->z);
}


void fix16_vec3_cross(const fix16_vec3_t* v0, const fix16_vec3_t* v1, fix16_vec3_t* dest)
{
	assert(v0);
	assert(v1);
	assert(dest);
	
	dest->x = fix16_mul(v0->y, v1->z) - fix16_mul(v0->z, v1->y);
	dest->y = fix16_mul(v0->z, v1->x) - fix16_mul(v0->x, v1->z);
	dest->z = fix16_mul(v0->x, v1->y) - fix16_mul(v0->y, v1->x);
}


fix16_t fix16_vec3_length(const fix16_vec3_t* v)
{
	assert(v);
	
	return fix16_sqrt(fix16_vec3_sqr_length(v));
}


fix16_t fix16_vec3_sqr_length(const fix16_vec3_t* v)
{
	assert(v);
	
	return fix16_mul(v->x, v->x) +
			fix16_mul(v->y, v->y) +
			fix16_mul(v->z, v->z);
}


void fix16_vec3_normalize(fix16_vec3_t* v)
{
/*
//figure out what this does
    const fix16_t length = fix16_vec3_length(v0);

    cpu_divu_fix16_set(FIX16(1.0), length);

    const fix16_t scale = cpu_divu_quotient_get();

    fix16_vec3_scale(scale, v0);
*/
	
	
	assert(v);
	
	fix16_t magnitude = fix16_vec3_length(v);
	
	v->x = fix16_div(v->x, magnitude);
	v->y = fix16_div(v->y, magnitude);
	v->z = fix16_div(v->z, magnitude);
}


size_t fix16_vec3_str(const fix16_vec3_t* v0, char* buffer, int32_t decimals)
{
    char *buffer_ptr;
    buffer_ptr = buffer;

	char fix16_buffer[64];

    *buffer_ptr++ = '(';
    fix16_to_str(v0->x, fix16_buffer, decimals);
    buffer_ptr += sprintf(buffer_ptr, "%s", fix16_buffer);
    *buffer_ptr++ = ',';
    *buffer_ptr++ = ' ';

    fix16_to_str(v0->y, fix16_buffer, decimals);
    buffer_ptr += sprintf(buffer_ptr, "%s", fix16_buffer);
    *buffer_ptr++ = ',';
    *buffer_ptr++ = ' ';

    fix16_to_str(v0->z, fix16_buffer, decimals);
    buffer_ptr += sprintf(buffer_ptr, "%s", fix16_buffer);
    *buffer_ptr++ = ')';

    *buffer_ptr = '\0';

    return (buffer_ptr - buffer);
}




/*
__BEGIN_ASM
static inline int32_t __always_inline
fix16_high_mul(fix16_t a, fix16_t b)
{
    __register int16_t out;

    __declare_asm("\tdmuls.l %[a], %[b]\n"
                  "\tsts mach, %[out]\n"
                  // Output
                  : [out] "=&r" (out)
                  // Input
                  : [a] "r" (a),
                    [b] "r" (b)
                  : "mach");

    return out;
}
__END_ASM
 */

//can confirm, this just spits out the answer as an int
//might need to do the math manually so that we have the whole 32 bit range
//math accomplished
int32_t fix16_high_mul(fix16_t a, fix16_t b)
{
	return (a >> 16) * (b >> 16);
}


/*
__BEGIN_ASM
static inline fix16_t __always_inline
fix16_mul(fix16_t a, fix16_t b)
{
    __register uint32_t mach;
    __register fix16_t out;

    __declare_asm("\tdmuls.l %[a], %[b]\n"
                  "\tsts mach, %[mach]\n"
                  "\tsts macl, %[out]\n"
                  "\txtrct %[mach], %[out]\n"
                  // Output
                  : [mach] "=&r" (mach),
                    [out] "=&r" (out)
                  // Input
                  : [a] "r" (a),
                    [b] "r" (b)
                  : "mach", "macl");

    return out;
}
__END_ASM
*/

//This returns the product of the multiple of the decimal of the two inputs
int32_t fix16_low_mul(fix16_t a, fix16_t b)
{
	return (a & 0x0000FFFF) * (b & 0x0000FFFF);
}
