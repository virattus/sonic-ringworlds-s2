#ifndef __FAKEYAUL_VDP2_CRAM_H__
#define __FAKEYAUL_VDP2_CRAM_H__



#define VDP2_CRAM_ADDR(address) (0x25F00000UL + ((address) << 1))

#define VDP2_CRAM_MODE_0_OFFSET(_256_bank, _16_bank, color_index)              \
    (0x25F00000UL + ((_256_bank) << 9) +                                       \
                    ((_16_bank) << 5) +                                        \
                    ((color_index) << 1))
#define VDP2_CRAM_MODE_1_OFFSET(_256_bank, _16_bank, color_index)              \
    VDP2_CRAM_MODE_0_OFFSET(_256_bank, _16_bank, color_index)
#define VDP2_CRAM_MODE_2_OFFSET(_256_bank, _16_bank, color_index)              \
     (0x25F00000UL + ((_256_bank) << 10) +                                     \
                     ((_16_bank) << 6) +                                       \
                     ((color_index) << 2))


#endif
