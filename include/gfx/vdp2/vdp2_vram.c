#include <gfx/vdp2/vdp2_vram.h>


#include <gfx/vdp2/vdp2.h>


static vdp2_vram_ctl_t _vdp2_vram_ctl = {
	.coeff_table = VDP2_VRAM_CTL_COEFF_TABLE_VRAM,
	.vram_mode = VDP2_VRAM_CTL_MODE_PART_BANK_BOTH,

};


void __vdp2_vram_init(void)
{
	//Maybe this stuff is unnecessary?
	//_state_vdp2()->vram_ctl = &_vdp2_vram_ctl;
	
	//vdp2_vram_control_set(&_vdp2_vram_ctl);
	//vdp2_vram_cycp_clear();
	
	//cpu_dmac_memset(0, (void*)VDP2_VRAM(0X0000), 0x00000000, VDP2_VRAM_SIZE);
}


void vdp2_vram_control_set(const vdp2_vram_ctl_t* vram_ctl)
{
}


void vdp2_vram_usage_set(const vdp2_vram_usage_t* vram_usage)
{
}



vdp2_vram_cycp_t* vdp2_vram_cycp_get(void)
{
}


void vdp2_vram_cycp_set(const vdp2_vram_cycp_t* vram_cycp)
{
}


void vdp2_vram_cycp_clear(void)
{
}



vdp2_vram_cycp_bank_t* vdp2_vram_cycp_bank_get(vdp2_vram_bank_t bank)
{
}


void vdp2_vram_cycp_bank_set(vdp2_vram_bank_t bank, const vdp2_vram_cycp_bank_t* cycp_bank)
{
}


void vdp2_vram_cycp_bank_clear(vdp2_vram_bank_t bank)
{
}
