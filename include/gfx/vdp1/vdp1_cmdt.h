#ifndef __FAKEYAUL_VDP1_CMD_T_H__
#define __FAKEYAUL_VDP1_CMD_T_H__


#include <core/preprocessor.h>
#include <core/colour.h>
#include <gfx/vdp1/vdp1_vram.h>
#include <gfx/vdp2/vdp2_sprite.h>

#include <stdint.h>
#include <stdbool.h>


//Replace this with a more global option
#define CMDT_MAX					2048



typedef uint16_t texture_slot_t;
typedef uint16_t gst_slot_t;


typedef enum VDP1_CMDT_COMMAND
{
	VDP1_CMDT_NORMAL_SPRITE 	= 0,
	VDP1_CMDT_SCALED_SPRITE 	= 1,
	VDP1_CMDT_DISTORTED_SPRITE 	= 2,
	VDP1_CMDT_POLYGON 			= 4,
	VDP1_CMDT_POLYLINE 			= 5,
	VDP1_CMDT_LINE 				= 6,
	VDP1_CMDT_USER_CLIP_COORD 	= 8,
	VDP1_CMDT_SYSTEM_CLIP_COORD = 9,
	VDP1_CMDT_LOCAL_COORD 		= 10,
	
} vdp1_cmdt_command_t;


typedef enum _sort_type
{
	SORT_TYPE_BFR,
	SORT_TYPE_MIN,
	SORT_TYPE_MAX,
	SORT_TYPE_CENTER,
	
} sort_type_t;


typedef enum _plane_type
{
	PLANE_TYPE_SINGLE,
	PLANE_TYPE_DOUBLE,
	
} plane_type_t;


typedef enum _read_dir //This handles the orientation of the texture on the face of the polygon
{
	READ_DIR_NORMAL = 0,
	READ_DIR_H = 1,
	READ_DIR_V = 2,
	READ_DIR_HV = 3,
	
} read_dir_t;


typedef enum _vdp1_cmdt_char_flip
{
	VDP1_CMDT_CHAR_FLIP_NONE = 0,
	VDP1_CMDT_CHAR_FLIP_H	= 1 << 4,
	VDP1_CMDT_CHAR_FLIP_V	= 1 << 5,
	VDP1_CMDT_CHAR_FLIP_HV	= VDP1_CMDT_CHAR_FLIP_H | VDP1_CMDT_CHAR_FLIP_V,
	
} vdp1_cmdt_char_flip_t;


typedef uint16_t vdp1_link_t;


typedef vdp2_sprite_type_0_t vdp1_color_bank_type_0_t;
typedef vdp2_sprite_type_1_t vdp1_color_bank_type_1_t;
typedef vdp2_sprite_type_2_t vdp1_color_bank_type_2_t;
typedef vdp2_sprite_type_3_t vdp1_color_bank_type_3_t;
typedef vdp2_sprite_type_4_t vdp1_color_bank_type_4_t;
typedef vdp2_sprite_type_5_t vdp1_color_bank_type_5_t;
typedef vdp2_sprite_type_6_t vdp1_color_bank_type_6_t;
typedef vdp2_sprite_type_7_t vdp1_color_bank_type_7_t;

typedef vdp2_sprite_type_8_t vdp1_color_bank_type_8_t;
typedef vdp2_sprite_type_9_t vdp1_color_bank_type_9_t;
typedef vdp2_sprite_type_a_t vdp1_color_bank_type_a_t;
typedef vdp2_sprite_type_b_t vdp1_color_bank_type_b_t;
typedef vdp2_sprite_type_c_t vdp1_color_bank_type_c_t;
typedef vdp2_sprite_type_d_t vdp1_color_bank_type_d_t;
typedef vdp2_sprite_type_e_t vdp1_color_bank_type_e_t;
typedef vdp2_sprite_type_f_t vdp1_color_bank_type_f_t;



typedef enum _command_type
{
	COMMAND_TYPE_SPRITE 			= 0,
	COMMAND_TYPE_SCALED_SPRITE 		= 1,
	COMMAND_TYPE_DISTORTED_SPRITE 	= 2,
	COMMAND_TYPE_POLYGON 			= 4,
	COMMAND_TYPE_POLYLINE 			= 5,
	COMMAND_TYPE_LINE 				= 6,
	
} command_type_t;


typedef enum vdp1_cmdt_link_type 
{
    VDP1_CMDT_LINK_TYPE_JUMP_NEXT   = 0,
    VDP1_CMDT_LINK_TYPE_JUMP_ASSIGN = 1 << 12,
    VDP1_CMDT_LINK_TYPE_JUMP_CALL   = 2 << 12,
    VDP1_CMDT_LINK_TYPE_JUMP_RETURN = 3 << 12,
    VDP1_CMDT_LINK_TYPE_SKIP_NEXT   = 4 << 12,
    VDP1_CMDT_LINK_TYPE_SKIP_ASSIGN = 5 << 12,
    VDP1_CMDT_LINK_TYPE_SKIP_CALL   = 6 << 12,
    VDP1_CMDT_LINK_TYPE_SKIP_RETURN = 7 << 12
    
} vdp1_cmdt_link_type_t;


typedef enum vdp1_cmdt_cm 
{
    VDP1_CMDT_CM_CB_16     = 0,
    VDP1_CMDT_CM_CLUT_16   = 1, //turns out vdp1 has a proper PSX style render mode, just only for 16 colour
    VDP1_CMDT_CM_CB_64     = 2,
    VDP1_CMDT_CM_CB_128    = 3,
    VDP1_CMDT_CM_CB_256    = 4,
    VDP1_CMDT_CM_RGB_32768 = 5
    
} vdp1_cmdt_cm_t;


typedef union VDP1_CMDT_DRAW_MODE
{
	struct
	{
		unsigned int msb_enable :1;
		unsigned int unknown0 :2;
		unsigned int hss_enable :1;
		unsigned int pre_clipping_disable :1;
		unsigned int user_clipping_enable :1;
		unsigned int user_clipping_mode :1;
		unsigned int mesh_enable :1;
		unsigned int end_code_disable :1;
		unsigned int trans_pixel_disable :1;
		unsigned int color_mode :3; //TODO replace with struct
		unsigned int cc_mode :3; //TODO replace with struct
	} __packed;
	uint16_t raw;
	
} __packed vdp1_cmdt_draw_mode_t;


typedef union VDP1_CMDT_COLOUR_BANK {
	vdp1_color_bank_type_0_t type_0;
	vdp1_color_bank_type_0_t type_1;
	vdp1_color_bank_type_0_t type_2;
	vdp1_color_bank_type_0_t type_3;
	vdp1_color_bank_type_0_t type_4;
	vdp1_color_bank_type_0_t type_5;
	vdp1_color_bank_type_0_t type_6;
	vdp1_color_bank_type_0_t type_7;
	
	struct {
		unsigned int :8;
		vdp1_color_bank_type_8_t type_8;
	};
	
	struct {
		unsigned int :8;
		vdp1_color_bank_type_9_t type_9;
	};
	
	struct {
		unsigned int :8;
		vdp1_color_bank_type_a_t type_a;
	};
	
	struct {
		unsigned int :8;
		vdp1_color_bank_type_b_t type_b;
	};
	
	struct {
		unsigned int :8;
		vdp1_color_bank_type_c_t type_c;
	};
	
	struct {
		unsigned int :8;
		vdp1_color_bank_type_d_t type_d;
	};
	
	struct {
		unsigned int :8;
		vdp1_color_bank_type_e_t type_e;
	};
	
	struct {
		unsigned int :8;
		vdp1_color_bank_type_f_t type_f;
	};
	
	uint16_t raw;
	
} __packed vdp1_cmdt_color_bank_t;


//This gets redefined for some reason
//Even though this is part of libmic3d, we'll define it here for convenience
typedef enum link_type 
{
    LINK_TYPE_JUMP_NEXT   = 0,
    LINK_TYPE_JUMP_ASSIGN = 1,
    LINK_TYPE_JUMP_CALL   = 2,
    LINK_TYPE_JUMP_RETURN = 3,
    LINK_TYPE_SKIP_NEXT   = 4,
    LINK_TYPE_SKIP_ASSIGN = 5,
    LINK_TYPE_SKIP_CALL   = 6,
    LINK_TYPE_SKIP_RETURN = 7
    
} link_type_t;


typedef union _indices 
{
	struct {
		uint16_t p0;
		uint16_t p1;
		uint16_t p2;
		uint16_t p3;
	};
	
	uint16_t p[4];
	
} __aligned(4) indices_t;


typedef union VDP1_FLAGS
{
	unsigned int unknown0 :11;
	sort_type_t sort_type :3;
	plane_type_t plane_type :1;
	unsigned int use_texture :1;
	
}__packed __aligned(2) flags_t;


typedef struct VDP1_ATTRIBUTE
{
	union
	{
		struct
		{
			unsigned int unknown0	:1;
			link_type_t link_type	:3;
			unsigned int unknown1	:6;
			read_dir_t	read_dir	:2;
			command_type_t command	:4;
		} __packed;
		
		uint16_t raw;
	} __packed __aligned(2) control;
	
	vdp1_cmdt_draw_mode_t draw_mode;
	
	union
	{
		uint16_t vram_index;
		uint16_t color_bank; //TODO replace with struct
		uint16_t raw;
		rgb1555_t base_color;
	} palette_data;
	
	texture_slot_t texture_slot;
	gst_slot_t shading_slot; //Gouraud Shading slot
	
} __aligned(4) attribute_t;


//So, on saturn this is a 10 byte struct
//but on x86, it's forced to be 12 bytes
//therefore, we need to convert the polygon object on import
typedef struct VDP1_POLYGON
{
	flags_t flags;
	indices_t indices;
	
} polygon_t;


typedef struct VDP1_CMDT
{
	uint16_t cmd_ctrl;
	uint16_t cmd_link;
	
	union {
		uint16_t cmd_pmod;
		
		vdp1_cmdt_draw_mode_t cmd_draw_mode;
	};
	
	uint16_t cmd_colr;
	uint16_t cmd_srca;
	uint16_t cmd_size;
	
	union
	{
		struct
		{
			int16_t cmd_xa;
			int16_t cmd_ya;
			int16_t cmd_xb;
			int16_t cmd_yb;
			int16_t cmd_xc;
			int16_t cmd_yc;
			int16_t cmd_xd;
			int16_t cmd_yd;
		} __packed;
		
		int16_vec2_t cmd_vertices[4];
	};
	
	uint16_t cmd_grda;
	
	uint16_t reserved;
	
} vdp1_cmdt_t;


typedef struct VDP1_CMDT_LIST
{
	vdp1_cmdt_t* cmdts;
	uint16_t count;
	
} vdp1_cmdt_list_t;


typedef struct VDP1_CMDT_ORDERLIST
{
	vdp1_cmdt_t* cmdt;
	
} vdp1_cmdt_orderlist_t;



vdp1_cmdt_list_t* vdp1_cmdt_list_alloc(uint16_t count);
void vdp1_cmdt_list_free(vdp1_cmdt_list_t* cmdt_list);
void vdp1_cmdt_list_init(vdp1_cmdt_list_t* cmdt_list, vdp1_cmdt_t* cmdts);


static inline void vdp1_cmdt_draw_mode_set(vdp1_cmdt_t* cmdt, vdp1_cmdt_draw_mode_t draw_mode)
{
	//This command is incorrect for little endian, it's assigning to mesh_enable, figure out what bits its actually trying to set
	
	//Straight from yaul
    /* Values 0x4, 0x5, 0x6 for comm indicate a non-textured command table,
     * and we want to set the bits 7 and 6 without branching */
	
	const uint16_t comm = (cmdt->cmd_ctrl & 0x0004);
	const uint16_t pmod_bits = (comm << 5) | (comm << 4);
	
	//cmdt->cmd_pmod = pmod_bits | draw_mode.raw;
	
	//For now we'll just bypass the saturn specific code
	cmdt->cmd_pmod |= draw_mode.raw;
}


static inline void vdp1_cmdt_command_set(vdp1_cmdt_t* cmdt, vdp1_cmdt_command_t command)
{
	cmdt->cmd_ctrl &= 0x7FF0;
	cmdt->cmd_ctrl |= command;
}


static inline void vdp1_cmdt_colour_set(vdp1_cmdt_t* cmdt, rgb1555_t colour)
{
	cmdt->cmd_colr = colour.raw;
}


static inline void vdp1_cmdt_color_bank_set(vdp1_cmdt_t* cmdt, const vdp1_cmdt_color_bank_t color_bank)
{
	cmdt->cmd_colr = color_bank.raw;
}


static inline void vdp1_cmdt_color_mode0_set(vdp1_cmdt_t* cmdt, const vdp1_cmdt_color_bank_t color_bank)
{
	cmdt->cmd_pmod &= 0xFFC7;
	cmdt->cmd_colr = color_bank.raw & 0xFFF0;
}

//The fact that this one's so different makes me think this is for cluts
static inline void vdp1_cmdt_color_mode1_set(vdp1_cmdt_t* cmdt, const vdp1_vram_t base)
{
	cmdt->cmd_pmod &= 0xFFC7;
	cmdt->cmd_pmod |= 0x0008;
	cmdt->cmd_colr = (uint16_t)((base >> 3) & 0xFFFF);
}


static inline void vdp1_cmdt_color_mode2_set(vdp1_cmdt_t* cmdt, const vdp1_cmdt_color_bank_t color_bank)
{
	cmdt->cmd_pmod &= 0xFFC7;
	cmdt->cmd_pmod |= 0x0010;
	cmdt->cmd_colr = color_bank.raw & 0xFFF0;
}


static inline void vdp1_cmdt_color_mode3_set(vdp1_cmdt_t* cmdt, const vdp1_cmdt_color_bank_t color_bank)
{
	cmdt->cmd_pmod &= 0xFFC7;
	cmdt->cmd_pmod |= 0x0018;
	cmdt->cmd_colr = color_bank.raw & 0xFF80;
}


static inline void vdp1_cmdt_color_mode4_set(vdp1_cmdt_t* cmdt, const vdp1_cmdt_color_bank_t color_bank)
{
	cmdt->cmd_pmod &= 0xFFC7;
	cmdt->cmd_pmod |= 0x0020;
	cmdt->cmd_colr = color_bank.raw & 0xFF00;
}


static inline void vdp1_cmdt_char_base_set(vdp1_cmdt_t* cmdt, vdp1_vram_t base)
{
	cmdt->cmd_srca = (base >> 3) & 0xFFFF;
}


static inline void vdp1_cmdt_char_size_set(vdp1_cmdt_t* cmdt, uint16_t width, uint16_t height)
{
	cmdt->cmd_size = (((width >> 3) << 8) | height) & 0x3FFF;
}


static inline void vdp1_cmdt_char_flip_set(vdp1_cmdt_t* cmdt, vdp1_cmdt_char_flip_t flip)
{
	cmdt->cmd_ctrl &= 0xFFCF;
	cmdt->cmd_ctrl |= flip;
}


static inline void vdp1_cmdt_vtx_zoom_point_set(vdp1_cmdt_t* cmdt, int16_vec2_t p)
{
	cmdt->cmd_vertices[0] = p;
}

static inline void vdp1_cmdt_vtx_zoom_display_set(vdp1_cmdt_t* cmdt, int16_vec2_t p)
{
	cmdt->cmd_vertices[1] = p;
}


static inline void vdp1_cmdt_vtx_scale_set(vdp1_cmdt_t* cmdt, int16_vec2_t ulp, int16_vec2_t lrp)
{
	cmdt->cmd_vertices[0] = ulp;
	cmdt->cmd_vertices[2] = lrp;
}


static inline void vdp1_cmdt_vtx_local_coord_set(vdp1_cmdt_t* cmdt, int16_vec2_t p)
{
	cmdt->cmd_vertices[0] = p;
}


static inline void vdp1_cmdt_vtx_system_clip_coord_set(vdp1_cmdt_t* cmdt, int16_vec2_t p)
{
	cmdt->cmd_vertices[2] = p;
}


static inline void vdp1_cmdt_vtx_user_clip_coord_set(vdp1_cmdt_t* cmdt, int16_vec2_t ulp, int16_vec2_t lrp)
{
	cmdt->cmd_vertices[0] = ulp;
	cmdt->cmd_vertices[2] = lrp;
}


static inline void vdp1_cmdt_vtx_set(vdp1_cmdt_t* cmdt, const int16_vec2_t* vertices)
{
	cmdt->cmd_vertices[0] = vertices[0];
	cmdt->cmd_vertices[1] = vertices[1];
	cmdt->cmd_vertices[2] = vertices[2];
	cmdt->cmd_vertices[3] = vertices[3];

}


static inline void vdp1_cmdt_gouraud_base_set(vdp1_cmdt_t* cmdt, vdp1_vram_t base)
{
	cmdt->cmd_grda = (base >> 3) & 0xFFFF;
}


static inline void vdp1_cmdt_normal_sprite_set(vdp1_cmdt_t* cmdt)
{
	vdp1_cmdt_command_set(cmdt, VDP1_CMDT_NORMAL_SPRITE);
}


static inline void vdp1_cmdt_scaled_sprite_set(vdp1_cmdt_t* cmdt)
{
	vdp1_cmdt_command_set(cmdt, VDP1_CMDT_SCALED_SPRITE);
}


static inline void vdp1_cmdt_distorted_sprite_set(vdp1_cmdt_t* cmdt)
{
	vdp1_cmdt_command_set(cmdt, VDP1_CMDT_DISTORTED_SPRITE);
}


static inline void vdp1_cmdt_polygon_set(vdp1_cmdt_t* cmdt)
{
	vdp1_cmdt_command_set(cmdt, VDP1_CMDT_POLYGON);
}


static inline void vdp1_cmdt_polyline_set(vdp1_cmdt_t* cmdt)
{
	vdp1_cmdt_command_set(cmdt, VDP1_CMDT_POLYLINE);
}


static inline void vdp1_cmdt_line_set(vdp1_cmdt_t* cmdt)
{
	vdp1_cmdt_command_set(cmdt, VDP1_CMDT_LINE);
}


static inline void vdp1_cmdt_local_coord_set(vdp1_cmdt_t* cmdt)
{
	vdp1_cmdt_command_set(cmdt, VDP1_CMDT_LOCAL_COORD);
}


static inline void vdp1_cmdt_system_clip_coord_set(vdp1_cmdt_t* cmdt)
{
	vdp1_cmdt_command_set(cmdt, VDP1_CMDT_SYSTEM_CLIP_COORD);
}


static inline void vdp1_cmdt_end_clear(vdp1_cmdt_t* cmdt)
{
	cmdt->cmd_ctrl &= ~0x8000;
}


static inline void vdp1_cmdt_end_set(vdp1_cmdt_t* cmdt)
{
	cmdt->cmd_ctrl |= 0x8000;
}


static inline void vdp1_cmdt_link_type_set(vdp1_cmdt_t* cmdt, vdp1_cmdt_link_type_t link_type)
{
	cmdt->cmd_ctrl &= 0x8FFF;
	cmdt->cmd_ctrl |= link_type;
}


static inline void vdp1_cmdt_link_set(vdp1_cmdt_t* cmdt, vdp1_link_t link)
{
	cmdt->cmd_link = link << 2;
}


#endif
