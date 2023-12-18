#ifndef __RINGWORLDS_SSV_H__
#define __RINGWORLDS_SSV_H__


#include <yaul.h>
#include <mic3d.h>


#include <stdint.h>


// For now, let's try making the definition more explicit 

//#define X	(0)
//#define Y	(1)
//#define Z	(2)
//#define XYZ (3)


//typedef int16_t fix8_vec3_t[XYZ];
typedef struct fix8_vec3
{
	int16_t x;
	int16_t y;
	int16_t z;
	
} fix8_vec3_t;


typedef struct SSV_FACE
{
	uint16_t dir;
	uint16_t point1;
	uint16_t point2;
	uint16_t point3;
	uint16_t point4;
	uint16_t texture_id;
	
} ssv_face_t;


typedef struct SSV_MODEL
{
	uint16_t points_count; 		//PTC
	uint16_t polygons_count; 	//FC
	uint16_t frame_count; 		//FRC
	uint16_t texture_offset; 	//TexOffset
	fix8_vec3_t* fix8_points; 	//PNTBL
	ssv_face_t* face_entries; 	//FINDX
	int8_vec3_t* normals; 		//NINDX 
	
} ssv_mesh_t;



void SSV_LoadFromMemory(ssv_mesh_t* mesh, void* ptrData);



void SSV_DrawMeshAnim(const ssv_mesh_t* mesh, const fix16_mat43_t* worldMatrix, fix16_t frameTime);


#endif
