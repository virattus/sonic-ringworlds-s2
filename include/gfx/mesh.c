#include <gfx/mesh.h>

#include <gamemath/matrix.h>
#include <gfx/render/render.h>
#include <gfx/texture.h>
#include <mic3d/state.h>

#include <libfixmath/fix16.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define INDICES_PER_POLY 4
#define HEADER_OFFSET 8
#define HEADER_BYTE_SIZE 28

#define SATURN_POINTS_SIZE 12
#define SATURN_NORMALS_SIZE 12
#define SATURN_COLOURS_SIZE 2
#define SATURN_ATTRIBUTE_SIZE 12
#define SATURN_POLYGON_SIZE 10


#define MESH_MAGIC_V2 0xDEADBEEF
#define MESH_MAGIC_V3 0xABBAABBA
#define MESH_MAGIC_V4 0x12345678
#define MESH_MAGIC_V5 0x12211535

#define MESH_MAGIC_CURRENT MESH_MAGIC_V5


typedef struct _modelHeader
{
	uint32_t magic;
	int32_t mesh_count;
} model_header_t;


typedef struct _meshHeader
{
	int32_t points_count;
	int32_t polygons_count;
	int32_t vertexOffset;
	int32_t normalOffset;
	int32_t colourOffset;
	int32_t attributeOffset;
	int32_t polygonOffset;
} mesh_header_t;


void* GenerateMeshDataArray(uint8_t* buffer, int32_t offset, int32_t sizeInBytes)
{
	void* data = malloc(sizeInBytes);
	memcpy(data, buffer + offset, sizeInBytes);
	return data;
}


mesh_t* mesh_LoadFromFile(const char* filename, int32_t* count)
{
	FILE* fp = fopen(filename, "rb");
	if(fp == NULL)
	{
		printf("failed to open file %s\n", filename);
		return NULL;
	}
	
	fseek(fp, 0, SEEK_END);
	long siz = ftell(fp);
	//printf("size of file is %ld\n", siz);
	rewind(fp);
	uint8_t* buffer = (uint8_t*)malloc(siz + 1);
	//printf("preparing to read file\n");
	if(fread(buffer, 1, siz, fp) == 0)
	{
		printf("Failed to read file into memory\n");
	}
	
	fclose(fp);
	
	
	uint32_t magic = buffer[0];
	
	memcpy(&magic, buffer, sizeof(uint32_t));
	
	if(magic != MESH_MAGIC_CURRENT)
	{
		printf("Failed to read magic of file correctly: %d\nIt's either the wrong file, or an old format.\n", magic);
		free(buffer);
		return NULL;
	}
	
	
	uint32_t meshCount = 0;
	memcpy(&meshCount, buffer + 4, 4);
	
	printf("number of meshes: %d\n", meshCount);
	
	*count = meshCount;
	
	mesh_t* meshPtr = (mesh_t*)malloc(sizeof(mesh_t) * meshCount); 
	
	assert(sizeof(mesh_header_t) == 28);

	for(uint32_t i = 0; i < meshCount; i++)
	{
		mesh_header_t header;
		memcpy(&header, buffer + (sizeof(mesh_header_t) * i) + HEADER_OFFSET, sizeof(mesh_header_t));
		
		meshPtr[i].polygons_count = header.polygons_count;
		meshPtr[i].points_count = header.points_count;
		
		//fix16_vec3_t* points = (fix16_vec3_t*)malloc(sizeof(fix16_vec3_t) * header.points_count);
		//memcpy(points, (buffer + header.vertexOffset), sizeof(fix16_vec3_t) * header.points_count);
		//meshPtr[i].points = points;
		meshPtr[i].points = (fix16_vec3_t*)GenerateMeshDataArray(buffer, header.vertexOffset, sizeof(fix16_vec3_t) * header.points_count);

		//fix16_vec3_t* normals = (fix16_vec3_t*)malloc(sizeof(fix16_vec3_t) * header.points_count);
		//memcpy(normals, (buffer + header.normalOffset), sizeof(fix16_vec3_t) * header.points_count);		
		//meshPtr[i].normals = normals;
		meshPtr[i].normals = (fix16_vec3_t*)GenerateMeshDataArray(buffer, header.normalOffset, sizeof(fix16_vec3_t) * header.points_count);
		
		if(header.colourOffset != -1)
		{			
			//meshPtr[i].Colours = (uint16_t*)(buffer + colourOffset);
		}
		else
		{
			meshPtr[i].Colours = NULL;
		}
		
		//attribute_t* attr = (attribute_t*)malloc(sizeof(attribute_t) * header.points_count);
		//memcpy(attr, (buffer + header.attributeOffset), sizeof(attribute_t) * header.points_count);				
		//meshPtr[i].attributes = attr;
		meshPtr[i].attributes = (attribute_t*)GenerateMeshDataArray(buffer, header.attributeOffset, sizeof(attribute_t) * header.polygons_count);
		/*
		attribute_t* attributes = (attribute_t*)malloc(sizeof(attribute_t) * header.polygons_count);
		for(int i = 0; i < header.polygons_count; i++)
		{
			uint16_t* attrPointer = (uint16_t*)(buffer + header.attributeOffset + (i * sizeof(attribute_t)));
			attributes[i].control.raw = *(uint16_t*)(attrPointer);
			attributes[i].draw_mode.raw = *(uint16_t*)(attrPointer + 2);
			attributes[i].palette_data.raw = *(uint16_t*)(attrPointer + 4);
			attributes[i].texture_slot = *(uint16_t*)(attrPointer + 6);
			attributes[i].shading_slot = *(uint16_t*)(attrPointer + 8);
		} 
		meshPtr[i].attributes = attributes;
		*/
		
		//HANDLE POLYGONS
		polygon_t* polygons = (polygon_t*)malloc(sizeof(polygon_t) * header.polygons_count);
		memset(polygons, 0xFFFFFFFF, sizeof(polygon_t) * header.polygons_count);
		
		printf("mesh import: polygon count: %d\n", meshPtr[i].polygons_count);
		for(int i = 0; i < header.polygons_count; i++)
		{
			//polygons are stored as 10 bytes, but they take 12 on x86
			uint8_t* polyPointer = (uint8_t*)(buffer + header.polygonOffset + (i * SATURN_POLYGON_SIZE));
			polygons[i].flags = *(flags_t*)(polyPointer);
			polygons[i].indices.p0 = *(uint16_t*)(polyPointer + 2);
			polygons[i].indices.p1 = *(uint16_t*)(polyPointer + 4);
			polygons[i].indices.p2 = *(uint16_t*)(polyPointer + 6);
			polygons[i].indices.p3 = *(uint16_t*)(polyPointer + 8);			
			
			//memcpy(polygons + (sizeof(polygon_t) * i), polyPointer, SATURN_POLYGON_SIZE);
		}
		meshPtr[i].polygons = polygons;
		
		printf("Poly Count: %d Vertex Count: %d, VertOffset %d NormalOffset %d ColourOffset %d AttributeOffset %d PolygonOffset %d\n", 
			meshPtr[i].polygons_count, 
			meshPtr[i].points_count,
			header.vertexOffset, 
			header.normalOffset, 
			header.colourOffset, 
			header.attributeOffset,
			header.polygonOffset);
		
	}
	
	
	//printf("got to end without crashing?\n");
	free(buffer);
	
	return meshPtr;
}

