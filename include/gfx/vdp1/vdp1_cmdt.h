#ifndef __FAKEYAUL_VDP1_CMD_T_H__
#define __FAKEYAUL_VDP1_CMD_T_H__


#include <core/preprocessor.h>
#include <core/colour.h>

#include <stdint.h>
#include <stdbool.h>




//TODO turn the displaylist code into this
typedef uint32_t vdp1_cmdt_list_t;

typedef uint16_t texture_slot_t;
typedef uint16_t gst_slot_t;


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
    VDP1_CMDT_CM_CLUT_16   = 1, //Figure out what's with this one
    VDP1_CMDT_CM_CB_64     = 2,
    VDP1_CMDT_CM_CB_128    = 3,
    VDP1_CMDT_CM_CB_256    = 4,
    VDP1_CMDT_CM_RGB_32768 = 5
} vdp1_cmdt_cm_t;


typedef union vdp1_cmdt_draw_mode
{
	struct
	{
		unsigned int msb_enable :1;
		unsigned int unknown0 :2;
		unsigned int hss_enable :1;
		unsigned int pre_clipping_disable :1;
		unsigned int mesh_enable :1;
		unsigned int end_code_disable :1;
		unsigned int trans_pixel_disable :1;
		unsigned int color_mode :3; //TODO replace with struct
		unsigned int cc_mode :3; //TODO replace with struct
		unsigned int missedOne :2; 
	} __packed;
	uint16_t raw;
	
} __packed vdp1_cmdt_draw_mode_t;


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


typedef union _flags
{
	unsigned int unknown0 :11;
	sort_type_t sort_type :3;
	plane_type_t plane_type :1;
	unsigned int use_texture :1;
}__packed __aligned(2) flags_t;


typedef struct _attribute
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
typedef struct _polygon
{
	flags_t flags;
	indices_t indices;
} polygon_t;


typedef struct COMMAND
{
} vdp1_cmdt_t;





#endif
