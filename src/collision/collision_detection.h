#ifndef __RINGWORLDS_COLLISION_ALGO_H__
#define __RINGWORLDS_COLLISION_ALGO_H__



#include "collision.h"

#include <stdint.h>



int32_t Collision_Point_Sphere(fix16_vec3_t* p0, col_sphere_t* s0);
int32_t Collision_Point_AABB(fix16_vec3_t* p0, col_aabb_t* a0);

int32_t Collision_Line_Sphere(col_line_t* l0, col_sphere_t* s0);
int32_t Collision_Line_AABB(col_line_t* l0, col_aabb_t* a0);
int32_t Collision_Line_Polygon();

int32_t Collision_Plane_Sphere(col_plane_t* p0, col_sphere_t* s0);

int32_t Collision_Sphere_Sphere(col_sphere_t* s0, col_sphere_t* s1);
int32_t Collision_Sphere_AABB(col_sphere_t* s0, col_aabb_t* a0);
int32_t Collision_Sphere_Polygon();

int32_t Collision_AABB_AABB(col_aabb_t* a0, col_aabb_t* a1);
int32_t Collision_AABB_Polygon();


#endif
