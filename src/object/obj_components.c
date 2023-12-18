#include "obj_components.h"




void Spatial_Update(spatial_com_t* _spatial)
{
	fix16_vec3_add(&_spatial->position, &_spatial->velocity, &_spatial->position);
}


void Mesh_Update(mesh_com_t* _mesh, const fix16_vec3_t* _pos, const fix16_vec3_t* _rot)
{
	fix16_mat33_identity(&_mesh->matrix.rotation);
	
	fix16_mat33_z_rotate(&_mesh->matrix.rotation, _rot->z, &_mesh->matrix.rotation);
	fix16_mat33_y_rotate(&_mesh->matrix.rotation, _rot->y, &_mesh->matrix.rotation);
	fix16_mat33_x_rotate(&_mesh->matrix.rotation, _rot->x, &_mesh->matrix.rotation);
	
	fix16_vec3_dup(_pos, &_mesh->matrix.translation);
}


void Mesh_Draw(const mesh_com_t* _mesh)
{
	render_mesh_xform(_mesh->pMesh, &_mesh->matrix);
}
