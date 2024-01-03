#ifndef __FAKEYAUL_MIC3D_MESH_H__
#define __FAKEYAUL_MIC3D_MESH_H__


#include <gamemath/vector.h>
#include <gamemath/mat43.h>
#include <gfx/vdp1/vdp1_cmdt.h>


typedef struct _MESH
{
	uint32_t polygons_count;
	uint32_t points_count;
	fix16_vec3_t* points;
	fix16_vec3_t* normals;
	uint16_t* Colours;
	attribute_t* attributes;
	polygon_t* polygons;
	
} __aligned(4) mesh_t;


mesh_t* mesh_LoadFromFile(const char* filename, int32_t* count);


#endif
