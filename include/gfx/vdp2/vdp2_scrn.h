#ifndef __FAKEYAUL_VDP2_SCRN_H__
#define __FAKEYAUL_VDP2_SCRN_H__


#include <core/colour.h>
#include <gfx/vdp2/vdp2_vram.h>

#include <libfixmath/fix16.h>

#include <stdint.h>


typedef enum VDP2_SCRN
{
	VDP2_SCRN_NONE		= 0,
	VDP2_SCRN_NBG0		= 1 << 0,
	VDP2_SCRN_NBG1		= 1 << 1,
	VDP2_SCRN_NBG2		= 1 << 2,
	VDP2_SCRN_NBG3		= 1 << 3,
	VDP2_SCRN_RBG0		= 1 << 4,
	VDP2_SCRN_SPRITE	= 1 << 5,
	VDP2_SCRN_BACK		= 1 << 6,
	VDP2_SCRN_RBG0_PA	= 1 << 7,
	VDP2_SCRN_RBG0_PB	= 1 << 8
} vdp2_scrn_t;


typedef enum VDP2_SCRN_DISP 
{
	VDP2_SCRN_DISPTP_NONE = 0x0000,
	VDP2_SCRN_DISPTP_NBG0 = 0x0001,
	VDP2_SCRN_DISPTP_NBG1 = 0x0002,
	VDP2_SCRN_DISPTP_NBG2 = 0x0004,
	VDP2_SCRN_DISPTP_NBG3 = 0x0008,
	VDP2_SCRN_DISPTP_RBG0 = 0x0010,
	VDP2_SCRN_DISPTP_RBG1 = 0x0020,
	
	VDP2_SCRN_DISP_NONE = 0,
	VDP2_SCRN_DISP_NBG0 = 0x0101,
	VDP2_SCRN_DISP_NBG1 = 0x0202,
	VDP2_SCRN_DISP_NBG2 = 0x0404,
	VDP2_SCRN_DISP_NBG3 = 0x0808,
	VDP2_SCRN_DISP_RBG0 = 0x1010,
	VDP2_SCRN_DISP_RBG1 = 0x0120,	
} vdp2_scrn_disp_t;


typedef enum VDP2_SCRN_CCC
{
	VDP2_SCRN_CCC_PALETTE_16,
	VDP2_SCRN_CCC_PALETTE_256,
	VDP2_SCRN_CCC_PALETTE_2048,
	VDP2_SCRN_CCC_RGB_32768,
	VDP2_SCRN_CCC_RGB_16770000,
	
} vdp2_scrn_ccc_t;


vdp2_scrn_disp_t vdp2_scrn_display_get(void);
void vdp2_scrn_display_set(vdp2_scrn_disp_t disp_mask);

void vdp2_scrn_priority_set(vdp2_scrn_t scroll_screen, uint8_t priority);
uint8_t vdp2_scrn_priority_get(vdp2_scrn_t scroll_screen);


void vdp2_scrn_scroll_x_set(vdp2_scrn_t scroll_screen, fix16_t scroll);
void vdp2_scrn_scroll_y_set(vdp2_scrn_t scroll_screen, fix16_t scroll);

void vdp2_scrn_scroll_x_update(vdp2_scrn_t scroll_screen, fix16_t delta);
void vdp2_scrn_scroll_y_update(vdp2_scrn_t scroll_screen, fix16_t delta);

void vdp2_scrn_reduction_x_set(vdp2_scrn_t scroll_screen, fix16_t zoom);
void vdp2_scrn_reduction_y_set(vdp2_scrn_t scroll_screen, fix16_t zoom);


#endif
