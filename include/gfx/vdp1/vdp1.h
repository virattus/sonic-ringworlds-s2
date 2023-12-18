#ifndef __FAKEYAUL_VDP1_H__
#define __FAKEYAUL_VDP1_H__


#include <core/callback.h>
#include <gamemath/vector.h>
#include <gfx/vram.h>
#include <gfx/vdp1/vdp1_env.h>
#include <gfx/primitive.h>
#include <gfx/vdp1/vdp1_cmdt.h>
#include <gfx/vdp1/vdp1_displaylist.h>

#include <stdint.h>


#define FRAMEBUFFER_SIZE_X 		320
#define FRAMEBUFFER_SIZE_Y 		224
#define FRAMEBUFFER_SIZE_BPP	4

#define VDP1_TEXTURE_RAM_DIM 	512
#define VDP1_TEXTURE_RAM_SIZE	0x0007FFFF


void VDP1_Init();
void VDP1_Delete();

void vdp_sync_vblank_out_set(callback_handler_t callback_handler, void* work);

void vdp1_sync_interval_set(int8_t interval);


VRAM* VDP1_GetFramebuffer(uint32_t ID);
VRAM* VDP1_GetDisplayFramebuffer();
VRAM* VDP1_GetDrawFramebuffer();
VRAM* VDP1_GetTextureBuffer();


void vdp1_cmdt_polygon_set(vdp1_cmdt_t* cmdt);
void vdp1_cmdt_draw_mode_set(vdp1_cmdt_t* cmdt, vdp1_cmdt_draw_mode_t draw_mode);


void VDP1_DisplayList_Set(displaylist_t* dl);
void vdp1_sync_cmdt_list_put(vdp1_cmdt_list_t* list, uint16_t index);

void vdp1_sync_render();
void vdp1_sync();
void vdp1_sync_wait();

#endif
