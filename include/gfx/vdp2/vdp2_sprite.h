#ifndef __FAKEYAUL_VDP2_SPRITE_H__
#define __FAKEYAUL_VDP2_SPRITE_H__


#include <stdint.h>


typedef struct VDP2_SPRITE_0
{
	unsigned int pr:2; //Priority bits
	unsigned int cc:3; //colour calculation ratio
	unsigned int dc:11; //dot colour data
	
} __packed vdp2_sprite_type_0_t;


typedef struct VDP2_SPRITE_1
{
	unsigned int pr:3; //Priority bits
	unsigned int cc:2; //colour calculation ratio
	unsigned int dc:11; //dot colour data
	
} __packed vdp2_sprite_type_1_t;


typedef struct VDP2_SPRITE_2
{
	unsigned int sdw:1; //Shadow (or sprite window?) bit
	unsigned int pr:1; //Priority bits
	unsigned int cc:3; //colour calculation ratio
	unsigned int dc:11; //dot colour data
	
} __packed vdp2_sprite_type_2_t;


typedef struct VDP2_SPRITE_3
{
	unsigned int sdw:1; //Shadow (or sprite window?) bit
	unsigned int pr:2; //Priority bits
	unsigned int cc:2; //colour calculation ratio
	unsigned int dc:11; //dot colour data
	
} __packed vdp2_sprite_type_3_t;


typedef struct VDP2_SPRITE_4
{
	unsigned int sdw:1; //Shadow (or sprite window?) bit
	unsigned int pr:2; //Priority bits
	unsigned int cc:3; //colour calculation ratio
	unsigned int dc:10; //dot colour data
	
} __packed vdp2_sprite_type_4_t;


typedef struct VDP2_SPRITE_5
{
	unsigned int sdw:1; //Shadow (or sprite window?) bit
	unsigned int pr:3; //Priority bits
	unsigned int cc:1; //colour calculation ratio
	unsigned int dc:11; //dot colour data
	
} __packed vdp2_sprite_type_5_t;


typedef struct VDP2_SPRITE_6
{
	unsigned int sdw:1; //Shadow (or sprite window?) bit
	unsigned int pr:3; //Priority bits
	unsigned int cc:2; //colour calculation ratio
	unsigned int dc:10; //dot colour data
	
} __packed vdp2_sprite_type_6_t;


typedef struct VDP2_SPRITE_7
{
	unsigned int sdw:1; //Shadow (or sprite window?) bit
	unsigned int pr:3; //Priority bits
	unsigned int cc:3; //colour calculation ratio
	unsigned int dc:9; //dot colour data
	
} __packed vdp2_sprite_type_7_t;


typedef struct VDP2_SPRITE_8
{
	unsigned int pr:1; //Priority bits
	unsigned int dc:7; //dot colour data
	
} __packed vdp2_sprite_type_8_t;


typedef struct VDP2_SPRITE_9
{
	unsigned int pr:1; //Priority bits
	unsigned int cc:1; //colour calculation ratio
	unsigned int dc:6; //dot colour data
	
} __packed vdp2_sprite_type_9_t;


typedef struct VDP2_SPRITE_A
{
	unsigned int pr:2; //Priority bits
	unsigned int dc:6; //dot colour data
	
} __packed vdp2_sprite_type_a_t;


typedef struct VDP2_SPRITE_B
{
	unsigned int cc:2; //colour calculation ratio
	unsigned int dc:6; //dot colour data
	
} __packed vdp2_sprite_type_b_t;


typedef struct VDP2_SPRITE_C
{
	unsigned int sp:1; //Priority and CRAM address bits?
	unsigned int dc:7; //dot colour data
	
} __packed vdp2_sprite_type_c_t;


typedef struct VDP2_SPRITE_D
{
	unsigned int sp:1; //Priority and CRAM address bits?
	unsigned int sc:1; //colour calculation ratio and CRAM addr
	unsigned int dc:6; //dot colour data
	
} __packed vdp2_sprite_type_d_t;


typedef struct VDP2_SPRITE_E
{
	unsigned int sp:2; //Priority and CRAM address bits?
	unsigned int dc:6; //dot colour data
	
} __packed vdp2_sprite_type_e_t;


typedef struct VDP2_SPRITE_F
{
	unsigned int sc:2; //colour calculation ratio and CRAM addr
	unsigned int dc:6; //dot colour data
	
} __packed vdp2_sprite_type_f_t;


#endif 
