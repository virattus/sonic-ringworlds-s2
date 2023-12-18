#ifndef __FAKEYAUL_SOFTWARE_DRAW_H__
#define __FAKEYAUL_SOFTWARE_DRAW_H__


#include <gamemath/vector.h>
#include <gfx/primitive.h>
#include <gfx/vram.h>
#include <gfx/render/drawstate.h>


void DrawLine(const DrawState_t* state, const PRIMITIVE_FLAGS* flags, const VERTEX* v0, const VERTEX* v1);


void DrawSpan(const DrawState_t* state, const PRIMITIVE_FLAGS* flags, const VERTEX* v0, const VERTEX* v1);


void DrawPolyLine(const DrawState_t* state, const PRIMITIVE_FLAGS* flags, const VERTEX* v0, const VERTEX* v1, const VERTEX* V2, const VERTEX* V3);


void DrawPolygon(const DrawState_t* state, const PRIMITIVE_FLAGS* flags, const VERTEX* v0, const VERTEX* v1, const VERTEX* v2, const VERTEX* v3);


#endif
