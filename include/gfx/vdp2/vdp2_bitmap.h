#ifndef __FAKEYAUL_VDP2_BITMAP_H__
#define __FAKEYAUL_VDP2_BITMAP_H__


#include <core/preprocessor.h>
#include <gfx/vdp2/vdp2_vram.h>
#include <gfx/vdp2/vdp2_scrn.h>
#include <gfx/vdp2/vdp2.h>

#include <stdint.h>


typedef enum VDP2_SCRN_BITMAP_SIZE
{
	VDP2_SCRN_BITMAP_SIZE_512X256,
	VDP2_SCRN_BITMAP_SIZE_512X512,
	VDP2_SCRN_BITMAP_SIZE_1024X256,
	VDP2_SCRN_BITMAP_SIZE_1024X512,
	
} __packed vdp2_scrn_bitmap_size_t;


typedef struct VDP2_SCRN_BITMAP_FORMAT
{
	vdp2_scrn_t scroll_screen;
	vdp2_scrn_ccc_t ccc;
	vdp2_scrn_bitmap_size_t bitmap_size;
	vdp2_cram_t palette_base;
	vdp2_vram_t bitmap_base;
	
} __packed __aligned(4) vdp2_scrn_bitmap_format_t;


void vdp2_scrn_bitmap_format_set(const vdp2_scrn_bitmap_format_t* bitmap_format);
void vdp2_scrn_bitmap_ccc_set(const vdp2_scrn_bitmap_format_t* bitmap_format);
void vdp2_scrn_bitmap_base_set(const vdp2_scrn_bitmap_format_t* bitmap_format);
void vdp2_scrn_bitmap_size_set(const vdp2_scrn_bitmap_format_t* bitmap_format);


#endif
