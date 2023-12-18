#include <gfx/vdp2/vdp2_tvmd.h>


#include <gfx/vdp2/vdp2.h>


void vdp2_tvmd_display_set(void)
{
}


void vdp2_tvmd_display_res_set(vdp2_tvmd_interlace_t interlace, vdp2_tvmd_horz_t horizontal, vdp2_tvmd_vert_t vertical)
{
	vdp2_state_t* _state = _state_vdp2();
	
	_state->tv.resolution = (int16_vec2_t){ 320, 224 };
	
}

