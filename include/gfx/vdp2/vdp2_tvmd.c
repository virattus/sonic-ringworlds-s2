#include <gfx/vdp2/vdp2_tvmd.h>


#include <gfx/vdp2/vdp2.h>


void vdp2_tvmd_display_set(void)
{
}


void vdp2_tvmd_display_res_set(vdp2_tvmd_interlace_t interlace, vdp2_tvmd_horz_t horizontal, vdp2_tvmd_vert_t vertical)
{
	vdp2_state_t* _state = _state_vdp2();
	
	_state->tv.interlaceMode = interlace;
	
	switch(vertical)
	{
		case VDP2_TVMD_VERT_224:
			_state->tv.resolution.y = 224;
			break;
		case VDP2_TVMD_VERT_240:
			_state->tv.resolution.y = 240;
			break;
		//case VDP2_TVMD_VERT_256:
			//_state->tv.resolution.y = 256;
			//break;
	}
	
	switch(horizontal)
	{
		case VDP2_TVMD_HORZ_NORMAL_A: /* 320x */
			_state->tv.resolution.x = 320;
			break;
		case VDP2_TVMD_HORZ_NORMAL_B: /* 352x */
			_state->tv.resolution.x = 352;
			break;
		case VDP2_TVMD_HORZ_HIRESO_A: /* 640x */
			_state->tv.resolution.x = 640;
			break;
		case VDP2_TVMD_HORZ_HIRESO_B: /* 704x */
			_state->tv.resolution.x = 704;
			break;
		case VDP2_TVMD_HORZ_NORMAL_AE: /* 320x480 */
			_state->tv.resolution.x = 320;
			_state->tv.resolution.y = 480;
			break;
		case VDP2_TVMD_HORZ_NORMAL_BE: /* 352x480 */
			_state->tv.resolution.x = 352;
			_state->tv.resolution.y = 480;
			break;
		case VDP2_TVMD_HORZ_HIRESO_AE: /* 640x480 */
			_state->tv.resolution.x = 640;
			_state->tv.resolution.y = 480;
			break;
		case VDP2_TVMD_HORZ_HIRESO_BE:  /* 704x480 */
			_state->tv.resolution.x = 704;
			_state->tv.resolution.y = 480;
			break;
	}
	
	printf("Set VDP2 Resolution to %dx%d\n", _state->tv.resolution.x, _state->tv.resolution.y);
	
	//TODO figure out how to signal to whatever reads VDP2's framebuffer that the resolution has changed
	
}

