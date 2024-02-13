#ifndef __FAKEYAUL_DBGIO_INTERNAL_H__
#define __FAKEYAUL_DBGIO_INTERNAL_H__


#include <core/dbgio.h>

#include <stdbool.h>
#include <stdarg.h>


typedef void (*font_load_callback)(void);
typedef void (*dev_ops_init_t)(const void* params);
typedef void (*dev_ops_deinit_t)(void);
typedef void (*dev_ops_display_set_t)(bool display);
typedef void (*dev_ops_font_load_t)(void);
typedef void (*dev_ops_puts_t)(const char* buffer);
typedef void (*dev_ops_printf_t)(const char* format, va_list ap);
typedef void (*dev_ops_flush_t)(void);


typedef struct dbgio_dev_ops
{
	dbgio_dev_t dev;
	const void* default_params;
	dev_ops_init_t init;
	dev_ops_deinit_t deinit;
	dev_ops_display_set_t display_set;
	dev_ops_font_load_t font_load;
	dev_ops_puts_t puts;
	dev_ops_printf_t printf;
	dev_ops_flush_t flush;
	
} dbgio_dev_ops_t;


extern const dbgio_dev_ops_t __dbgio_dev_ops_null;
extern const dbgio_dev_ops_t __dbgio_dev_ops_vdp1;
extern const dbgio_dev_ops_t __dbgio_dev_ops_vdp2;
extern const dbgio_dev_ops_t __dbgio_dev_ops_vdp2_async;
extern const dbgio_dev_ops_t __dbgio_dev_ops_usb_cart;
extern const dbgio_dev_ops_t __dbgio_dev_ops_mednafen_debug;

extern const dbgio_font_t __dbgio_default_font;

extern void __dbgio_font_1bpp_4bpp_decompress(const dbgio_font_t* font, uint8_t* dec_cg);


#endif
