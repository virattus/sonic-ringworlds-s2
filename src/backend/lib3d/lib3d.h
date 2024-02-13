#ifndef __FAKEYAUL_LIB3D_H__
#define __FAKEYAUL_LIB3D_H__


#ifdef PC_BUILD
	#include <gamemath/mat43.h>
	#include <mic3d/mesh.h>
#else
#endif


void mesh_xform(mesh_t* mesh, const fix16_mat43_t* world_mat);



#endif
