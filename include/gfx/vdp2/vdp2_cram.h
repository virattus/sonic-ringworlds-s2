#ifndef __FAKEYAUL_VDP2_CRAM_H__
#define __FAKEYAUL_VDP2_CRAM_H__


#include <gfx/vdp2/vdp2_scrn.h>
#include <gfx/vdp2/vdp2.h>


//From Yaul:
/* Color RAM (CRAM) stores color data of sprites and scroll screens.
 *
 * R/W access from the CPU or DMA controller(s) is possible, but the image may
 * be disturbed by the access timing. Access through CPU or DMA controller(s) is
 * possible only in word units and long word units. Access in bytes is not
 * allowed */


#define VDP2_CRAM_ADDR(address) (0x25F00000UL + ((address) << 1))

#define VDP2_CRAM_MODE_0_OFFSET(_256_bank, _16_bank, color_index)              \
    (0x25F00000UL + ((_256_bank) << 9) +                                       \
                    ((_16_bank) << 5) +                                        \
                    ((color_index) << 1))
#define VDP2_CRAM_MODE_1_OFFSET(_256_bank, _16_bank, color_index)              \
    VDP2_CRAM_MODE_0_OFFSET(_256_bank, _16_bank, color_index)
#define VDP2_CRAM_MODE_2_OFFSET(_256_bank, _16_bank, color_index)              \
     (0x25F00000UL + ((_256_bank) << 10) +                                     \
                     ((_16_bank) << 6) +                                       \
                     ((color_index) << 2))


/*-
 * CRAM Modes
 *
 * Mode 0: RGB 555 1,024 colors mirrored
 * Mode 1: RGB 555 2,048 colors
 * Mode 2: RGB 888 1,024 colors
 */

#define VDP2_CRAM_MODE_0_SIZE	0x0800
#define VDP2_CRAM_MODE_1_SIZE	0x1000
#define VDP2_CRAM_MODE_2_SIZE	0x1000


#define VDP2_CRAM_SIZE			0x1000

#define COLOR_PALETTE_DATA		0x0000
#define COLOR_RGB_DATA			0x8000

typedef uint32_t vdp2_cram_mode_t;


vdp2_cram_mode_t vdp2_cram_mode_get(void);
void vdp2_cram_mode_set(vdp2_cram_mode_t mode);
void vdp2_cram_offset_set(vdp2_scrn_t scroll_screen, vdp2_cram_t cram);



#endif
