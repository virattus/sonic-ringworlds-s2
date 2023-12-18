#ifndef __FAKEYAUL_VDP2_TVMD_H__
#define __FAKEYAUL_VDP2_TVMD_H__


typedef enum vdp2_tvmd_interlace 
{
    VDP2_TVMD_INTERLACE_NONE   = 0,
    VDP2_TVMD_INTERLACE_SINGLE = 1,
    VDP2_TVMD_INTERLACE_DOUBLE = 2
    
} vdp2_tvmd_interlace_t;


//Thanks YAUL
typedef enum vdp2_tvmd_horz 
{
    VDP2_TVMD_HORZ_NORMAL_A  = 0, /* 320x */
    VDP2_TVMD_HORZ_NORMAL_B  = 1, /* 352x */
    VDP2_TVMD_HORZ_HIRESO_A  = 2, /* 640x */
    VDP2_TVMD_HORZ_HIRESO_B  = 3, /* 704x */
    VDP2_TVMD_HORZ_NORMAL_AE = 4, /* 320x480 */
    VDP2_TVMD_HORZ_NORMAL_BE = 5, /* 352x480 */
    VDP2_TVMD_HORZ_HIRESO_AE = 6, /* 640x480 */
    VDP2_TVMD_HORZ_HIRESO_BE = 7  /* 704x480 */
    
} vdp2_tvmd_horz_t;


typedef enum vdp2_tvmd_vert 
{
    VDP2_TVMD_VERT_224 = 0,
    VDP2_TVMD_VERT_240 = 1,
    VDP2_TVMD_VERT_256 = 2
    
} vdp2_tvmd_vert_t;



void vdp2_tvmd_display_set(void);

void vdp2_tvmd_display_res_set(vdp2_tvmd_interlace_t interlace, vdp2_tvmd_horz_t horizontal, vdp2_tvmd_vert_t vertical);

#endif
