#ifndef __RINGWORLDS_SONIC_CHARACTER_H__
#define __RINGWORLDS_SONIC_CHARACTER_H__


#include "../../obj_components.h"

#include <yaul.h>


enum sonic_state
{
	IDLE,
	MOVE,
	JUMP,
	FALL,
	HIT,
	
};


typedef struct _sonic_character
{
	spatial_com_t spatial;
	mesh_com_t mesh;
	angle_t rot;
	
} char_sonic_t;


void Sonic_Update(char_sonic_t* _sonic);

void Sonic_Draw(char_sonic_t* _sonic);



#endif
