#ifndef __FAKEYAUL_ANGLE_H__
#define __FAKEYAUL_ANGLE_H__


#include <stdint.h>

//ANGLES ARE JUST fix16 but clamped to the decimal, 1.0 = 360!
//But they're stored as signed shorts, so 180 is negative
typedef int16_t angle_t;

#define DEG2ANGLE(d) ((angle_t)((65536.0 * (d)) / 360.0))


#endif
