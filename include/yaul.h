#ifndef __FAKEYAUL_H__
#define __FAKEYAUL_H__

#define SDL_MAIN_HANDLED

#include <core/preprocessor.h>
#include <core/types.h>
#include <core/ram.h>
#include <core/window.h>
#include <core/timer.h>
#include <core/controller.h>
#include <core/scu.h>
#include <core/cd.h>
#include <core/audio.h>
#include <core/colour.h>
#include <core/dbgio.h>
#include <core/gamewindow.h>

#include <gamemath/math3d.h>
#include <gamemath/vector.h>
#include <gamemath/fix16_vector.h>
#include <gamemath/int_vector.h>
#include <gamemath/matrix.h>
#include <gamemath/mat33.h>
#include <gamemath/mat43.h>

#include <gfx/mesh.h>
#include <gfx/clip.h>
#include <gfx/texture.h>
#include <gfx/vram.h>
#include <gfx/primitive.h>
#include <gfx/light_processor.h>

#include <gfx/vdp1/vdp1.h>
#include <gfx/vdp1/vdp1_env.h>
#include <gfx/vdp1/vdp1_vram.h>
#include <gfx/vdp1/vdp1_cmdt.h>
#include <gfx/vdp1/vdp1_displaylist.h>


#include <gfx/vdp2/vdp2.h>
#include <gfx/vdp2/vdp2_cram.h>
#include <gfx/vdp2/vdp2_tvmd.h>
#include <gfx/vdp2/vdp2_vram.h>
#include <gfx/vdp2/vdp2_scrn.h>
#include <gfx/vdp2/vdp2_tilemap.h>
#include <gfx/vdp2/vdp2_mode7.h>

#include <gfx/render/render.h>
#include <gfx/render/render_mesh.h>

#endif