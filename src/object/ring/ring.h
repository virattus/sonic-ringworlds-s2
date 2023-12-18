#ifndef __RINGWORLDS_RING_H__
#define __RINGWORLDS_RING_H__



#include <stdint.h>
#include <stdbool.h>

/*
 * Rings are a special object, they don't take space in the temporary objects array,
 * instead they have their own structure for maintaining their data. Rings can only 
 * collide with Sonic, and loose rings the level geometry.
 */


typedef struct _obj_ring
{
	
	
} obj_ring_t;


typedef struct _obj_loose_ring
{
	int16_t lifetime;
	
} obj_loose_ring_t;


bool UpdateRing(obj_ring_t* data);

bool UpdateLooseRing(obj_loose_ring_t* data);



#endif
