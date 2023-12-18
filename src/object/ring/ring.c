#include "ring.h"



bool UpdateRing(obj_ring_t* data)
{
}

bool UpdateLooseRing(obj_loose_ring_t* data)
{
	if(data->lifetime <= 0)
		return false;
	
	data->lifetime--;
	
	return true;
}
