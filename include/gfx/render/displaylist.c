#include "displaylist.h"


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


void DisplayList_AddPrimitive(displaylist_t* dl, const Primitive_t* p)
{	
	if(dl->PrimitiveCount < PRIMITIVE_MAX)
	{
		dl->PrimitiveList[dl->PrimitiveCount++] = *p;
		return;
	}
	
	printf("DisplayList_AddPrimitive Error: exceeding max primitive count: %d\n", PRIMITIVE_MAX);
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
		Primitive_t p = dl->PrimitiveList[i];
		
		fprintf(file, "Polygon TextureOffset: %d TextureDimensions: %d\n", p.flags.TextureOffset, p.flags.TextureDimensions);
		fprintf(file, "    X0: %d Y0: %d -- X1: %d Y1: %d\n", 
			p.v[0].x, p.v[0].y, p.v[1].x, p.v[1].y);

		fprintf(file, "    X3: %d Y3: %d -- X2: %d Y2: %d\n\n", 
			p.v[2].x, p.v[2].y, p.v[3].x, p.v[3].y);
	}
	
	
	fclose(file);
}
