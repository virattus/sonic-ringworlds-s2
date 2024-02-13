#ifndef __FAKEYAUL_VDP2_VRAM_H__
#define __FAKEYAUL_VDP2_VRAM_H__


#include <core/preprocessor.h>

#include <stdint.h>


#define VDP2_VRAM_ADDR(bank, offset) 		(((bank) << 17) + (offset))

#define VDP2_VRAM_DIM 						512
#define VDP2_VRAM_BYTE_SIZE					0x0007FFFF

#define VDP2_VRAM_FRAMEBUFFER_SIZE			1048576


typedef uint32_t vdp2_vram_t;



typedef enum vdp2_vram_bank
{
	VDP2_VRAM_BANK_A0,
	VDP2_VRAM_BANK_A1,
	VDP2_VRAM_BANK_B0,
	VDP2_VRAM_BANK_B1,
	
} vdp2_vram_bank_t;


typedef enum vdp2_vram_mode
{
	VDP2_VRAM_CTL_MODE_NO_PART_BANK_A = 0x00,
	VDP2_VRAM_CTL_MODE_NO_PART_BANK_B = 0x00,
	VDP2_VRAM_CTL_MODE_PART_BANK_A = 0x01,
	VDP2_VRAM_CTL_MODE_PART_BANK_B = 0x02,
	VDP2_VRAM_CTL_MODE_PART_BANK_BOTH = 0x03,
	
} vdp2_vram_ctl_mode_t;


//sets whether to store the table in VRAM or colour ram
//See if it's faster to do it in cram or something
typedef enum vdp2_vram_ctl_coeff_table
{
	VDP2_VRAM_CTL_COEFF_TABLE_VRAM,
	VDP2_VRAM_CTL_COEFF_TABLE_CRAM,
	
} vdp2_vram_ctl_coeff_table_t;


typedef enum vdp2_vram_usage_type
{
	VDP2_VRAM_USAGE_TYPE_NONE,
	VDP2_VRAM_USAGE_TYPE_COEFF_TBL,
	VDP2_VRAM_USAGE_TYPE_PND,
	VDP2_VRAM_USAGE_TYPE_CPD_BPD,
	
} __packed vdp2_vram_usage_type_t;


typedef struct vdp2_vram_usage
{
	vdp2_vram_usage_type_t a0;
	vdp2_vram_usage_type_t a1;
	vdp2_vram_usage_type_t b0;
	vdp2_vram_usage_type_t b1;

} __packed vdp2_vram_usage_t;


typedef struct vdp2_vram_ctl
{
	vdp2_vram_ctl_coeff_table_t coeff_table;
	
	vdp2_vram_ctl_mode_t vram_mode;
	
} vdp2_vram_ctl_t;


//These defines appear to be for handling VDP2 memory access in some way

//These are for the pattern info for the background layers
#define VDP2_VRAM_CYCP_PNDR_NBG0	0x0
#define VDP2_VRAM_CYCP_PNDR_NBG1	0x1
#define VDP2_VRAM_CYCP_PNDR_NBG2	0x2
#define VDP2_VRAM_CYCP_PNDR_NBG3	0x3

//These are for the character data
#define VDP2_VRAM_CYCP_CHPNDR_NBG0	0x4
#define VDP2_VRAM_CYCP_CHPNDR_NBG1	0x5
#define VDP2_VRAM_CYCP_CHPNDR_NBG2	0x6
#define VDP2_VRAM_CYCP_CHPNDR_NBG3	0x7

//These are for the vertical cell scroll tables
#define VDP2_VRAM_CYCP_VCSTDR_NBG0	0xC
#define VDP2_VRAM_CYCP_VCSTDR_NBG1	0xD

#define VDP2_VRAM_CYCP_CPU_RW		0xE //CPU read/write
#define VDP2_VRAM_CYCP_NO_ACCESS	0xF //NO ACCESS


#define VDP2_VRAM_CYCP_PNDR(n)		((n) & 0x03) //Pattern name data read
#define VDP2_VRAM_CYCP_CHPNDR(n)	(((n) & 0x03) + 0x04) //character pattern name read
#define VDP2_VRAM_CYCP_VCSTDR(n)	(((n) & 0x01) + 0x0C) // vertical cell scroll table data read


typedef struct vdp2_vram_cycp_bank
{
	union
	{
		uint32_t raw;
		
		struct
		{
			unsigned int t0:4;
			unsigned int t1:4;
			unsigned int t2:4;
			unsigned int t3:4;
			unsigned int t4:4;
			unsigned int t5:4;
			unsigned int t6:4;
			unsigned int t7:4;
			
		} __packed;
	};
	
} __packed vdp2_vram_cycp_bank_t;


typedef struct vdp2_vram_cycp
{
	vdp2_vram_cycp_bank_t pt[4];
	
} __packed vdp2_vram_cycp_t;


void vdp2_vram_control_set(const vdp2_vram_ctl_t* vram_ctl);
void vdp2_vram_usage_set(const vdp2_vram_usage_t* vram_usage);

vdp2_vram_cycp_t* vdp2_vram_cycp_get(void);
void vdp2_vram_cycp_set(const vdp2_vram_cycp_t* vram_cycp);
void vdp2_vram_cycp_clear(void);

vdp2_vram_cycp_bank_t* vdp2_vram_cycp_bank_get(vdp2_vram_bank_t bank);
void vdp2_vram_cycp_bank_set(vdp2_vram_bank_t bank, const vdp2_vram_cycp_bank_t* cycp_bank);
void vdp2_vram_cycp_bank_clear(vdp2_vram_bank_t bank);




#endif
