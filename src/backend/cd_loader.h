#ifndef __RINGWORLDS_CD_LOADER_H__
#define __RINGWORLDS_CD_LOADER_H__


#include <yaul.h>


void CD_InitFileList();

void CD_LoadFile(const char* filename, void* dest);


#endif
