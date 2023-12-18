#include "object.h"

#include "object_types.h"

#include "ring/ring.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>



static game_object_t TemporaryObjects[TEMP_OBJECT_MAX];



void AddObject(game_object_t* obj)
{
	assert(obj != NULL);
	
	for(int i = 0; i < TEMP_OBJECT_MAX; i++)
	{
		if(TemporaryObjects[i].objectType == OBJECT_NULL)
		{
			TemporaryObjects[i].objectType = obj->objectType;
			TemporaryObjects[i].data = obj->data;
			return;
		}
	}
	
	printf("ERROR: failed to add object to temporary objects table\n");
	assert(0);
}



void UpdateTemporaryObjects(void)
{
	for(int i = 0; i < TEMP_OBJECT_MAX; i++)
	{
		if(!UpdateObject(&TemporaryObjects[i]))
			TemporaryObjects[i].objectType = OBJECT_NULL;
	}
}


/*
 * This function will handle updating every type of object that exists
 * in the game. There's probably a better way of handing off the object
 * to each function that corresponds to each object type, but I'm not
 * gonna worry about it right now
 */
bool UpdateObject(game_object_t* obj)
{
	switch(obj->objectType)
	{
		case OBJECT_NULL:
			return true;
		break;
		case OBJECT_RING:
			return UpdateRing(obj->data);
		break;
		case OBJECT_LOOSE_RING:
			return UpdateLooseRing(obj->data);
		break;
	}
	
	return true;
}
