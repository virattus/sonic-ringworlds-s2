#ifndef __FAKEYAUL_PREPROCESSOR_H__
#define __FAKEYAUL_PREPROCESSOR_H__



#include <stdint.h>


#define __packed			__attribute__ ((__packed__))
#define __aligned(x)		__attribute__ ((__aligned__ (x)))
#define __unused			__attribute__ ((__unused__))


#define static_assert(e) _Static_assert(e, "Failed on " __FILE__ ": L" __XSTRING(__LINE__))

#define min(a,b) \
	({ __typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a < _b ? _a : _b; })

#define max(a,b) \
	({ __typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _a : _b; })


#define clamp(x, y, z)                                                         \
    __extension__ ({ __typeof__ (x) _x = (x);                                  \
       __typeof__ (y) _y = (y);                                                \
       __typeof__ (z) _z = (z);                                                \
       (_x <= _y) ? _y : ((_x >= _z) ? _z : _x);                               \
    })


#define SDL_A_MASK 0x000000FF
#define SDL_R_MASK 0xFF000000
#define SDL_G_MASK 0x00FF0000
#define SDL_B_MASK 0x0000FF00


#endif
