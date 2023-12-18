#include "collision_detection.h"

#include <yaul.h>


#include <assert.h>



int32_t Collision_Point_Sphere(fix16_vec3_t* p0, col_sphere_t* s0)
{
	assert(p0);
	assert(s0);
	
	fix16_t distance = fix16_sqrt(	fix16_mul(p0->x - s0->pos.x, p0->x - s0->pos.x) + 
									fix16_mul(p0->y - s0->pos.y, p0->y - s0->pos.y) + 
									fix16_mul(p0->z - s0->pos.z, p0->z - s0->pos.z)	);
	
	return distance < s0->radius;
	
}


int32_t Collision_Point_AABB(fix16_vec3_t* p0, col_aabb_t* a0)
{
	assert(p0);
	assert(a0);
	
	return (p0->x <= a0->max.x &&
			p0->x >= a0->min.x &&
			p0->y <= a0->max.y &&
			p0->y >= a0->min.y &&
			p0->z <= a0->max.z &&
			p0->z >= a0->min.z);
}



//https://gamedev.stackexchange.com/questions/96459/fast-ray-sphere-collision-code
int32_t Collision_Line_Sphere(col_line_t* l0, col_sphere_t* s1)
{
	assert(l0);
	assert(s1);
	
	fix16_t distance = 0;
	fix16_vec3_t collision;
	
	fix16_vec3_t m;
	fix16_vec3_add(&l0->start, &s1->pos, &m);
	//fix16_t b = Dot_VECTOR3(&m, 
	fix16_t c = fix16_vec3_dot(&m, &m) - fix16_mul(s1->radius, s1->radius);
	
	
	return 0;
}


int32_t Collision_Line_AABB(col_line_t* l0, col_aabb_t* a1)
{
	assert(l0);
	assert(a1);
	
	return 0;
}


int32_t Collision_Line_Polygon()
{
	
}


int32_t Collision_Plane_Sphere(col_plane_t* p0, col_sphere_t* s0)
{
	
}


int32_t Collision_Sphere_Sphere(col_sphere_t* s0, col_sphere_t* s1)
{
	assert(s0);
	assert(s1);
	
	fix16_t distance = fix16_sqrt(	fix16_mul(s0->pos.x - s1->pos.x, s0->pos.x - s1->pos.x) + 
									fix16_mul(s0->pos.y - s1->pos.y, s0->pos.y - s1->pos.y) + 
									fix16_mul(s0->pos.z - s1->pos.z, s0->pos.z - s1->pos.z)	);
	
	fix16_t radDistance = s0->radius + s1->radius;
	
	return distance < radDistance;
}


//https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_sphere_aabb.html
int32_t Collision_Sphere_AABB(col_sphere_t* s0, col_aabb_t* a1)
{
	assert(s0);
	assert(a1);
	
	fix16_t x = max(a1->min.x, min(s0->pos.x, a1->max.x));
	fix16_t y = max(a1->min.y, min(s0->pos.y, a1->max.y));
	fix16_t z = max(a1->min.z, min(s0->pos.z, a1->max.z));
	
	fix16_t distance = fix16_sqrt(	fix16_mul(x - s0->pos.x, x - s0->pos.x) +
									fix16_mul(y - s0->pos.y, y - s0->pos.y) +
									fix16_mul(z - s0->pos.z, z - s0->pos.z)	);
	
	return distance < s0->radius;
}


int32_t Collision_Sphere_Polygon()
{
}


int32_t Collision_AABB_AABB(col_aabb_t* a0, col_aabb_t* a1)
{
	assert(a0);
	assert(a1);
	
	return (a0->min.x <= a1->max.x &&
			a0->max.x >= a1->min.x &&
			a0->min.y <= a1->max.y &&
			a0->max.y >= a1->min.y &&
			a0->min.z <= a1->max.z &&
			a0->max.z >= a1->min.z);
}


int32_t Collision_AABB_Polygon()
{
}
