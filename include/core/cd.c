#include <core/cd.h>


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>





void cdfs_init(void)
{
}



void cdfs_config_set(const cdfs_config_t* config)
{
}


void cdfs_config_default_set(void)
{
}



cdfs_filelist_entry_t* cdfs_entries_alloc(int32_t count)
{
	
	
	return NULL;
}



void cdfs_filelist_init()
{
	
	
}



void cdfs_filelist_root_read()
{
}


int cd_block_init(void)
{
}


int cd_block_sectors_read(fad_t fad, void* output_buffer, uint32_t length)
{
}



void* DEBUG_ReadFile(const char* filename)
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
	
	return buffer;
}
