#ifndef __FAKEYAUL_RAM_H__
#define __FAKEYAUL_RAM_H__



#include <stdint.h>


#define WRAM_SIZE 			(1 << 20)


extern uint8_t LOW_WRAM[WRAM_SIZE];
extern uint8_t HIGH_WRAM[WRAM_SIZE];


#define LWRAM(x)				(LOW_WRAM + (x))
#define LWRAM_UNCACHED(x)		(LOW_WRAM + (x))
#define LWRAM_PURGE_CACHE(x)	(LOW_WRAM + (x))
#define LWRAM_SIZE				(WRAM_SIZE)

#define HWRAM(x)				(HIGH_WRAM + (x))
#define HWRAM_UNCACHED(x)		(HIGH_WRAM + (x))
#define HWRAM_PURGE_CACHE(x)	(HIGH_WRAM + (x))
#define HWRAM_SIZE				(WRAM_SIZE)



#endif
