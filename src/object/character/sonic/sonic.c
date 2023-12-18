#include "sonic.h"

#define FRAME_RATE 	500
#define GRAVITY 	FIX16(9.8 / FRAME_RATE)


#include <yaul.h>
#include <mic3d.h>


void Sonic_Update(char_sonic_t* _sonic)
{
	//Spatial_Update(&_sonic->spatial);
	
	if(_sonic->spatial.position.y < 0)
		_sonic->spatial.velocity.y += GRAVITY;
	else
		_sonic->spatial.velocity.y -= GRAVITY;
		
	fix16_vec3_t rot_t = { 0, _sonic->rot, 0 };
		
	Mesh_Update(&_sonic->mesh, &_sonic->spatial.position, &rot_t);
	
	_sonic->rot += DEG2ANGLE(0.1);
}



void Sonic_Draw(char_sonic_t* _sonic)
{
	Mesh_Draw(&_sonic->mesh);
}
