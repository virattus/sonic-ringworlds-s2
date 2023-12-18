#ifndef __RINGWORLDS_OBJECT_H__
#define __RINGWORLDS_OBJECT_H__


#include <stdint.h>
#include <stdbool.h>


#define TEMP_OBJECT_MAX 256


typedef struct _GAME_OBJECT
{
	uint16_t objectType;
	void* data;
} game_object_t;


void AddObject(game_object_t* obj);
void RemoveObject(game_object_t* obj);

void UpdateTemporaryObjects(void);
bool UpdateObject(game_object_t* obj);



#endif
