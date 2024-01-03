#ifndef __FAKEYAUL_RENDER_DISPLAYLIST_H__
#define __FAKEYAUL_RENDER_DISPLAYLIST_H__


#include <gfx/render/primitive.h>


#define PRIMITIVE_MAX 2048


typedef struct DISPLAYLIST_DATA
{
	uint32_t PrimitiveCount;
	Primitive_t PrimitiveList[PRIMITIVE_MAX];
	
} displaylist_t;


void DisplayList_AddPrimitive(displaylist_t* dl, const Primitive_t* p);

void DisplayList_WriteToFile(const displaylist_t* dl, const char* filename);

#endif
