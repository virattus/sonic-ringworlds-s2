#ifndef __FAKEYAUL_CELL_H__
#define __FAKEYAUL_CELL_H__


#include <core/preprocessor.h>
#include <gfx/vdp2/vdp2_vram.h>
#include <gfx/vdp2/vdp2_scrn.h>
#include <gfx/vdp2/vdp2_cram.h>

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>


typedef enum vdp2_scrn_char_size
{
	VDP2_SCRN_CHAR_SIZE_1X1,
	VDP2_SCRN_CHAR_SIZE_2X2,
	
} vdp2_scrn_char_size_t;


typedef enum vdp2_scrn_plane_size
{
	VDP2_SCRN_PLANE_SIZE_1X1 = 1 * 1,
	VDP2_SCRN_PLANE_SIZE_2X1 = 2 * 1,
	VDP2_SCRN_PLANE_SIZE_2X2 = 2 * 2,
	
} vdp2_scrn_plane_size_t;


typedef enum vdp2_scrn_plane
{
	VDP2_SCRN_PLANE_A,
	VDP2_SCRN_PLANE_B,
	VDP2_SCRN_PLANE_C,
	VDP2_SCRN_PLANE_D,
	VDP2_SCRN_PLANE_E,
	VDP2_SCRN_PLANE_F,
	VDP2_SCRN_PLANE_G,
	VDP2_SCRN_PLANE_H,
	VDP2_SCRN_PLANE_I,
	VDP2_SCRN_PLANE_J,
	VDP2_SCRN_PLANE_K,
	VDP2_SCRN_PLANE_L,
	VDP2_SCRN_PLANE_M,
	VDP2_SCRN_PLANE_N,
	VDP2_SCRN_PLANE_O,
	VDP2_SCRN_PLANE_P,
	
} vdp2_scrn_plane_t;


//These define a flip function? 0 == flip, 1 == no flip
typedef enum vdp2_scrn_aux_mode
{
	VDP2_SCRN_AUX_MODE_0,
	VDP2_SCRN_AUX_MODE_1,
	
} vdp2_scrn_aux_mode_t;


typedef struct vdp2_scrn_cell_format
{
	vdp2_scrn_t scroll_screen;
	vdp2_scrn_ccc_t ccc;
	vdp2_scrn_char_size_t char_size;
	uint8_t pnd_size;
	vdp2_scrn_aux_mode_t aux_mode;
	
	vdp2_scrn_plane_size_t plane_size;
	
	vdp2_vram_t cpd_base;
	vdp2_cram_t palette_base;
	
} vdp2_scrn_cell_format_t;


typedef union vdp2_scrn_normal_map
{
	vdp2_vram_t base_addr[4];
	
	struct
	{
		vdp2_vram_t plane_a;
		vdp2_vram_t plane_b;
		vdp2_vram_t plane_c;
		vdp2_vram_t plane_d;
		
	} __packed;
	
} vdp2_scrn_normal_map_t;


typedef struct vdp2_scrn_rotation_map
{
	union
	{
		vdp2_vram_t base_addr[16];
		struct
		{
			union
			{
				vdp2_vram_t plane;
				vdp2_vram_t plane_a;
			};
			
			vdp2_vram_t plane_b;
			vdp2_vram_t plane_c;
			vdp2_vram_t plane_d;
			vdp2_vram_t plane_e;
			vdp2_vram_t plane_f;
			vdp2_vram_t plane_g;
			vdp2_vram_t plane_h;
			vdp2_vram_t plane_i;
			vdp2_vram_t plane_j;
			vdp2_vram_t plane_k;
			vdp2_vram_t plane_l;
			vdp2_vram_t plane_m;
			vdp2_vram_t plane_n;
			vdp2_vram_t plane_o;
			vdp2_vram_t plane_p;	
		} __packed;
	} __packed;
	
	bool single;
} __aligned(4) vdp2_scrn_rotation_map_t;


void vdp2_scrn_cell_format_set(const vdp2_scrn_cell_format_t* cell_format, const vdp2_scrn_normal_map_t* normal_map);
void vdp2_scrn_cell_ccc_set(const vdp2_scrn_cell_format_t* cell_format);
void vdp2_scrn_char_size_set(const vdp2_scrn_cell_format_t* cell_format);
void vdp2_scrn_plane_size_set(const vdp2_scrn_cell_format_t* cell_format);
void vdp2_scrn_map_set(const vdp2_scrn_cell_format_t* cell_format, vdp2_scrn_plane_t plane, const vdp2_scrn_normal_map_t* normal_map);
void vdp2_scrn_map_plane_set(const vdp2_scrn_cell_format_t* cell_format, vdp2_scrn_plane_t plane, vdp2_vram_t plane_base);
void vdp2_scrn_pnd_set(const vdp2_scrn_cell_format_t* cell_format);



#endif
