#ifndef MY_OS_H
#define MY_OS_H

#include "handle.h"

/**
 * Creates and binds a handle to the "file", storing the filename with the handle in the handle system
 * 
 * @param filename The filename you are trying to open
 * 
 * @return HANDLE pointing to your opened file
 */
HANDLE my_os_open(char * filename);

/**
 * Copies "data" from the file into a buffer. For simplicity, this does not actually open the file but pretends the file
 *  contents are "Contents of file <filename>"
 * 
 * @param handle The handle to the file you want to read
 * @param buffer The buffer into which the data will be stored
 * @param len The size of the buffer
 * 
 * @return 0 on success or 1 on failure
 */
int my_os_read(HANDLE handle, char * buffer, int len);

/**
 * Will free the file handle to be used again
 * 
 * @param handle The handle to the file you want to free
 * 
 */
void my_os_close(HANDLE handle);

#endif
