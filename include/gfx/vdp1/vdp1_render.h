#ifndef __FAKEYAUL_VDP1_RENDER_H__
#define __FAKEYAUL_VDP1_RENDER_H__


#include <gfx/vdp1/vdp1_cmdt.h>
#include <gfx/render/drawstate.h>

#include <stdint.h>
#include <stdbool.h>


//returns 0 for ending drawing
uint32_t VDP1_DecodeAndDrawCmdt(const DrawState_t* state, const vdp1_cmdt_t* cmdt, uint16_t* nextCmdt);


bool VDP1_IsRendering();

void VDP1_Render();
void VDP1_WaitThreads();


#endif 
