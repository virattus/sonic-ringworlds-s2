#ifndef __FAKEYAUL_VDP1_SYNC_H__
#define __FAKEYAUL_VDP1_SYNC_H__


#include <core/callback.h>
#include <gfx/vdp1/vdp1_cmdt.h>


#include <stdint.h>


typedef enum VDP1_SYNC_MODE
{
	VDP1_SYNC_MODE_ERASE_CHANGE = 0,
	VDP1_SYNC_MODE_CHANGE_ONLY = 1,
} vdp_sync_mode_t;


void vdp_sync_vblank_out_set(callback_handler_t callback_handler, void* work);

void vdp1_sync_interval_set(int8_t interval);


void vdp1_sync_mode_set(vdp_sync_mode_t mode);
void vdp1_sync_cmdt_list_put(vdp1_cmdt_list_t* list, uint16_t index);
void vdp1_sync_cmdt_put(const vdp1_cmdt_t* cmdts, uint16_t count, uint16_t index);

void vdp1_sync_force_put(void);

//as far as I can tell these three do different things with registers and whatnot
//but here we'll just have them do the same thing
void vdp1_sync_render();
void vdp1_sync();
void vdp1_sync_wait();


#endif
