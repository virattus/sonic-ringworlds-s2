#ifndef __FAKEYAUL_RENDER_MESH_H__
#define __FAKEYAUL_RENDER_MESH_H__


#include <gfx/render/render.h>


void render_mesh_xform(mesh_t* mesh, const fix16_mat43_t* world_mat);


void _perspective_transform(void);
void _orthogonal_transform(void);


#endif
