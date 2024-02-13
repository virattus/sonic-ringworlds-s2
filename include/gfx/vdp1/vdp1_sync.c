#include <gfx/vdp1/vdp1_sync.h>

#include <core/colour.h>
#include <gfx/vdp1/vdp1.h>
#include <gfx/vdp1/vdp1_vram.h>
#include <gfx/vdp1/vdp1_render.h>


#include <assert.h>
#include <string.h>




void vdp_sync_vblank_out_set(callback_handler_t callback_handler, void* work)
{
}


void vdp1_sync_interval_set(int8_t interval)
{
}


void vdp1_sync_mode_set(vdp_sync_mode_t mode)
{
}


void vdp1_sync_cmdt_list_put(vdp1_cmdt_list_t* list, uint16_t index)
{
	assert(list != NULL);
	
	vdp1_sync_cmdt_put(list->cmdts, list->count, index);
}


void vdp1_sync_cmdt_put(const vdp1_cmdt_t* cmdts, uint16_t count, uint16_t index)
{
	assert(cmdts != NULL);
	
	if(count == 0)
	{
		return;
	}
	
	//this looks to be just for making sure we don't clobber the rendering data
	//_vdp1_sync_put();
	
	//regular dma transfer like you'd think
	//Index is indeed an offset into the cmdt list
	//index looks like it's a byte offset, so might need to do some math
	
	vdp1_vram_partitions_t partitions = _state_vdp1()->vram_partitions;
	
	vdp1_cmdt_t* cmdt_p = partitions.cmdt_base + index;
	
	//memcpy(partitions.cmdt_base + index, cmdts, count * sizeof(vdp1_cmdt_t));
	
	for(int i = 0; i < count; i++)
	{
		cmdt_p[i] = cmdts[i];
	}
	
}


void vdp1_sync_force_put(void)
{
}


void vdp1_sync_render()
{
	VDP1_Render();
}


void vdp1_sync()
{
	if(VDP1_IsRendering())
	{
		VDP1_WaitThreads();
	}
}


void vdp1_sync_wait()
{
	vdp1_sync();
}
