#include "my_os.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

HANDLE my_os_open(const char * filename){
    HANDLE new_handle = handle_alloc();

    if (new_handle != HANDLE_NULL){
        handle_bind(new_handle, (const void *) filename);
    }
    return new_handle;
}

int my_os_read(HANDLE handle, char * buffer, int len){
    // construct some fake data to read
    char * data1 = handle_get(handle);
    char data2[18] = "Contents of file ";
    
    strncpy(buffer, data2, len);
    strncat(buffer, data1, len - strlen(buffer));

    return 0;
}

void my_os_close(HANDLE handle) {
    handle_free(handle);
}