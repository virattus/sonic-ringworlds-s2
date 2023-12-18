#ifndef __RINGWORLDS_OCTREE_H__
#define __RINGWORLDS_OCTREE_H__



#include "collision.h"


#include <yaul.h>

#include <stdint.h>


#define MAX_OCTREE_NODES (4096)



typedef struct OCTREE_NODE
{
	int16_t parentID;
	int16_t PDATAid; //contains leaf vs node state?
	int16_t children[8];
	uint32_t PVSIndex;
	uint32_t tick;
	col_aabb_t boundingVolume;
	int16_t EntitiesCount;
	void* entities; //void pointer for now
	
} octree_node_t;



#endif
