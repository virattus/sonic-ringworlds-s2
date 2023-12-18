#include <gfx/vdp1/vdp1_displaylist.h>


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>



static displaylist_t vdp1_dl;


static int PrimSort(const void* p0, const void* p1)
{
	assert(p0 != NULL);
	assert(p1 != NULL);
	
	if(p0 == NULL || p1 == NULL)
	{
		return 0;
	}
	
	int16_t z0 = ((PRIMITIVE*)p0)->zSort;
	int16_t z1 = ((PRIMITIVE*)p1)->zSort;
	
	if(z0 > z1) return  1;
	if(z0 < z1) return -1;
	
	return 0;
}


void DisplayList_AddPrimitive(displaylist_t* dl, const PRIMITIVE* p)
{	
	if(dl->PrimitiveCount < PRIMITIVE_MAX)
	{
		dl->PrimitiveList[dl->PrimitiveCount++] = *p;
		return;
	}
	
	printf("DisplayList_AddPrimitive Error: exceeding max primitive count: %d\n", PRIMITIVE_MAX);
}


void DisplayList_Sort(displaylist_t* dl)
{
	if(dl->PrimitiveCount > 0)
	{
		qsort(dl->PrimitiveList, dl->PrimitiveCount, sizeof(PRIMITIVE), PrimSort);
	}
}


void DisplayList_WriteToFile(const displaylist_t* dl, const char* filename)
{
	FILE* file = fopen(filename, "w");
	if(!file)
	{
		printf("failed to open file for writing: %s\n", filename);
		return;
	}
	
	//TODO rewrite this to support additional primitive types
	printf("writing out %u primitives\n", dl->PrimitiveCount);
	for(int i = 0; i < dl->PrimitiveCount; i++)
	{
		PRIMITIVE p = dl->PrimitiveList[i];
		
		fprintf(file, "Polygon TextureID: %d ZSort: %d\n", p.flags.textureID, p.zSort);
		fprintf(file, "    X0: %d Y0: %d -- X1: %d Y1: %d\n", 
			p.v[0].pos.x, p.v[0].pos.y, p.v[1].pos.x, p.v[1].pos.y);

		fprintf(file, "    X3: %d Y3: %d -- X2: %d Y2: %d\n\n", 
			p.v[2].pos.x, p.v[2].pos.y, p.v[3].pos.x, p.v[3].pos.y);
	}
	
	
	fclose(file);
}


displaylist_t* DisplayList_VDP1_Get()
{
	return &vdp1_dl;
}


void DisplayList_VDP1_AddPrimitive(const PRIMITIVE* p)
{
	DisplayList_AddPrimitive(&vdp1_dl, p);
}
