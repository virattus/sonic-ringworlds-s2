#ifndef __FAKEYAUL_MIC3D_STATE_H__
#define __FAKEYAUL_MIC3D_STATE_H__


#include <gfx/light_processor.h>
#include <gfx/render/render.h>
#include <gfx/texture.h>
#include <gamemath/matrix.h>
#include <mic3d/sort.h>
#include <mic3d/workarea.h>



typedef struct gst gst_t;

typedef struct perf perf_t;

typedef struct 
{
	workarea_mic3d_t* workarea;
    render_t *render;
    sort_t *sort;
    tlist_t *tlist;
    mstack_t *mstack;
    light_t *light;
    gst_t *gst;
    perf_t *perf;
} state_t;

extern state_t __state;


void mic3d_init(workarea_mic3d_t* workarea);

#endif
