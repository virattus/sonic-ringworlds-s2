#include <gfx/vdp2/vdp2_bitmap.h>


#include <gfx/vdp2/vdp2_cram.h>



void vdp2_scrn_bitmap_format_set(const vdp2_scrn_bitmap_format_t* bitmap_format)
{
	vdp2_scrn_bitmap_ccc_set(bitmap_format);
	vdp2_scrn_bitmap_base_set(bitmap_format);
	vdp2_scrn_bitmap_size_set(bitmap_format);
}


void vdp2_scrn_bitmap_ccc_set(const vdp2_scrn_bitmap_format_t* bitmap_format)
{
	//TODO these functions do a lot with shadow registers and bit manips, learn more before you try to recreate them

	switch(bitmap_format->scroll_screen)
	{
		case VDP2_SCRN_NBG0:
			break;
		case VDP2_SCRN_NBG1:
			break;
		case VDP2_SCRN_RBG0:
		case VDP2_SCRN_RBG0_PA:
		case VDP2_SCRN_RBG0_PB:
			break;
		default: //This one is legit empty
			break;
	}
	
	//From Yaul:
    /* Specifying a palette number for 2,048-colors is invalid as it uses up
     * nearly all of CRAM (4 KiB).
     *
     * 256-color needs 3 bits as there are 8 possible color banks.
     *
     * For 16-colors, there are 128 possible banks, but only 8 are available
     * (the lower 4- bits of the palette number is 0) */

	switch(bitmap_format->ccc)
	{
		case VDP2_SCRN_CCC_PALETTE_16:
		case VDP2_SCRN_CCC_PALETTE_256:
			break;
		default:
			break;
	}
}


void vdp2_scrn_bitmap_base_set(const vdp2_scrn_bitmap_format_t* bitmap_format)
{
	//Same as above
	
	switch(bitmap_format->scroll_screen)
	{
		case VDP2_SCRN_NBG0:
			break;
		case VDP2_SCRN_NBG1:
			break;
		case VDP2_SCRN_RBG0:
		case VDP2_SCRN_RBG0_PA:
			break;
		case VDP2_SCRN_RBG0_PB:
			break;
		default: //This one is legit empty
			break;
	}
}


void vdp2_scrn_bitmap_size_set(const vdp2_scrn_bitmap_format_t* bitmap_format)
{
	//same as above
	switch(bitmap_format->scroll_screen)
	{
		case VDP2_SCRN_NBG0:
			break;
		case VDP2_SCRN_NBG1:
			break;
		case VDP2_SCRN_RBG0_PA:
		case VDP2_SCRN_RBG0_PB:
			break;
		default: //This one is legit empty
			break;
	}
}
