#include <gfx/vdp2/vdp2_screen_back.h>

#include <gfx/vdp2/vdp2.h>

#include <assert.h>


static inline void _back_set(vdp2_vram_t vram, const rgb1555_t* buffer, uint32_t count)
{
	//unlike the real thing, we just write whatever we get and just get the line's colour by modulus of count ( 1 % 224 = 1)
	//Also note that count is measured in entries, not bytes like the real one
	
	_state_vdp2()->back.vram = vram;
	_state_vdp2()->back.buffer = buffer;
	_state_vdp2()->back.len = count;
	
	if(count == 1)
	{
		//write out the value if it's a single value
		VRAM_WriteShort(VDP2_GetTextureBuffer(), vram >> 1, buffer[0].raw);
	}
}



//TODO reading lines from this
rgb1555_t vdp2_scrn_back_colour_get(uint32_t lineNum)
{
	assert(_state_vdp2()->back.len != 0);
	
	const uint32_t lineoffset = (sizeof(rgb1555_t) * (lineNum % _state_vdp2()->back.len));
	
	return *(rgb1555_t*)(VDP2_GetTextureBuffer()->Memory + _state_vdp2()->back.vram + lineoffset);
}


void vdp2_scrn_back_color_set(vdp2_vram_t vram, rgb1555_t colour)
{
	assert(vram < VDP2_GetTextureBuffer()->Size);
	
	static rgb1555_t buffer = RGB1555_INITIALIZER(1, 0, 0, 0);
	
	buffer = colour;
	
	_back_set(vram, &buffer, 1);
}


void vdp2_scrn_back_buffer_set(vdp2_vram_t vram, const rgb1555_t* buffer, uint32_t count)
{
	assert(buffer != NULL);
	assert(count > 0);
	
	_back_set(vram, buffer, count);
}


void vdp2_scrn_back_sync(void)
{
	if(_state_vdp2()->back.len == 0)
	{
		return;
	}
	
	//copy over buffer
	memcpy(VDP2_GetTextureBuffer()->Memory + _state_vdp2()->back.vram, _state_vdp2()->back.buffer, sizeof(rgb1555_t) * _state_vdp2()->back.len);
}
