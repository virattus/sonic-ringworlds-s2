#include <gfx/vdp2/vdp2_mode7.h>



void DrawMode7Bitmap()
{
/*



	float fNear = 0.03f;
	float fFar = 0.03f;
	float fFoVHalf = 3.1415926535 / 4.0f;
	int Mode7Top = 0;
	int Mode7Bottom = SCREEN_HEIGHT;
	uint8_t alpha = 0;

void Mode7(SDL_Surface* tex, int16_vec2_t* texDim, int32_vec3_t* pos, int32_t ang)
{
	VRAM* dest = VDP1_GetDisplayFramebuffer();
	uint32_t* pixels = (uint32_t*)dest->Memory;
	uint32_t* texPixels = (uint32_t*)tex->pixels;
	const uint8_t* kb = SDL_GetKeyboardState(NULL);

	float fAng = ang / 60.0f;
	
	
	if(kb[SDL_SCANCODE_J])
		fNear -= 0.5f;
	if(kb[SDL_SCANCODE_K])
		fNear += 0.5f;
	if(kb[SDL_SCANCODE_I])
		fFar += 0.5f;
	if(kb[SDL_SCANCODE_O])
		fFar -= 0.5f;
	
	if(kb[SDL_SCANCODE_N])
		fFoVHalf += 0.005f;
	if(kb[SDL_SCANCODE_M])
		fFoVHalf -= 0.005f;
		
	if(kb[SDL_SCANCODE_Z])
		Mode7Top = clamp((Mode7Top + 1) % 256, 1, SCREEN_HEIGHT);
	if(kb[SDL_SCANCODE_X])
		Mode7Bottom = clamp((Mode7Bottom + 1) % 256, 0, SCREEN_HEIGHT);
	
	float fFarX1 = pos->x + (cosf(fAng - fFoVHalf) * fFar);
	float fFarY1 = pos->y + (sinf(fAng - fFoVHalf) * fFar);
	
	float fFarX2 = pos->x + cosf(fAng + fFoVHalf) * fFar;
	float fFarY2 = pos->y + sinf(fAng + fFoVHalf) * fFar;	
	
	float fNearX1 = pos->x + cosf(fAng - fFoVHalf) * fNear;
	float fNearY1 = pos->y + sinf(fAng - fFoVHalf) * fNear;
	
	float fNearX2 = pos->x + cosf(fAng + fFoVHalf) * fNear;
	float fNearY2 = pos->y + sinf(fAng + fFoVHalf) * fNear;

	for(int y = Mode7Top; y < Mode7Bottom; y++)
	{
		float fSampleDepth = (float)y / (Mode7Bottom + 0.00001f);
		
		float fStartX = (fFarX1 - fNearX1) / (fSampleDepth) + fNearX1;
		float fStartY = (fFarY1 - fNearY1) / (fSampleDepth) + fNearY1;
		
		float fEndX = (fFarX2 - fNearX2) / (fSampleDepth) + fNearX2;
		float fEndY = (fFarY2 - fNearY2) / (fSampleDepth) + fNearY2; 
		
		for(int x = 0; x < SCREEN_WIDTH; x++)
		{
			float fSampleWidth = (float)x / (float)SCREEN_WIDTH;
			int SampleX = (fEndX - fStartX) * fSampleWidth + fStartX;
			int SampleY = (fEndY - fStartY) * fSampleWidth + fStartY;
		
			int SamplePos = abs(SampleY * texDim->x + SampleX) % (texDim->x * texDim->y);

			uint32_t col = texPixels[SamplePos];
			//col &= 0xFFFFFF10; YES WE CAN USE THE ALPHA TO SET TRANSPARENCY
			pixels[y * (SCREEN_WIDTH) + x] = col;
		}
	}
}

 
*/
}


void DrawMode7Tilemap(vdp2_tilemap_t* tilemap)
{
}
