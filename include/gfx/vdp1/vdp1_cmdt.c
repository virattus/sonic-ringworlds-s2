#include <gfx/vdp1/vdp1_cmdt.h>


#include <assert.h>
#include <stdlib.h>
#include <string.h>


vdp1_cmdt_list_t* vdp1_cmdt_list_alloc(uint16_t count)
{
	assert(count > 0);
	
	vdp1_cmdt_list_t* cmdt_list;
	cmdt_list = malloc(sizeof(vdp1_cmdt_list_t));
	assert(cmdt_list != NULL);
	
	//the commands are created with memalign, look into this for PC
	vdp1_cmdt_t* cmdts;
	cmdts = malloc(sizeof(vdp1_cmdt_t) * count);
	assert(cmdts != NULL);
	
	vdp1_cmdt_list_init(cmdt_list, cmdts);
	
	return cmdt_list;
}


void vdp1_cmdt_list_free(vdp1_cmdt_list_t* cmdt_list)
{
	assert(cmdt_list != NULL);
	assert(cmdt_list->cmdts != NULL);
	
	cmdt_list->count = 0;
	free(cmdt_list->cmdts);
	free(cmdt_list);
}


void vdp1_cmdt_list_init(vdp1_cmdt_list_t* cmdt_list, vdp1_cmdt_t* cmdts)
{
	assert(cmdt_list != NULL);
	assert(cmdts != NULL);
	
	cmdt_list->cmdts = cmdts;
	cmdt_list->count = 0;
}
