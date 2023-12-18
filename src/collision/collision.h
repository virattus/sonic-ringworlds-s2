#ifndef __RINGWORLDS_COLLISION_H__
#define __RINGWORLDS_COLLISION_H__


#include <yaul.h>

#include <stdint.h>


typedef struct _COLLINE
{
	fix16_vec3_t start;
	fix16_vec3_t end;
} col_line_t;


typedef struct _COLPLANE
{
	fix16_vec3_t normal;
	fix16_t distAlongNormal;
} col_plane_t;


typedef struct _COLSPHERE
{
	fix16_vec3_t pos;
	fix16_t radius;
	
} col_sphere_t;


typedef struct _COLAABB
{
	fix16_vec3_t min;
	fix16_vec3_t max;
} col_aabb_t;


typedef struct _COLPOLYGON
{
	
	
} col_polygon_t;


#endif
