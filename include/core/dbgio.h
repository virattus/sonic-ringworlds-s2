#ifndef __FAKEYAUL_DBGIO_H__
#define __FAKEYAUL_DBGIO_H__



typedef enum dbgio_dev 
{
    DBGIO_DEV_NULL,
    DBGIO_DEV_VDP1,
    DBGIO_DEV_VDP2,
    DBGIO_DEV_VDP2_ASYNC,
    DBGIO_DEV_USB_CART,
    DBGIO_DEV_MEDNAFEN_DEBUG
    
} dbgio_dev_t;



void dbgio_init(void);

void dbgio_dev_init(dbgio_dev_t dev, const void *params);
void dbgio_dev_default_init(dbgio_dev_t dev);

void dbgio_dev_font_load(void);

void dbgio_printf(const char* format, ...);

void dbgio_flush(void);

#endif
