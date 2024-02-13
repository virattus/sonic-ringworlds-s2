#ifndef __FAKEYAUL_VDP2_SCREEN_FUNCS_H__
#define __FAKEYAUL_VDP2_SCREEN_FUNCS_H__


#include <gfx/vdp2/vdp2_scrn.h>

#include <libfixmath/fix16.h>

#include <stdint.h>


typedef enum vdp2_scrn_ls_type
{
	VDP2_SCRN_LS_TYPE_HORZ = 0x01,
	VDP2_SCRN_LS_TYPE_VERT = 0x02,
	VDP2_SCRN_LS_TYPE_ZOOM_HORZ = 0x04,
	
} vdp2_scrn_ls_type_t;


typedef enum vdp2_scrn_coff
{
	VDP2_SCRN_COFF_A,
	VDP2_SCRN_COFF_B,
	
} vdp2_scrn_coff_t;


typedef enum vdp2_scrn_reduction
{
	VDP2_SCRN_REDUCTION_NONE,
	VDP2_SCRN_REDUCTION_HALF,
	VDP2_SCRN_REDUCTION_QUARTER,
	
} vdp2_scrn_reduction_t;


typedef enum vdp2_scrn_sf_mode
{
	VDP2_SCRN_SF_MODE_0,
	VDP2_SCRN_SF_MODE_1,
	VDP2_SCRN_SF_MODE_2,
	
} vdp2_scrn_sf_mode_t;


typedef enum vdp2_scrn_sf_type
{
	VDP2_SCRN_SF_TYPE_NONE = 0,
	VDP2_SCRN_SF_TYPE_CELL_CC = 0x10,
	VDP2_SCRN_SF_TYPE_CELL_PRIORITY = 0x20,
	VDP2_SCRN_SF_TYPE_BITMAP_CC = 0x18,
	VDP2_SCRN_SF_TYPE_BITMAP_PRIORITY = 0x28,
	
} vdp2_scrn_sf_type_t;


typedef enum vdp2_scrn_sf_code
{
	VDP2_SCRN_SF_CODE_A,
	VDP2_SCRN_SF_CODE_B,
	
} vdp2_scrn_sf_code_t;


//TODO figure out what the fuck this is supposed to mean
typedef enum vdp2_scrn_sf_code_range
{
	VDP2_SCRN_SF_CODE_0x00_0x01 = (1 << 0),
	VDP2_SCRN_SF_CODE_0x02_0x03 = (1 << 1),
	VDP2_SCRN_SF_CODE_0x04_0x05 = (1 << 2),
	VDP2_SCRN_SF_CODE_0x06_0x07 = (1 << 3),
	VDP2_SCRN_SF_CODE_0x08_0x09 = (1 << 4),
	VDP2_SCRN_SF_CODE_0x0A_0x0B = (1 << 5),
	VDP2_SCRN_SF_CODE_0x0C_0x0D = (1 << 6),
	VDP2_SCRN_SF_CODE_0x0E_0x0F = (1 << 7),
	
} vdp2_scrn_sf_code_range_t;





#define VDP2_SCRN_REDUCTION_STEP	FIX16(1.0f / 256.0f)
#define VDP2_SCRN_REDUCTION_MIN		VDP2_SCRN_REDUCTION_STEP
#define VDP2_SCRN_REDUCTION_MAX		FIX16(7.0f)


typedef struct vdp2_scrn_ls_format
{
	vdp2_scrn_t scroll_screen;
	vdp2_vram_t table_base; //Line Scroll Table base address (offset)
	uint8_t interval;
	vdp2_scrn_ls_type_t type;
	
} vdp2_scrn_ls_format_t;


typedef struct vdp2_scrn_ls_h
{
	fix16_t horz;
	
} vdp2_scrn_ls_h_t;


typedef struct vdp2_scrn_ls_v
{
	fix16_t vert;
	
} vdp2_scrn_ls_v_t;


typedef struct vdp2_scrn_ls_hv
{
	fix16_t horz;
	fix16_t vert;
	
} vdp2_scrn_ls_hv_t;


typedef struct vdp2_scrn_ls_hvz
{
	fix16_t horz;
	fix16_t vert;
	fix16_t horz_incr;
	
} vdp2_scrn_ls_hvz_t;


typedef struct vdp2_scrn_vcs_format
{
	vdp2_scrn_t scroll_screen;
	vdp2_vram_t table_base;
	
} vdp2_scrn_vcs_format_t;


typedef struct vdp2_scrn_coff_rgb
{
	int16_t r;
	int16_t g;
	int16_t b;
	
} vdp2_scrn_coff_rgb_t;




#endif
