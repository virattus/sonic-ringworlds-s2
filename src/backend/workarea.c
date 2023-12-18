#include "workarea.h"

#include <yaul.h>


static vdp1_gouraud_table_t _pool_shading_tables[CONFIG_MIC3D_CMDT_COUNT] __aligned(16);
static vdp1_gouraud_table_t _pool_shading_tables2[512] __aligned(16);

static workarea_mic3d_depth_values_t _pool_depth_values;
static workarea_mic3d_z_values_t _pool_z_values;
static workarea_mic3d_screen_points_t _pool_screen_points;
static workarea_mic3d_sort_singles_t _pool_sort_singles;
static workarea_mic3d_cmdts_t _pool_cmdts;
static workarea_mic3d_render_matrices_t _pool_render_matrices;
static workarea_mic3d_light_matrices_t _pool_light_matrices;
static workarea_mic3d_colors_t _pool_colors;
static workarea_mic3d_work_t _pool_work;

workarea_mic3d_t _workarea = {
    &_pool_depth_values,   &_pool_z_values, &_pool_screen_points,
    &_pool_sort_singles,   &_pool_cmdts,    &_pool_render_matrices,
    &_pool_light_matrices, &_pool_colors,   &_pool_work
};
