#ifndef __FAKEYAUL_VDP1_VRAM_H__
#define __FAKEYAUL_VDP1_VRAM_H__


#include <core/colour.h>
#include <gfx/vdp2/vdp2_sprite.h>

#include <stdint.h>


#define VDP1_VRAM_DEFAULT_CMDT_COUNT 	(2048)
#define VDP1_VRAM_DEFAULT_TEXTURE_SIZE	(442336) //bytes
#define VDP1_VRAM_DEFAULT_GOURAUD_COUNT (1024)
#define	VDP1_VRAM_DEFAULT_CLUT_COUNT 	(256)


#define VDP1_VRAM_SIZE			524288 //bytes

#define VDP1_CMD_TABLE(x, y)	VDP1_VRAM(((x) << 5) + (((y) << 1) & 0x1F))


typedef uint32_t vdp1_vram_t;


typedef struct VDP1_CMDT vdp1_cmdt_t;


typedef struct VDP1_GOURAUD_TABLE
{
	rgb1555_t colors[4];
	
} __aligned(8) vdp1_gouraud_table_t;


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


typedef union VDP1_COLOUR_BANK
{
	vdp1_color_bank_type_0_t type_0;
	vdp1_color_bank_type_1_t type_1;
	vdp1_color_bank_type_2_t type_2;
	vdp1_color_bank_type_3_t type_3;
	vdp1_color_bank_type_4_t type_4;
	vdp1_color_bank_type_5_t type_5;
	vdp1_color_bank_type_6_t type_6;
	vdp1_color_bank_type_7_t type_7;
	vdp1_color_bank_type_8_t type_8;
	vdp1_color_bank_type_9_t type_9;
	vdp1_color_bank_type_a_t type_a;
	vdp1_color_bank_type_b_t type_b;
	vdp1_color_bank_type_c_t type_c;
	vdp1_color_bank_type_d_t type_d;
	vdp1_color_bank_type_e_t type_e;
	vdp1_color_bank_type_f_t type_f;
	
} vdp1_color_bank_t;


typedef union VDP1_CLUT
{
	vdp1_color_bank_t entries[16];
	rgb1555_t colors[16];
	
} __aligned(32) vdp1_clut_t;


typedef struct VDP1_VRAM_PARTITIONS
{
	vdp1_cmdt_t* cmdt_base;
	uint32_t cmdt_size;
	
	void* texture_base;
	uint32_t texture_size;
	
	vdp1_gouraud_table_t* gouraud_base;
	uint32_t gouraud_size;
	
	vdp1_clut_t* clut_base;
	uint32_t clut_size;
	
	void* remaining_base;
	uint32_t remaining_size;
	
} vdp1_vram_partitions_t;


void vdp1_vram_partitions_get(vdp1_vram_partitions_t* partition);
void vdp1_vram_partitions_set(uint32_t cmdt_count, uint32_t texture_size, uint32_t gouraud_count, uint32_t clut_count);


void* VDP1_VRAM(uint32_t offset);


#endif
