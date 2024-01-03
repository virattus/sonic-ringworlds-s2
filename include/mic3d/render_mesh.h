#ifndef __FAKEYAUL_MIC3D_RENDER_MESH_H__
#define __FAKEYAUL_MIC3D_RENDER_MESH_H__


#include <mic3d/render.h>


void render_mesh_xform(mesh_t* mesh, const fix16_mat43_t* world_mat);


void _perspective_transform(void);
void _orthogonal_transform(void);


#endif
