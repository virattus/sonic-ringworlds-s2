#ifndef __FAKEYAUL_SOFTWARE_DRAW_H__
#define __FAKEYAUL_SOFTWARE_DRAW_H__


#include <gamemath/vector.h>
#include <gfx/render/primitive.h>
#include <gfx/vram.h>
#include <gfx/render/drawstate.h>



void DrawLine(DrawState_t* state, const PrimitiveFlags_t* flags, const Vertex_t* v0, const Vertex_t* v1);

void DrawPolyLine(DrawState_t* state, const PrimitiveFlags_t* flags, const Vertex_t* v0, const Vertex_t* v1, const Vertex_t* V2, const Vertex_t* V3);

void DrawSpan(DrawState_t* state, const PrimitiveFlags_t* flags, const Vertex_t* v0, const Vertex_t* v1, int16_t y);
void DrawQuad(DrawState_t* state, const PrimitiveFlags_t* flags, const Vertex_t* v0, const Vertex_t* v1, const Vertex_t* v2, const Vertex_t* v3);


#endif
