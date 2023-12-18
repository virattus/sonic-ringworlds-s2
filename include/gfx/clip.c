#include <gfx/clip.h>

#include <gamemath/vector.h>

/*
 * Thanks to PsxN00bSDK for this algorithm
 */


#define CLIP_LEFT 	1
#define CLIP_RIGHT 	2
#define CLIP_TOP 	4 
#define CLIP_BOTTOM 8



int32_t test_clip(const RECT* clip, const int16_vec3_t* v)
{
	int32_t result = 0;
	
	if( v->x < clip->min.x )
	{
		result |= CLIP_LEFT;
	}
	
	if( v->x >= clip->max.x )
	{
		result |= CLIP_RIGHT;
	}
	
	if( v->y < clip->min.y )
	{
		result |= CLIP_TOP;
	}
	
	if( v->y >= clip->max.y )
	{
		result |= CLIP_BOTTOM;
	}
	
	return result;
}



int32_t QuadVisible(const int16_vec3_t* poly, const RECT* rect)
{
	int32_t result[4];
	
	result[0] = test_clip(rect, &poly[0]);
	result[1] = test_clip(rect, &poly[1]);
	result[2] = test_clip(rect, &poly[2]);
	result[3] = test_clip(rect, &poly[3]);
	
	if((result[0] & result[1]) == 0)
		return 0;
	if((result[1] & result[2]) == 0)
		return 0;
	if((result[2] & result[3]) == 0)
		return 0;
	if((result[3] & result[0]) == 0)
		return 0;
	if((result[0] & result[2]) == 0)
		return 0;
	if((result[1] & result[3]) == 0)
		return 0;	

	return 1;
}
