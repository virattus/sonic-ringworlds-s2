#ifndef __FAKEYAUL_MIC3D_SORT_H__
#define __FAKEYAUL_MIC3D_SORT_H__



#include <stdint.h>



typedef struct _sort_single
{
	int16_t next_single;
} sort_single_t;


typedef struct _sort_list
{
	uint16_t head;
} sort_list_t;


typedef struct _sort
{
	sort_single_t* singles_pool;
	sort_list_t* sort_lists_pool;
	
	sort_single_t* singles_top;
	uint32_t singles_index;
	uint32_t max_depth;
} sort_t;


void __sort_init(void);
void __sort_reset(void);

void __sort_insert(uint32_t z);
void __sort_iterate(void);


#endif
