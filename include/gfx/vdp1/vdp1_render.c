#include <gfx/vdp1/vdp1_render.h>

#include <core/colour.h>
#include <gfx/vdp1/vdp1.h>
#include <mic3d/texture.h>
#include <gfx/render/primitive.h>
#include <gfx/render/software/draw.h>



#include <SDL2/SDL_thread.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>


#define VDP1_CMDT_SIZE				32
#define FRAMEBUFFER_BOUNDARY 		1024

#define NUM_THREADS					1



typedef struct VDP1_PRIM_STATE
{
	vdp1_cmdt_t* Cmdt;
	vdp1_cmdt_t* NextCmdt;
	vdp1_cmdt_t* SubroutineCaller;
	int16_vec2_t LocalCoordinates;
	int16_vec2_t ClipMin, ClipMax;
	int16_vec2_t UserClipMin, UserClipMax;
	
} VDP1_PrimState_t;



static bool IsVDP1Rendering = false;

static SDL_Thread* threads[NUM_THREADS];




static int32_t VDP1_VertexInBounds(Vertex_t* v)
{
	if((v->x < -FRAMEBUFFER_BOUNDARY || v->x > FRAMEBUFFER_BOUNDARY) ||
		(v->y < -FRAMEBUFFER_BOUNDARY || v->y > FRAMEBUFFER_BOUNDARY))
	{
		return 0;
	}
	
	return 1;
}


static int32_t VDP1_PolygonInBounds(Primitive_t* p)
{
	for(int i = 0; i < 4; i++)
	{
		if(!VDP1_VertexInBounds(&p->v[i]))
		{
			return 0;
		}
	}
	
	return 1;
}


static int32_t VDP1_LineInBounds(Primitive_t* p)
{
	if(!VDP1_VertexInBounds(&p->v[0]) || 
		!VDP1_VertexInBounds(&p->v[1]))
	{
		return 0;
	}
	
	return 1;
}


static void VDP1_SortHorizontal(Vertex_t* v0, Vertex_t* v1)
{
	if(v0->x > v1->x)
	{
		Vertex_t temp = *v1;
		*v1 = *v0;
		*v0 = temp;
	}
}

static void VDP1_SortVertical(Vertex_t* v0, Vertex_t* v1)
{
	if(v0->y < v1->y)
	{
		Vertex_t temp = *v1;
		*v1 = *v0;
		*v0 = temp;
	}
}


static int VDP1_DrawPrimitive(const DrawState_t* state, VDP1_PrimState_t* primState, Primitive_t* p)
{
	switch(p->flags.DrawPrimType)
	{
		case DRAW_POLYGON:
			if(VDP1_PolygonInBounds(p))
			{
				DrawQuad(state, &p->flags, &p->v[0], &p->v[1], &p->v[2], &p->v[3]);
			}
			break;
		case DRAW_LINE:
			if(VDP1_LineInBounds(p))
			{
				DrawLine(state, &p->flags, &p->v[0], &p->v[1]);
			}
			break;
		case DRAW_POLYLINE:
			if(VDP1_PolygonInBounds)
			{
				DrawPolyLine(state, &p->flags, &p->v[0], &p->v[1], &p->v[2], &p->v[3]);
			}
			break;
		default:
			assert(false);
			break;
	}
	
	return 1;
}


static void VDP1_SetPrimitiveGouraud(Primitive_t* p, uint16_t gst_slot)
{
	const vdp1_gouraud_table_t* gouraudTable = _state_vdp1()->vram_partitions.gouraud_base;
	
	for(int i = 0; i < 4; i++)
	{
		p->v[i].r = gouraudTable[gst_slot].colors[i].r;
		p->v[i].g = gouraudTable[gst_slot].colors[i].g;
		p->v[i].b = gouraudTable[gst_slot].colors[i].b;
	}
}


static int32_t VDP1_GetNextCommand(VDP1_PrimState_t* primState)
{
	assert(primState != NULL);
	
	const vdp1_cmdt_t* cmdt_base = _state_vdp1()->vram_partitions.cmdt_base;
	
	const uint16_t jumpSelect = (primState->Cmdt->cmd_ctrl & 0x7000) >> 12;

	
	if(jumpSelect == 0)
	{
		//Jump Next, just go to next command (+32 bytes)
		primState->NextCmdt = (vdp1_cmdt_t*)(primState->Cmdt + 1);
	}
	else if(jumpSelect == 1)
	{
		//Jump assign, go straight to jumplink
		uint16_t jumpLink = primState->Cmdt->cmd_link;
		
		primState->NextCmdt = (vdp1_cmdt_t*)(cmdt_base + (jumpLink >> 5));
	}
	else if(jumpSelect == 2)
	{
		//Jump Call, subroutine call, find out what this is
		primState->SubroutineCaller = (vdp1_cmdt_t*)(primState->Cmdt + 1);
		
		uint16_t jumpLink = primState->Cmdt->cmd_link;
		
		primState->NextCmdt = (vdp1_cmdt_t*)(cmdt_base + (jumpLink >> 5));
	}
	else if(jumpSelect == 3)
	{
		//Jump Return, subroutine return, jesus
		assert(primState->SubroutineCaller != NULL);
		
		primState->NextCmdt = primState->SubroutineCaller; //Look up if this should be set to NULL afterwards
	}
	
	if(jumpSelect & 0x4)
	{
		//Skip commands, which do the same thing but don't render
		return 0;
	}
	else
	{
		return 1;
	}
}


static uint32_t VDP1_DrawCmdt(const DrawState_t* state, VDP1_PrimState_t* primState)
{
	if(!VDP1_GetNextCommand(primState))
	{
		//Skip drawing this primitive
		return 1;
	}
	
	
	vdp1_cmdt_t* cmdt = primState->Cmdt;
	
	
	uint8_t ctrl = cmdt->cmd_ctrl & 0x000F;

	
	//Draw End Command
	if(cmdt->cmd_ctrl == 0x8000)
	{
		//printf("Draw End\n");
		return 0;
	}
	//Set local coords
	else if(ctrl == 0x0A)
	{
		//printf("set local coords\n");
		
		primState->LocalCoordinates.x = cmdt->cmd_xa;
		primState->LocalCoordinates.y = cmdt->cmd_ya;
		
		return 1;
	}
	//system clipping coordinate set
	else if(ctrl == 0x09)
	{
		//printf("set system clipping\n");
		
		primState->ClipMax.x = cmdt->cmd_xc;
		primState->ClipMax.y = cmdt->cmd_yc;
		
		return 1;
	}
	//user clipping coordinates
	else if(ctrl == 0x08)
	{
		//printf("user clipping coords\n");
		
		
		primState->UserClipMin.x = cmdt->cmd_xa;
		primState->UserClipMin.y = cmdt->cmd_ya;
		
		primState->UserClipMax.x = cmdt->cmd_xc;
		primState->UserClipMax.y = cmdt->cmd_yc;
		
		return 1;
	}
	
	
	//Actual render primitives
	Primitive_t prim;
	
	prim.flags = (PrimitiveFlags_t){
		.DrawPrimType = DRAW_POLYGON,
		.ColourMode = cmdt->cmd_draw_mode.color_mode,
		.DrawMesh = cmdt->cmd_draw_mode.mesh_enable,
		.DrawTransparent = cmdt->cmd_draw_mode.cc_mode & 1,
		.DrawHalfLuminance = cmdt->cmd_draw_mode.cc_mode & 2,
		.DrawGouraud = cmdt->cmd_draw_mode.cc_mode & 4,
		.IgnoreTextureTransparency = !(cmdt->cmd_draw_mode.trans_pixel_disable),
		.ClipMin = state->SystemClipMin,
		.ClipMax = state->SystemClipMax,
	};

	
	if(cmdt->cmd_draw_mode.user_clipping_enable)
	{
		//Handle user clipping coordinates
		//Remember that we need to adjust them so that we keep to the space allotted for multithreading
		prim.flags.ClipMin = state->SystemClipMin;
		prim.flags.ClipMax = state->SystemClipMax;
	}
	
	
	//normal sprite draw
	if(ctrl == 0x00)
	{
		printf("Draw normal sprite\n");
		
		prim.flags.DrawTextured = true;
		prim.flags.TextureOffset = cmdt->cmd_srca;
		prim.flags.TextureDimensions = cmdt->cmd_size;

		int16_vec2_t dimensions = TEXTURE_SIZE_TO_VEC(cmdt->cmd_size);
		
		prim.v[0] = (Vertex_t){
			.x = cmdt->cmd_xa + primState->LocalCoordinates.x,
			.y = cmdt->cmd_ya + primState->LocalCoordinates.y,
			.u = 0,
			.v = 0,
			.r = 0,
			.g = 0,
			.b = 0,
		};

		//Probably use the character size for the dimensions
		
		prim.v[1] = (Vertex_t){
			.x = cmdt->cmd_xa + dimensions.x + primState->LocalCoordinates.x,
			.y = cmdt->cmd_ya + primState->LocalCoordinates.y,
			.u = dimensions.x,
			.v = 0,
			.r = 0,
			.g = 0,
			.b = 0,
		};
		
		prim.v[2] = (Vertex_t){
			.x = cmdt->cmd_xa + dimensions.x + primState->LocalCoordinates.x,
			.y = cmdt->cmd_ya + dimensions.y + primState->LocalCoordinates.y,
			.u = dimensions.x,
			.v = dimensions.y,
			.r = 0,
			.g = 0,
			.b = 0,
		};
		
		prim.v[3] = (Vertex_t){
			.x = cmdt->cmd_xa + primState->LocalCoordinates.x,
			.y = cmdt->cmd_ya + dimensions.y + primState->LocalCoordinates.y,
			.u = 0,
			.v = dimensions.y,
			.r = 0,
			.g = 0,
			.b = 0,
		};

		
	}
	//Scaled sprite draw
	else if(ctrl == 0x01)
	{
		printf("Draw scaled sprite\n");
		
		prim.flags.DrawTextured = true;
		prim.flags.TextureOffset = cmdt->cmd_srca;
		prim.flags.TextureDimensions = cmdt->cmd_size;
		
		int16_vec2_t dimensions = TEXTURE_SIZE_TO_VEC(cmdt->cmd_size);
		
		

		
	}
	//Distorted sprite
	else if(ctrl == 0x02)
	{
		printf("Draw distorted sprite\n");
		
		prim.flags.DrawTextured = true;
		prim.flags.TextureOffset = cmdt->cmd_srca;
		prim.flags.TextureDimensions = cmdt->cmd_size;

		int16_vec2_t dimensions = TEXTURE_SIZE_TO_VEC(cmdt->cmd_size);

		//TODO gotta handle the read direction
		prim.v[0] = (Vertex_t){
			.x = cmdt->cmd_xa + primState->LocalCoordinates.x,
			.y = cmdt->cmd_ya + primState->LocalCoordinates.y,
			.u = 0,
			.v = 0,
			.r = 0,
			.g = 0,
			.b = 0,
		};
		
		prim.v[1] = (Vertex_t){
			.x = cmdt->cmd_xb + primState->LocalCoordinates.x,
			.y = cmdt->cmd_yb + primState->LocalCoordinates.y,
			.u = dimensions.x,
			.v = 0,
			.r = 0,
			.g = 0,
			.b = 0,
		};
		
		prim.v[2] = (Vertex_t){
			.x = cmdt->cmd_xc + primState->LocalCoordinates.x,
			.y = cmdt->cmd_yc + primState->LocalCoordinates.y,
			.u = dimensions.x,
			.v = dimensions.y,
			.r = 0,
			.g = 0,
			.b = 0,
		};
		
		prim.v[3] = (Vertex_t){
			.x = cmdt->cmd_xd + primState->LocalCoordinates.x,
			.y = cmdt->cmd_yd + primState->LocalCoordinates.y,
			.u = 0,
			.v = dimensions.y,
			.r = 0,
			.g = 0,
			.b = 0,
		};
		
	}
	//non-textured polygon
	else if(ctrl == 0x04)
	{
		//printf("draw non-textured polygon\n");
		
		prim.flags.DrawTextured = false;
		
		rgb1555_t colour = (rgb1555_t)cmdt->cmd_colr;
				
		//store base colour in texture dimensions
		prim.flags.TextureOffset = colour.raw;
		
		prim.v[0] = (Vertex_t){
			.x = cmdt->cmd_xa + primState->LocalCoordinates.x,
			.y = cmdt->cmd_ya + primState->LocalCoordinates.y,
			.u = 1,
			.v = 1,
			.r = 31,
			.g = 31,
			.b = 31,
		};
		
		prim.v[1] = (Vertex_t){
			.x = cmdt->cmd_xb + primState->LocalCoordinates.x,
			.y = cmdt->cmd_yb + primState->LocalCoordinates.y,
			.u = 1,
			.v = 1,
			.r = 0,
			.g = 0,
			.b = 0,
		};
		
		prim.v[2] = (Vertex_t){
			.x = cmdt->cmd_xc + primState->LocalCoordinates.x,
			.y = cmdt->cmd_yc + primState->LocalCoordinates.y,
			.u = 1,
			.v = 1,
			.r = 31,
			.g = 31,
			.b = 31,
		};
		
		prim.v[3] = (Vertex_t){
			.x = cmdt->cmd_xd + primState->LocalCoordinates.x,
			.y = cmdt->cmd_yd + primState->LocalCoordinates.y,
			.u = 1,
			.v = 1,
			.r = 0,
			.g = 0,
			.b = 0,
		};
		
		
	}
	//non-textured polyline
	else if(ctrl == 0x05)
	{
		//printf("Draw non-textured polyline\n");
		
		prim.flags.DrawPrimType = DRAW_POLYLINE;
		prim.flags.DrawTextured = false;
		
		rgb1555_t colour = (rgb1555_t)cmdt->cmd_colr;
				
		//store base colour in texture dimensions
		prim.flags.TextureOffset = colour.raw;
		
		prim.v[0] = (Vertex_t){
			.x = cmdt->cmd_xa + primState->LocalCoordinates.x,
			.y = cmdt->cmd_ya + primState->LocalCoordinates.y,
			.u = 1,
			.v = 1,
			.r = 0,
			.g = 0,
			.b = 0,
		};
		
		prim.v[1] = (Vertex_t){
			.x = cmdt->cmd_xb + primState->LocalCoordinates.x,
			.y = cmdt->cmd_yb + primState->LocalCoordinates.y,
			.u = 1,
			.v = 1,
			.r = 0,
			.g = 0,
			.b = 0,
		};
		
		prim.v[2] = (Vertex_t){
			.x = cmdt->cmd_xc + primState->LocalCoordinates.x,
			.y = cmdt->cmd_yc + primState->LocalCoordinates.y,
			.u = 1,
			.v = 1,
			.r = 0,
			.g = 0,
			.b = 0,
		};
		
		prim.v[3] = (Vertex_t){
			.x = cmdt->cmd_xd + primState->LocalCoordinates.x,
			.y = cmdt->cmd_yd + primState->LocalCoordinates.y,
			.u = 1,
			.v = 1,
			.r = 0,
			.g = 0,
			.b = 0,
		};
	}
	//non-textured line
	else if(ctrl == 0x06)
	{
		//printf("Drawing non textured line\n");
		
		prim.flags.DrawPrimType = DRAW_LINE;
		prim.flags.DrawTextured = false;
		
		rgb1555_t colour = (rgb1555_t)cmdt->cmd_colr;
				
		//store base colour in texture dimensions
		prim.flags.TextureOffset = colour.raw;
		
		prim.v[0] = (Vertex_t){
			.x = cmdt->cmd_xa + primState->LocalCoordinates.x,
			.y = cmdt->cmd_ya + primState->LocalCoordinates.y,
			.u = 1,
			.v = 1,
			.r = 0,
			.g = 0,
			.b = 0,
		};

		prim.v[1] = (Vertex_t){
			.x = cmdt->cmd_xb + primState->LocalCoordinates.x,
			.y = cmdt->cmd_yb + primState->LocalCoordinates.y,
			.u = 1,
			.v = 1,
			.r = 0,
			.g = 0,
			.b = 0,
		};
	}
	
	
	VDP1_DrawPrimitive(state, primState, &prim);
	
	return 1;
}


bool VDP1_IsRendering()
{
	return IsVDP1Rendering;
}



static int VDP1_RenderThread(void* data)
{
	const DrawState_t* state = (DrawState_t*)data;
	
	VDP1_PrimState_t primState = 
	{
		.Cmdt = _state_vdp1()->vram_partitions.cmdt_base,
		.NextCmdt = NULL,
		.SubroutineCaller = NULL,
		.LocalCoordinates = { 0, 0 }, 
		.ClipMin = state->SystemClipMin,
		.ClipMax = state->SystemClipMax,
		.UserClipMin = state->SystemClipMin,
		.UserClipMax = state->SystemClipMax,
	};
			
	
	//We run a fixed size loop to ensure that there's no
	//infinite loops in the cmdt jump logic
	//In the future, we might end up doing loops on purpose
	//to speed up rendering but for right now this should work
	for(int i = 0; i < CMDT_MAX; i++)
	{
		if(!VDP1_DrawCmdt(state, &primState))
		{
			break;
		}
		
		primState.Cmdt = primState.NextCmdt;
	}
}


//We should figure out the best way to structure this so that 
//everything runs appropriately on the right cores
void VDP1_Render()
{		
	if(IsVDP1Rendering)
	{
		return;
	}

	IsVDP1Rendering = true;
	
	vdp1_state_t* vdp1State = _state_vdp1();
	vdp1_env_t* vdp1Env = vdp1State->current_env;
	
	//Clear screen, this might not be needed?
	VRAM* drawBuffer = VDP1_GetDrawFramebuffer();
	VRAM_Fill(drawBuffer, 0, drawBuffer->Size, 0);
		
	DrawState_t state = {
		.buffer = drawBuffer,
		.bufferSize = vdp1Env->erase_points[1],
		.textureBuffer = VDP1_GetTextureBuffer(),
		.colDepth = COL_RGB_1555,
		.SystemClipMin = vdp1Env->erase_points[0],
		.SystemClipMax = vdp1Env->erase_points[1],
	};
	
	//printf("VDP1 Screen Size: %dx%d\n", state.bufferSize.x, state.bufferSize.y);
	
	//We're segfaulting here in a weird way, I'm not gonna bother trying to figure it out any more
	/*
	for(int i = 0; i < NUM_THREADS; i++)
	{
		//Because the scanline raster is horizontally oriented, we 
		//divide the screen into vertical groups for max efficiency
		//TODO: this should be done better to ensure that there's no 
		//issues from rounding due to the vertical size of the framebuffer
		//not being evenly divisible by the number of threads
		state.SystemClipMin.y = (state.bufferSize.y / NUM_THREADS) * i;
		state.SystemClipMax.y = (state.bufferSize.y / NUM_THREADS) * (i + 1);
		
		threads[i] = SDL_CreateThread(VDP1_RenderThread, "VDP1_Render_" + i, (void*)&state);
	}
	*/
	
	
	
	VDP1_RenderThread((void*)&state);
	
	
	//printf("drew %d primitives\n", dl->PrimitiveCount);
}


void VDP1_WaitThreads()
{
	
	for(int i = 0; i < NUM_THREADS; i++)
	{
		SDL_WaitThread(threads[i], NULL);
	}
	
	
	IsVDP1Rendering = false;
}
