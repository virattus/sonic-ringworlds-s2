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

#define NUM_THREADS					4



static bool IsVDP1Rendering = false;

static SDL_Thread* threads[NUM_THREADS];

static int16_vec2_t localCoordinates = { 0, 0 };

//used by jump mode for fucking jumping around
static vdp1_cmdt_t* SubroutineCaller = NULL;


static int32_t VDP1_VertexInBounds(Vertex_t* v)
{
	if((v->x < -FRAMEBUFFER_BOUNDARY || v->x > FRAMEBUFFER_BOUNDARY) ||
		(v->y < -FRAMEBUFFER_BOUNDARY || v->y > FRAMEBUFFER_BOUNDARY))
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


static int VDP1_DrawPrimitive(DrawState_t* state, Primitive_t* p)
{	
	//Make sure that we skip polys with absurd sizes
	for(int i = 0; i < 4; i++)
	{
		p->v[i].x += localCoordinates.x;
		p->v[i].y += localCoordinates.y;
		
		if(!VDP1_VertexInBounds(&p->v[i]))
		{
			printf("VDP_DrawPrimitive attempt to draw primitive that exceeds max size\n");
			return 1;
		}
	}
	
	switch(p->flags.DrawPrimType)
	{
		case DRAW_POLYGON:
			DrawQuad(state, &p->flags, &p->v[0], &p->v[1], &p->v[2], &p->v[3]);
			break;
		case DRAW_LINE:
			DrawLine(state, &p->flags, &p->v[0], &p->v[1]);
			break;
		case DRAW_POLYLINE:
			DrawPolyLine(state, &p->flags, &p->v[0], &p->v[1], &p->v[2], &p->v[3]);
			break;
		default:
			assert(false);
			break;
	}
	
	return 1;
}


static void VDP1_SetPrimitiveGouraud(Primitive_t* p, uint16_t gst_slot, bool modifyValues)
{
	const vdp1_gouraud_table_t* gouraudTable = _state_vdp1()->vram_partitions.gouraud_base;
	
	for(int i = 0; i < 4; i++)
	{
		uint8_t r = p->v[i].r;
		uint8_t g = p->v[i].g;
		uint8_t b = p->v[i].b;
		
		if(modifyValues)
		{
			r = 16;
			g = 16;
			b = 16;
		}
		
		p->v[i].r = CLAMP(r + gouraudTable[gst_slot].colors[i].r - 15, 0, 31);
		p->v[i].g = CLAMP(g + gouraudTable[gst_slot].colors[i].r - 15, 0, 31);
		p->v[i].b = CLAMP(b + gouraudTable[gst_slot].colors[i].r - 15, 0, 31);
		
	}
	
	
}


static int32_t VDP1_GetNextCommand(const vdp1_cmdt_t* cmdt, vdp1_cmdt_t** nextCmdt)
{
	assert(cmdt != NULL);
	assert(nextCmdt != NULL);
	
	const vdp1_cmdt_t* cmdt_base = _state_vdp1()->vram_partitions.cmdt_base;
	
	const uint16_t jumpSelect = (cmdt->cmd_ctrl & 0x7000) >> 12;

	
	if(jumpSelect == 0)
	{
		//Jump Next, just go to next command (+32 bytes)
		*nextCmdt = (vdp1_cmdt_t*)(cmdt + 1);
	}
	else if(jumpSelect == 1)
	{
		//Jump assign, go straight to jumplink
		uint16_t jumpLink = cmdt->cmd_link;
		
		*nextCmdt = (vdp1_cmdt_t*)(cmdt_base + (jumpLink >> 5));
	}
	else if(jumpSelect == 2)
	{
		//Jump Call, subroutine call, find out what this is
		SubroutineCaller = (vdp1_cmdt_t*)(cmdt + 1);
		
		uint16_t jumpLink = cmdt->cmd_link;
		
		*nextCmdt = (vdp1_cmdt_t*)(cmdt_base + (jumpLink >> 5));
	}
	else if(jumpSelect == 3)
	{
		//Jump Return, subroutine return, jesus
		
		*nextCmdt = SubroutineCaller;
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


static uint32_t VDP1_DrawCmdt(DrawState_t* state, const vdp1_cmdt_t* cmdt, vdp1_cmdt_t** nextCmdt)
{
	if(!VDP1_GetNextCommand(cmdt, nextCmdt))
	{
		//Skip drawing this primitive
		return 1;
	}
	
	uint8_t ctrl = cmdt->cmd_ctrl & 0x000F;

	
	//Draw End Command
	if(cmdt->cmd_ctrl == 0x8000)
	{
		printf("Draw End\n");
		return 0;
	}
	//Set local coords
	else if(ctrl == 0x0A)
	{
		printf("set local coords\n");
		
		localCoordinates.x = cmdt->cmd_xa;
		localCoordinates.y = cmdt->cmd_ya;
		
		return 1;
	}
	//system clipping coordinate set
	else if(ctrl == 0x09)
	{
		printf("set system clipping\n");
		
		state->SystemClipMax.x = cmdt->cmd_xc;
		state->SystemClipMax.y = cmdt->cmd_yc;
		
		return 1;
	}
	//user clipping coordinates
	else if(ctrl == 0x08)
	{
		printf("user clipping coords\n");
		
		
		state->UserClipMin.x = cmdt->cmd_xa;
		state->UserClipMin.y = cmdt->cmd_ya;
		
		state->UserClipMax.x = cmdt->cmd_xc;
		state->UserClipMax.y = cmdt->cmd_yc;
		
		return 1;
	}
	
	
	//Actual render primitives
	Primitive_t prim;
	
	prim.flags.DrawPrimType = DRAW_POLYGON;
	prim.flags.DrawMesh = cmdt->cmd_draw_mode.mesh_enable;
	prim.flags.DrawGouraud = false;
	prim.flags.DrawPaletted = false;
	prim.flags.DrawTransparent = false;
	prim.flags.UserClipParameters = false; //cmdt->cmd_draw_mode.user_clipping_enable;
	
	
	
	//normal sprite draw
	if(ctrl == 0x00)
	{
		printf("Draw normal sprite\n");
		
		prim.flags.DrawTextured = true;

		int16_vec2_t dimensions = TEXTURE_SIZE_TO_VEC(cmdt->cmd_size);
		
		prim.v[0] = (Vertex_t){
			.x = cmdt->cmd_xa,
			.y = cmdt->cmd_ya,
		};

		//Probably use the character size for the dimensions
		
		prim.v[1] = (Vertex_t){
			.x = cmdt->cmd_xa + dimensions.x,
			.y = cmdt->cmd_ya,
		};
		
		prim.v[2] = (Vertex_t){
			.x = cmdt->cmd_xa + dimensions.x,
			.y = cmdt->cmd_ya + dimensions.y,
		};
		
		prim.v[3] = (Vertex_t){
			.x = cmdt->cmd_xa,
			.y = cmdt->cmd_ya + dimensions.y,
		};

		
	}
	//Scaled sprite draw
	else if(ctrl == 0x01)
	{
		printf("Draw scaled sprite\n");
		
		prim.flags.DrawTextured = true;

		
	}
	//Distorted sprite
	else if(ctrl == 0x02)
	{
		printf("Draw distorted sprite\n");
		
		prim.flags.DrawTextured = true;

		prim.v[0] = (Vertex_t){
			.x = cmdt->cmd_xa,
			.y = cmdt->cmd_ya,
		};
		
		prim.v[1] = (Vertex_t){
			.x = cmdt->cmd_xb,
			.y = cmdt->cmd_yb,
		};
		
		prim.v[2] = (Vertex_t){
			.x = cmdt->cmd_xc,
			.y = cmdt->cmd_yc,
		};
		
		prim.v[3] = (Vertex_t){
			.x = cmdt->cmd_xd,
			.y = cmdt->cmd_yd,
		};
		
	}
	//non-textured polygon
	else if(ctrl == 0x04)
	{
		printf("draw non-textured polygon\n");
		
		prim.flags.DrawTextured = false;
		
		CVECTOR colour = RGB1555_to_CVector((rgb1555_t)cmdt->cmd_colr); 
		
		prim.v[0] = (Vertex_t){
			.x = cmdt->cmd_xa,
			.y = cmdt->cmd_ya,
			.u = 1,
			.v = 1,
			.r = colour.r,
			.g = colour.g,
			.b = colour.b,
		};
		
		prim.v[1] = (Vertex_t){
			.x = cmdt->cmd_xb,
			.y = cmdt->cmd_yb,
			.u = 1,
			.v = 1,
			.r = colour.r,
			.g = colour.g,
			.b = colour.b,
		};
		
		prim.v[2] = (Vertex_t){
			.x = cmdt->cmd_xc,
			.y = cmdt->cmd_yc,
			.u = 1,
			.v = 1,
			.r = colour.r,
			.g = colour.g,
			.b = colour.b,
		};
		
		prim.v[3] = (Vertex_t){
			.x = cmdt->cmd_xd,
			.y = cmdt->cmd_yd,
			.u = 1,
			.v = 1,
			.r = colour.r,
			.g = colour.g,
			.b = colour.b,
		};
		
		
	}
	//non-textured polyline
	else if(ctrl == 0x05)
	{
		printf("Draw non-textured polyline\n");
		
		prim.flags.DrawPrimType = DRAW_POLYLINE;
		prim.flags.DrawTextured = false;
		
		prim.v[0] = (Vertex_t){
			.x = cmdt->cmd_xa,
			.y = cmdt->cmd_ya,
			.u = 1,
			.v = 1,
			.r = 255,
			.g = 255,
			.b = 255,
		};
		
		prim.v[1] = (Vertex_t){
			.x = cmdt->cmd_xb,
			.y = cmdt->cmd_yb,
			.u = 1,
			.v = 1,
			.r = 255,
			.g = 255,
			.b = 255,
		};
		
		prim.v[2] = (Vertex_t){
			.x = cmdt->cmd_xc,
			.y = cmdt->cmd_yc,
			.u = 1,
			.v = 1,
			.r = 255,
			.g = 255,
			.b = 255,
		};
		
		prim.v[3] = (Vertex_t){
			.x = cmdt->cmd_xd,
			.y = cmdt->cmd_yd,
			.u = 1,
			.v = 1,
			.r = 255,
			.g = 255,
			.b = 255,
		};
	}
	//non-textured line
	else if(ctrl == 0x06)
	{
		printf("Drawing non textured line\n");
		
		prim.flags.DrawPrimType = DRAW_LINE;
		prim.flags.DrawTextured = false;
		
		rgba8888_t col = {
			.r = 0,
			.g = 0,
			.b = 0,
		};
		
		prim.v[0] = (Vertex_t){
			.x = cmdt->cmd_xa,
			.y = cmdt->cmd_ya,
			.r = 255,
			.g = 255,
			.b = 255,
		};

		prim.v[1] = (Vertex_t){
			.x = cmdt->cmd_xb,
			.y = cmdt->cmd_yb,
			.r = 255,
			.g = 0,
			.b = 255,
		};
	}
	
	
	VDP1_DrawPrimitive(state, &prim);
	
	return 1;
}


bool VDP1_IsRendering()
{
	return IsVDP1Rendering;
}



static int VDP1_RenderThread(void* data)
{
	DrawState_t* state = (DrawState_t*)data;
	
	vdp1_cmdt_t* command = _state_vdp1()->vram_partitions.cmdt_base;
	vdp1_cmdt_t* nextCommand = NULL;			
	
	//We run a fixed size loop to ensure that there's no
	//infinite loops in the cmdt jump logic
	//In the future, we might end up doing loops on purpose
	//to speed up rendering but for right now this should work
	for(int i = 0; i < CMDT_MAX; i++)
	{
		if(!VDP1_DrawCmdt(state, command, &nextCommand))
		{
			break;
		}
		
		command = nextCommand;
	}
}


//We should figure out the best way to structure this so that 
//everything runs appropriately on the right cores
void VDP1_Render()
{
	vdp1_state_t* vdp1State = _state_vdp1();
	
	if(IsVDP1Rendering)
	{
		return;
	}

	IsVDP1Rendering = true;
		
	DrawState_t state = {
		.buffer = VDP1_GetDrawFramebuffer(),
		.bufferSize = vdp1State->framebufferDimensions,
		.textureBuffer = VDP1_GetTextureBuffer(),
		.colDepth = COL_ARGB_1555,
		.SystemClipMin = INT16_VEC2_INITIALIZER(0, 0),
		.SystemClipMax = INT16_VEC2_INITIALIZER(vdp1State->framebufferDimensions.x - 1, vdp1State->framebufferDimensions.y - 1),
		.UserClipMin = INT16_VEC2_INITIALIZER(0, 0),
		.UserClipMax = INT16_VEC2_INITIALIZER(vdp1State->framebufferDimensions.x - 1, vdp1State->framebufferDimensions.y - 1),
	};
	
	//We're segfaulting here in a weird way, I'm not gonna bother trying to figure it out any more
	/*
	for(int i = 0; i < NUM_THREADS; i++)
	{
		//Because the scanline raster is horizontally oriented, we 
		//divide the screen into vertical groups for max efficiency
		//TODO: this should be done better to ensure that there's no 
		//issues from rounding due to the vertical size of the framebuffer
		//not being evenly divisible by the number of threads
		state.SystemClipMin.y = (vdp1State->framebufferDimensions.y / NUM_THREADS) * i;
		state.SystemClipMax.y = (vdp1State->framebufferDimensions.y / NUM_THREADS) * (i + 1);
		
		threads[i] = SDL_CreateThread(VDP1_RenderThread, "VDP1_Render_" + i, (void*)&state);
	}
	*/
	
	
	
	VDP1_RenderThread((void*)&state);
	
	
	//printf("drew %d primitives\n", dl->PrimitiveCount);
}


void VDP1_WaitThreads()
{
	/*
	for(int i = 0; i < NUM_THREADS; i++)
	{
		SDL_WaitThread(threads[i], NULL);
	}
	*/
	
	IsVDP1Rendering = false;
}
