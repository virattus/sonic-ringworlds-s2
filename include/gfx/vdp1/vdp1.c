#include <gfx/vdp1/vdp1.h>

#include <core/preprocessor.h>
#include <gfx/vdp1/vdp1_env.h>
#include <gfx/vram.h>
#include <gfx/render/software/draw.h>
#include <gfx/render/drawstate.h>
#include <gfx/vdp1/vdp1_cmdt.h>
#include <gfx/vdp1/vdp1_displaylist.h>

#include <stdio.h>
#include <assert.h>


#define FRAMEBUFFER_BOUNDARY 		1024




static int __vdp1_initialised = 0;

static uint32_t ActiveFramebuffer = 0;
static VRAM Framebuffers[2];
static VRAM TextureArea;
static int16_vec2_t primitive_local_coord = { FRAMEBUFFER_SIZE_X >> 1, FRAMEBUFFER_SIZE_Y >> 1 };

static const int16_vec2_t FramebufferDimensions = { FRAMEBUFFER_SIZE_X, FRAMEBUFFER_SIZE_Y };

static displaylist_t* dl = NULL;


void VDP1_Init()
{
	if(__vdp1_initialised) return;
	
	vdp1_env_default_set();
		
	VRAM_Init(&Framebuffers[0], FRAMEBUFFER_SIZE_X * FRAMEBUFFER_SIZE_Y * FRAMEBUFFER_SIZE_BPP, 0);
	VRAM_Init(&Framebuffers[1], FRAMEBUFFER_SIZE_X * FRAMEBUFFER_SIZE_Y * FRAMEBUFFER_SIZE_BPP, 0);
	
	VRAM_Init(&TextureArea, VDP1_TEXTURE_RAM_SIZE, 0x0000FFFF);
	VRAM_Fill(&TextureArea, 0, 65535, 0xFF00FFFF);
	
	
		
	__vdp1_initialised = 1;
}


void VDP1_Delete()
{
	if(!__vdp1_initialised) return;
	
	VRAM_Free(&Framebuffers[0]);
	VRAM_Free(&Framebuffers[1]);
	VRAM_Free(&TextureArea);
	
	__vdp1_initialised = 0;
}


void vdp_sync_vblank_out_set(callback_handler_t callback_handler, void* work)
{
}


void vdp1_sync_interval_set(int8_t interval)
{
}


VRAM* VDP1_GetFramebuffer(uint32_t ID)
{
	assert(ID < 2);
	assert(__vdp1_initialised);
	
	return &Framebuffers[ID];
}


VRAM* VDP1_GetDisplayFramebuffer()
{
	return VDP1_GetFramebuffer(ActiveFramebuffer ^ 1);
}


VRAM* VDP1_GetDrawFramebuffer()
{
	return VDP1_GetFramebuffer(ActiveFramebuffer);
}


VRAM* VDP1_GetTextureBuffer()
{
	assert(__vdp1_initialised);
	return &TextureArea;
}



int32_t VDP1_VertexInBounds(VERTEX* v)
{
	if((v->pos.x < -FRAMEBUFFER_BOUNDARY || v->pos.x > FRAMEBUFFER_BOUNDARY) ||
		(v->pos.y < -FRAMEBUFFER_BOUNDARY || v->pos.y > FRAMEBUFFER_BOUNDARY))
	{
		return 0;
	}
	
	return 1;
}


static void VDP1_SortHorizontal(VERTEX* v0, VERTEX* v1)
{
	if(v0->pos.x > v1->pos.x)
	{
		VERTEX temp = *v1;
		*v1 = *v0;
		*v0 = temp;
	}
}

static void VDP1_SortVertical(VERTEX* v0, VERTEX* v1)
{
	if(v0->pos.y < v1->pos.y)
	{
		VERTEX temp = *v1;
		*v1 = *v0;
		*v0 = temp;
	}
}


static void VDP1_DrawPrimitive(PRIMITIVE* p, const DrawState_t* state)
{	
	//Make sure that we skip polys with absurd sizes
	for(int i = 0; i < 4; i++)
	{
		p->v[i].pos.x += primitive_local_coord.x;
		p->v[i].pos.y += primitive_local_coord.y;
		
		if(!VDP1_VertexInBounds(&p->v[i]))
		{
			return;
		}
	}
	
	//printf("Texture ID: %d\n", ActiveTexture);
	
	//p->v[1].pos.x += 1;
	//p->v[2].pos.x += 1;
	//p->v[2].pos.y += 1;
	//p->v[3].pos.y += 1;
	
	p->v[0].col = (CVECTOR){ 255, 255, 255 };
	p->v[1].col = (CVECTOR){ 0, 0, 255 };
	p->v[2].col = (CVECTOR){ 255, 255, 0 };
	p->v[3].col = (CVECTOR){ 0, 255, 255 };
	
	
	DrawPolygon(state, &p->flags, &p->v[0], &p->v[1], &p->v[2], &p->v[3]);
}


void vdp1_cmdt_polygon_set(vdp1_cmdt_t* cmdt)
{
}


void vdp1_cmdt_draw_mode_set(vdp1_cmdt_t* cmdt, vdp1_cmdt_draw_mode_t draw_mode)
{
}


void VDP1_DisplayList_Set(displaylist_t* _dl)
{
	dl = _dl;
}


void vdp1_sync_cmdt_list_put(vdp1_cmdt_list_t* list, uint16_t index)
{
}


void vdp1_sync_render()
{
	dl = DisplayList_VDP1_Get();
	
	DisplayList_Sort(dl);
	
	DrawState_t state = {
		.frameBuffer = VDP1_GetDrawFramebuffer(),
		.frameBuffer_size = FramebufferDimensions,
		.textureRam = &TextureArea,
		.colDepth = COL_RGBA_1555,
	};
	
	for(int j = 0; j < dl->PrimitiveCount; j++)
	{
		VDP1_DrawPrimitive(&dl->PrimitiveList[j], &state);
	}
	
	//printf("drew %d primitives\n", dl->PrimitiveCount);
}


void vdp1_sync()
{
	ActiveFramebuffer ^= 1;
	VRAM_Fill(&Framebuffers[ActiveFramebuffer], 0, Framebuffers[ActiveFramebuffer].Size, 0x000000);
}


void vdp1_sync_wait()
{
	
}
