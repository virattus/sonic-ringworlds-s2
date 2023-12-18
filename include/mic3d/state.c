#include <mic3d/state.h>

#include <gamemath/matrix.h>
#include <gfx/vdp1/vdp1.h>
#include <gfx/vdp2/vdp2.h>


static tlist_t tl;
static mstack_t ms;

state_t __state;



void mic3d_init(workarea_mic3d_t* workarea)
{
	VDP2_Init();
	VDP1_Init();
	
	__state.workarea = workarea;
	__state.tlist = &tl;
	__state.mstack = &ms;
	
	__render_init();
	__matrix_init();
}
