#ifndef __FAKEYAUL_VDP2_SCREEN_BACK_H__
#define __FAKEYAUL_VDP2_SCREEN_BACK_H__


#include <core/colour.h>
#include <gfx/vdp2/vdp2_vram.h>

#include <stdint.h>


rgb1555_t vdp2_scrn_back_colour_get(uint32_t lineNum);

void vdp2_scrn_back_color_set(vdp2_vram_t vram, rgb1555_t colour);

void vdp2_scrn_back_buffer_set(vdp2_vram_t vram, const rgb1555_t* buffer, uint32_t count);

void vdp2_scrn_back_sync(void);


#endif
