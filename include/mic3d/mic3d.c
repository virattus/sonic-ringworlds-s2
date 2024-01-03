#include <mic3d.h>


#include <mic3d/light_processor.h>
#include <mic3d/gst.h>


#include <assert.h>


static render_t _render;
static sort_t _sort;
static tlist_t _tl;
static light_t _light;
static gst_t _gst;
static mstack_t ms;




void mic3d_init(workarea_mic3d_t* workarea)
{
	assert(workarea != NULL);
		
	__state.workarea = workarea;
	__state.render = &_render;
	__state.sort = &_sort;
	__state.tlist = &_tl;
	__state.light = &_light;
	__state.gst = &_gst;
	__state.mstack = &ms;
	
	__matrix_init();
	__render_init();
	__sort_init();
	__tlist_init();
	__light_init();
	__gst_init();
}

