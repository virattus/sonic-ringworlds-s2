#ifndef __FAKEYAUL_MIC3D_STATE_H__
#define __FAKEYAUL_MIC3D_STATE_H__


#include "mic3d.h"



typedef struct _RENDER render_t;
typedef struct _sort sort_t;
typedef struct texturelist tlist_t;
typedef struct LIGHT_NODE light_t;
typedef struct gst gst_t;
typedef struct mstack mstack_t;

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


#endif
