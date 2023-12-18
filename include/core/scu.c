#include <core/scu.h>

#include <gfx/vdp1/vdp1.h>
#include <gfx/vdp2/vdp2.h>
#include <gfx/vram.h>


void scu_dma_transfer(scu_dma_level_t level, void* dst, const void* src, size_t len)
{
	//Looks like this will get used for a lot, but right now we're just going to use it to handle
	//uploading textures to the vdps
	
	
}


void scu_dma_transfer_wait(scu_dma_level_t level)
{
	//do nothing for now
}
