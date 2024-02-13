#include <gfx/vdp2/vdp2_cram.h>

#include <gfx/vdp2/vdp2.h>



void __vdp2_cram_init(void)
{
	vdp2_cram_mode_set(1);
	
	//Zero get written to VDP2 CRAM SIZE?
}


vdp2_cram_mode_t vdp2_cram_mode_get(void)
{
	return _state_vdp2()->cram_mode;
}


void vdp2_cram_mode_set(vdp2_cram_mode_t mode)
{
	_state_vdp2()->cram_mode = mode;
}


void vdp2_cram_offset_set(vdp2_scrn_t scroll_screen, vdp2_cram_t cram)
{
	//TODO figure out what this function is doing
	
	//for some reason gcc says to replace the enum names with themselves, figure this out later
	/*
	switch(scroll_screen)
	{
		case VDP2_SCRN_NBG0:
			break;
		case VDP2_SCRN_NGB1:
			break;
		case VDP2_SCRN_NGB2:
			break;
		case VDP2_SCRN_NGB3:
			break;
		case VDP2_SCRN_RGB0:
		case VDP2_SCRN_RGB0_PA:
		case VDP2_SCRN_RGB0_PB:
			break;
		case VDP2_SCRN_SPRITE:
			break;
		default:
			break;
	}
	*/
}
