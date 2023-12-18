#include <gfx/texture.h>

#include <gamemath/vector.h>
#include <gfx/render/render.h>
#include <gfx/vram.h>
#include <gfx/vdp1/vdp1.h>
#include <mic3d/state.h>

#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#define BUFFER_SIZE 1024
int32_t LinesInFile(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if(!file)
	{
		printf("failed to open file: %s\n", filename);
		return -1;
	}
	
	char* buffer = (char*)malloc(BUFFER_SIZE);
	
	int counter = 0;
	
	while(fscanf(file, "%s", buffer) != EOF)
	{
		if(strlen(buffer) > 3)
		{
			counter++;
		}
	}
	
	fclose(file);
	
	free(buffer);

	return counter;
}


char* GetRootDirectory(const char* filename)
{
	int nameLength = strlen(filename);
	char* Root = (char*)malloc(nameLength);

	int delim = 0;	
	for(int i = 0; i < nameLength; i++)
	{
		if(filename[i] == '/')
		{
			delim = i;
		}
	}

	//printf("string of length %d - delim found at %d\n", nameLength, delim);

	memcpy(Root, filename, delim);
	Root[delim] = '\0';
	
	return Root;
}


texture_t* Texture_LoadFromFile(const char* texListFile, int32_t* texCount)
{	
	printf("loading textures related to %s\n", texListFile);
	
	size_t filenameLen = strlen(texListFile) + 10;
	char* TextureFile = (char*)malloc(filenameLen);
	sprintf(TextureFile, "%s.textures", texListFile);
	
	int32_t lineCount = LinesInFile(TextureFile);
	
	texture_t* texArray = (texture_t*)malloc(sizeof(texture_t) * lineCount);
	
	
	FILE* handler = fopen(TextureFile, "r");
	if(!handler)
	{
		printf("Failed to open texture descriptor file: %s\n", texListFile);
		free(texArray);
		return NULL;
	}
	
	char* TextureRoot = GetRootDirectory(texListFile);
	printf("texture root directory: %s\n", TextureRoot);
	
	uint32_t offset = 0;
	uint32_t texID = 0;
	
	char* buffer = (char*)malloc(256);
	int string_size;
	
	if(handler)
	{
		char* texPath = (char*)malloc(256);
		while(fscanf(handler, "%s", buffer) != EOF)
		{
			sprintf(texPath, "%s/%s", TextureRoot, buffer);
			printf("%s\n", texPath);
			
			texArray[texID].vram_index = offset;
			offset += TextureMapper_WriteToVRAM(VDP1_GetTextureBuffer(), offset, &texArray[texID].size, texPath);
			
			int16_vec2_t texSize = TEXTURE_SIZE_TO_VEC(texArray[texID].size);
			printf("Texture %s ID %d offset: %d dimensions: %d %d\n", texPath, texID, texArray[texID].vram_index, texSize.x, texSize.y);
			
			texID++;
		}
		free(texPath);
	}
	
	fclose(handler);
	
	free(TextureRoot);
	free(buffer);
	
	*texCount = lineCount;
	return texArray;
}


uint32_t TextureMapper_GetVRAMOffset(const texture_t* tex, const int16_vec2_t* uv)
{
	//printf("tex size: %d %d UVS: %d %d\n", tex->size.x, tex->size.y, uv->x, uv->y);
	
	int16_vec2_t texSize = TEXTURE_SIZE_TO_VEC(tex->size);
	
	assert(uv->x >= 0);
	assert(uv->x < texSize.x);
	assert(uv->y >= 0);
	assert(uv->y < texSize.y);
	
	return (tex->vram_index + (texSize.x * uv->y + uv->x));
}


uint32_t TextureMapper_WriteToVRAM(VRAM* ram, uint32_t offset, int16_t* dimensions, const char* filename)
{
	SDL_Surface* newtex = IMG_Load(filename);
	if(newtex == NULL)
	{
		printf("Failed to load texture %s with error: %s\n", filename, SDL_GetError());
		assert(false);
		return 0;
	}
	
	SDL_Surface* oldsurface = newtex;
	
	newtex = SDL_ConvertSurfaceFormat(newtex, SDL_PIXELFORMAT_RGBA8888, 0);
	
	if((newtex->w % 8 != 0) ||
		(newtex->w > 504) ||
		newtex->h > 255)
	{
		printf("TextureMapper_WriteToVRAM: Texture %s Dim X: %d Y: %d Incorrect dimensions\n", filename, newtex->w, newtex->h);
		SDL_FreeSurface(oldsurface);
		SDL_FreeSurface(newtex);
		assert(false);
		return 0;
	}
	
	if((offset + (newtex->w * newtex->h)) > ram->Size)
	{
		printf("Error: Texture %s is too large to fit in VRAM\n", filename);
		SDL_FreeSurface(oldsurface);
		SDL_FreeSurface(newtex);
		assert(false);
		return 0;
	}

	dimensions = TEXTURE_SIZE(newtex->w, newtex->h);
	
	int32_t* mem = (int32_t*)ram->Memory;
	int32_t* pixels = (int32_t*)newtex->pixels;
	for(int i = 0; i < newtex->w * newtex->h; i++)
	{
		*(mem + offset + i) = *(pixels + i);
	}
		
	//memcpy(ram->Memory + offset, newtex->pixels, dimensions->x * dimensions->y * 4); 
	
	SDL_FreeSurface(oldsurface);
	SDL_FreeSurface(newtex);
	
	return (newtex->w * newtex->h);
}


void tlist_set(texture_t* textures, uint16_t count)
{	
	__state.tlist->textures = textures;
	__state.tlist->count = count;
}



void TextureMapper_Init()
{
}
