#ifndef MY_OS_H
#define MY_OS_H

#include "handle.h"

HANDLE my_os_open(char * filename);

int my_os_read(HANDLE handle, char * buffer, int len);

void my_os_close(HANDLE handle);

#endif
