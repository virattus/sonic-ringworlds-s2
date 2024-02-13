#include <gfx/vdp2/vdp2_tile.h>



void vdp2_scrn_cell_format_set(const vdp2_scrn_cell_format_t* cell_format, const vdp2_scrn_normal_map_t* normal_map)
{
	vdp2_scrn_cell_ccc_set(cell_format);
	vdp2_scrn_char_size_set(cell_format);
	vdp2_scrn_plane_size_set(cell_format);
	vdp2_scrn_map_set(cell_format, 0, normal_map);
	vdp2_scrn_map_set(cell_format, 1, normal_map);
	vdp2_scrn_map_set(cell_format, 2, normal_map);
	vdp2_scrn_map_set(cell_format, 3, normal_map);
	vdp2_scrn_pnd_set(cell_format);
	
}


void vdp2_scrn_cell_ccc_set(const vdp2_scrn_cell_format_t* cell_format)
{
	//TODO writes cell_format->ccc to special registers, recreate this
	
	switch(cell_format->scroll_screen)
	{
		case VDP2_SCRN_NBG0:
		break;
		case VDP2_SCRN_NBG1:
		break;
		case VDP2_SCRN_NBG2:
		break;
		case VDP2_SCRN_NBG3:
		break;
		case VDP2_SCRN_RBG0: //These fall through, but might not in our implementation
		case VDP2_SCRN_RBG0_PA:
		case VDP2_SCRN_RBG0_PB:
		break;
		default:
		break;
	}
}


void vdp2_scrn_char_size_set(const vdp2_scrn_cell_format_t* cell_format)
{
	//TODO Same as above, but writes cell_format->char_size
	
	switch(cell_format->scroll_screen)
	{
		case VDP2_SCRN_NBG0:
		break;
		case VDP2_SCRN_NBG1:
		break;
		case VDP2_SCRN_NBG2:
		break;
		case VDP2_SCRN_NBG3:
		break;
		case VDP2_SCRN_RBG0: //These fall through, but might not in our implementation
		case VDP2_SCRN_RBG0_PA:
		case VDP2_SCRN_RBG0_PB:
		break;
		default:
		break;
	}
	
}


void vdp2_scrn_plane_size_set(const vdp2_scrn_cell_format_t* cell_format)
{
	const uint16_t plane_size = (cell_format->plane_size - 1) << 2;
	
	//Same as above again but with plane_size
	switch(cell_format->scroll_screen)
	{
		case VDP2_SCRN_NBG0:
		break;
		case VDP2_SCRN_NBG1:
		break;
		case VDP2_SCRN_NBG2:
		break;
		case VDP2_SCRN_NBG3:
		break;
		case VDP2_SCRN_RBG0: //These fall through, but might not in our implementation
		case VDP2_SCRN_RBG0_PA:
		case VDP2_SCRN_RBG0_PB:
		break;
		default:
		break;
	}
}


void vdp2_scrn_map_set(const vdp2_scrn_cell_format_t* cell_format, vdp2_scrn_plane_t plane, const vdp2_scrn_normal_map_t* normal_map)
{
	//TODO looks like we've got some pointer info
	vdp2_scrn_map_plane_set(cell_format, plane, normal_map->base_addr[plane]);
}


void vdp2_scrn_map_plane_set(const vdp2_scrn_cell_format_t* cell_format, vdp2_scrn_plane_t plane, vdp2_vram_t plane_base)
{
	//TODO Great news, this looks pretty complicated and unfinished, copying the yaul code directly
	
	//YAUL CODE HERE
	/*
	//A bit less clear than just dividing the plane address by the page
	//size to get the map bits
    uint16_t map_bits;
    map_bits = plane_base >> 11;

    const uint8_t page_config =
      (cell_format->char_size << 2) | cell_format->pnd_size;

    switch (page_config) {
    case ((VDP2_SCRN_CHAR_SIZE_2X2 << 2) | 1):
        break;
    case ((VDP2_SCRN_CHAR_SIZE_2X2 << 2) | 2):
        map_bits >>= 1;
        break;
    case ((VDP2_SCRN_CHAR_SIZE_1X1 << 2) | 1):
        map_bits >>= 2;
        break;
    case ((VDP2_SCRN_CHAR_SIZE_1X1 << 2) | 2):
        map_bits >>= 3;
        break;
    }

    map_bits &= 0x03FF;

    //Calculate the upper 3-bits of the 9-bits "map register", but only for
     * the first plane.
     *
     * Does this then imply that the all pattern name data for all planes
     * must be stored on the same VRAM bank?
    const uint16_t map_offset_bits = (map_bits >> 6) & 0x07;

    switch (cell_format->scroll_screen) {
    case VDP2_SCRN_NBG0:
        _state_vdp2()->shadow_regs.mpofn &= 0xFFF8;
        _state_vdp2()->shadow_regs.mpofn |= map_offset_bits;
        break;
    case VDP2_SCRN_NBG1:
        _state_vdp2()->shadow_regs.mpofn &= 0xFF8F;
        _state_vdp2()->shadow_regs.mpofn |= map_offset_bits << 4;
        break;
    case VDP2_SCRN_NBG2:
        _state_vdp2()->shadow_regs.mpofn &= 0xF8FF;
        _state_vdp2()->shadow_regs.mpofn |= map_offset_bits << 8;
        break;
    case VDP2_SCRN_NBG3:
        _state_vdp2()->shadow_regs.mpofn &= 0x8FFF;
        _state_vdp2()->shadow_regs.mpofn |= map_offset_bits << 12;
        break;
    case VDP2_SCRN_RBG0:
    case VDP2_SCRN_RBG0_PA:
        _state_vdp2()->shadow_regs.mpofr &= 0xFFF8;
        _state_vdp2()->shadow_regs.mpofr |= map_offset_bits;
        break;
    case VDP2_SCRN_RBG0_PB:
        _state_vdp2()->shadow_regs.mpofr &= 0xFF8F;
        _state_vdp2()->shadow_regs.mpofr |= map_offset_bits << 4;
        break;
    default:
        break;
    }

    uint16_t *mp_reg;
    mp_reg = (uint16_t *)&_state_vdp2()->shadow_regs.mpabn0;

    switch (cell_format->scroll_screen) {
    case VDP2_SCRN_NBG0:
        break;
    case VDP2_SCRN_NBG1:
        mp_reg += 2;
        break;
    case VDP2_SCRN_NBG2:
        mp_reg += 4;
        break;
    case VDP2_SCRN_NBG3:
        mp_reg += 6;
        break;
    case VDP2_SCRN_RBG0:
    case VDP2_SCRN_RBG0_PA:
        mp_reg += 8;
        break;
    case VDP2_SCRN_RBG0_PB:
        mp_reg += 16;
        break;
    default:
        break;
    }

    //Choose which of the two map plane registers
    mp_reg += (plane >> 1);

    const uint16_t plane_bits = map_bits & 0x003F;

    //Select between first or second page within the register
    if ((plane & 1) == 0) {
        *mp_reg &= 0xFFC0;
        *mp_reg |= plane_bits;
    } else {
        *mp_reg &= 0xC0FF;
        *mp_reg |= plane_bits << 8;
    }
    */
}


void vdp2_scrn_pnd_set(const vdp2_scrn_cell_format_t* cell_format)
{
	//TODO this looks even more complicated, not even going to bother with copy pasting
}

