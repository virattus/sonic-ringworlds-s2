#ifndef __FAKEYAUL_VDP1_CMDT_LIST_H__
#define __FAKEYAUL_VDP1_CMDT_LIST_H__


#include <gfx/primitive.h>


#define PRIMITIVE_MAX 2048

typedef struct DISPLAYLIST_DATA
{
	uint32_t PrimitiveCount;
	PRIMITIVE PrimitiveList[PRIMITIVE_MAX];
	
} displaylist_t;


void DisplayList_AddPrimitive(displaylist_t* dl, const PRIMITIVE* p);

void DisplayList_Sort(displaylist_t* dl);

void DisplayList_WriteToFile(const displaylist_t* dl, const char* filename);

displaylist_t* DisplayList_VDP1_Get();
void DisplayList_VDP1_AddPrimitive(const PRIMITIVE* p);

#endif
