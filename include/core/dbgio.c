#include <core/dbgio.h>

#include <core/dbgio-internal.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>



//temporarily handle all the outputs here
const dbgio_dev_ops_t __dbgio_dev_ops_null = {
	.dev = DBGIO_DEV_NULL,
	.default_params = NULL,
	.init = NULL,
	.deinit = NULL,
	.display_set = NULL,
	.puts = NULL,
	.printf = NULL,
	.flush = NULL,
};


const dbgio_dev_ops_t __dbgio_dev_ops_vdp1 = {
	.dev = DBGIO_DEV_NULL,
	.default_params = NULL,
	.init = NULL,
	.deinit = NULL,
	.display_set = NULL,
	.puts = NULL,
	.printf = NULL,
	.flush = NULL,
};


const dbgio_dev_ops_t __dbgio_dev_ops_vdp2 = {
	.dev = DBGIO_DEV_NULL,
	.default_params = NULL,
	.init = NULL,
	.deinit = NULL,
	.display_set = NULL,
	.puts = NULL,
	.printf = NULL,
	.flush = NULL,
};


const dbgio_dev_ops_t __dbgio_dev_ops_vdp2_async = {
	.dev = DBGIO_DEV_NULL,
	.default_params = NULL,
	.init = NULL,
	.deinit = NULL,
	.display_set = NULL,
	.puts = NULL,
	.printf = NULL,
	.flush = NULL,
};


const dbgio_dev_ops_t __dbgio_dev_ops_usb_cart = {
	.dev = DBGIO_DEV_NULL,
	.default_params = NULL,
	.init = NULL,
	.deinit = NULL,
	.display_set = NULL,
	.puts = NULL,
	.printf = NULL,
	.flush = NULL,
};


const dbgio_dev_ops_t __dbgio_dev_ops_mednafen_debug = {
	.dev = DBGIO_DEV_NULL,
	.default_params = NULL,
	.init = NULL,
	.deinit = NULL,
	.display_set = NULL,
	.puts = NULL,
	.printf = NULL,
	.flush = NULL,
};









static struct {
	bool initialised;
	const dbgio_dev_ops_t* dev_ops;
} _dbgio_state;


static const dbgio_dev_ops_t* _dev_ops_table[] = {
	&__dbgio_dev_ops_null,
	&__dbgio_dev_ops_vdp1,
	&__dbgio_dev_ops_vdp2,
	&__dbgio_dev_ops_vdp2_async,
	&__dbgio_dev_ops_usb_cart,
	&__dbgio_dev_ops_mednafen_debug,
};


void dbgio_init(void)
{
	if(_dbgio_state.initialised)
	{
		return;
	}
	
	_dbgio_state.initialised = true;
	
	dbgio_dev_default_init(DBGIO_DEV_NULL);
}


void dbgio_dev_init(dbgio_dev_t dev, const void *params)
{
	assert(_dbgio_state.initialised);
	assert(dev < DBGIO_DEV_COUNT);
	
	if(_dbgio_state.dev_ops != _dev_ops_table[dev])
	{
		dbgio_dev_deinit();
	}
	
	_dbgio_state.dev_ops = _dev_ops_table[dev];
	
	if(_dbgio_state.dev_ops->init != NULL)
	{
		_dbgio_state.dev_ops->init(params);
	}
}


void dbgio_dev_default_init(dbgio_dev_t dev)
{
	assert(dev < DBGIO_DEV_COUNT);
	
	dbgio_dev_init(dev, _dev_ops_table[dev]->default_params);
}


void dbgio_dev_deinit(void)
{
	assert(_dbgio_state.initialised);
	
	if(_dbgio_state.dev_ops == NULL)
	{
		return;
	}
	
	if(_dbgio_state.dev_ops->deinit != NULL)
	{
		_dbgio_state.dev_ops->deinit();
	}
	
	_dbgio_state.dev_ops = NULL;
}


dbgio_dev_t dbgio_dev_selected_get(void)
{
	assert(_dbgio_state.initialised);
	
	if(_dbgio_state.dev_ops == NULL)
	{
		return DBGIO_DEV_NULL;
	}
	
	return _dbgio_state.dev_ops->dev;
}


void dbgio_dev_font_load(void)
{
	assert(_dbgio_state.initialised);
	assert(_dbgio_state.dev_ops != NULL);
	
	if(_dbgio_state.dev_ops->font_load != NULL)
	{
		_dbgio_state.dev_ops->font_load();
	}	
}


void dbgio_display_set(bool display)
{
	assert(_dbgio_state.initialised);
	assert(_dbgio_state.dev_ops != NULL);
	
	if(_dbgio_state.dev_ops->display_set != NULL)
	{
		_dbgio_state.dev_ops->display_set(display);
	}
}


void dbgio_puts(const char* buffer)
{
	assert(_dbgio_state.initialised);
	assert(_dbgio_state.dev_ops != NULL);
	
	if (_dbgio_state.dev_ops->puts == NULL)
	{
		return;
	}
	
	assert(buffer != NULL);
	
	if(*buffer == '\0')
	{
		return;
	}
	
	_dbgio_state.dev_ops->puts(buffer);
}


void dbgio_printf(const char* format, ...)
{
	assert(_dbgio_state.initialised);
	assert(_dbgio_state.dev_ops != NULL);
	
	if(_dbgio_state.dev_ops->printf == NULL)
	{
		return;
	}
	
	va_list args;
	va_start(args, format);
	_dbgio_state.dev_ops->printf(format, args);
	va_end(args);
}


void dbgio_flush(void)
{
	assert(_dbgio_state.initialised);
	assert(_dbgio_state.dev_ops != NULL);
	
	if(_dbgio_state.dev_ops->flush != NULL)
	{
		_dbgio_state.dev_ops->flush();
	}
}


