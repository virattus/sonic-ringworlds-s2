#ifndef __RINGWORLDS_OBJECT_COMPONENTS_H__
#define __RINGWORLDS_OBJECT_COMPONENTS_H__


#include <yaul.h>

#include <mic3d.h>


typedef struct _spatial_component
{
	fix16_vec3_t position;
	fix16_vec3_t velocity;
	
} spatial_com_t;


typedef struct _sprite_component
{
	
	
} sprite_com_t;


typedef struct _mesh_component
{
	mesh_t* pMesh;
	fix16_mat43_t matrix;
	
} mesh_com_t;


typedef struct _model_component
{
	
	
} model_com_t;


typedef struct _animated_model_component
{
	
	
} anim_model_com_t;


void Spatial_Update(spatial_com_t* _spatial);
void Mesh_Update(mesh_com_t* _mesh, const fix16_vec3_t* _pos, const fix16_vec3_t* _rot);


void Mesh_Draw(const mesh_com_t* _mesh);


#endif
