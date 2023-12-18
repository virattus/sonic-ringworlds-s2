#ifndef __FAKEYAUL_CD_H__
#define __FAKEYAUL_CD_H__



#include <stdint.h>
#include <stddef.h>



#define CDFS_SECTOR_SIZE (2048U)

/* The maximum number of file list entries to read */
#define CDFS_FILELIST_ENTRIES_COUNT (4096)

/* CDFS limitations */
#define ISO_DIR_LEVEL_MAX       	8
#define ISO_FILENAME_MAX_LENGTH 	11


typedef uint32_t sector_t;
typedef uint32_t fad_t;

typedef union
{
	uint32_t buffer32[CDFS_SECTOR_SIZE / sizeof(uint32_t)];
	uint16_t buffer16[CDFS_SECTOR_SIZE / sizeof(uint32_t)];
	uint8_t buffer8[CDFS_SECTOR_SIZE];
	
} sector_buffer_t;


typedef enum cdfs_entry_type 
{
    CDFS_ENTRY_TYPE_INVALID   = 0,
    CDFS_ENTRY_TYPE_FILE      = 1,
    CDFS_ENTRY_TYPE_DIRECTORY = 2
    
} cdfs_entry_type_t;


typedef struct CD_FILE_ENTRY
{
    cdfs_entry_type_t type;
    char name[ISO_FILENAME_MAX_LENGTH + 1];
    fad_t starting_fad;
    size_t size;
    uint16_t sector_count;
    
} cdfs_filelist_entry_t; //28 bytes


typedef struct CD_FILELIST
{
    cdfs_filelist_entry_t* entries;
    uint32_t entries_pooled_count;
    uint32_t entries_count;
	
} cdfs_filelist_t;



typedef struct CD_CONFIG
{
	
	
} cdfs_config_t;


void cdfs_init(void);

void cdfs_config_set(const cdfs_config_t* config);
void cdfs_config_default_set(void);

cdfs_filelist_entry_t* cdfs_entries_alloc(int32_t count);

void cdfs_filelist_init();

void cdfs_filelist_root_read();


//CD BLOCK SHIT

int cd_block_init(void);
int cd_block_sectors_read(fad_t fad, void* output_buffer, uint32_t length);


void* DEBUG_ReadFile(const char* filename);


#endif
