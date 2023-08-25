#include "handle.h"

#include <stdlib.h>

static union handle_node * handle_system = NULL;
static HANDLE handle_system_size;
static HANDLE handle_first_available;

static int handle_system_resize(HANDLE new_size){
    union handle_node * old_handle_system = handle_system;
    handle_system = malloc(new_size*sizeof(union handle_node)); // allocate space for new size

    // printf("Resizing to %i\n", new_size);

    if (handle_system == NULL) return 1; // if malloc fails, error out

    for (HANDLE i = 0; i < handle_system_size; ++i) { // copy over data from old handle_system
        handle_system[i] = old_handle_system[i];
    }

    for (HANDLE i = handle_system_size; i < new_size; ++i) { // initialize new section of handle_system
        handle_system[i].next_free_handle = i + 1;
    }

    handle_system[new_size - 1].next_free_handle = MAX_HANDLE; // set marker to know when the array needs to be resized

    handle_system_size = new_size;

    free(old_handle_system);

    return 0; // success
}

int handle_init(){
    handle_system_size = 0;
    handle_first_available = 0;

    return handle_system_resize(HANDLE_SYSTEM_INIT_SIZE);    
}

void handle_cleanup(){
    free(handle_system); // this won't fail unless memory gets corrupted
}

int handle_alloc(HANDLE * retvalue){
    if (handle_first_available == MAX_HANDLE){ // if this is the case we are out of space and need to resize the array
        HANDLE new_size = handle_system_size * 2;

        if (new_size < handle_system_size) return 1; // we have overflowed and cannot tolerate more handles

        handle_first_available = handle_system_size; // first available handle is the first handle in the newly added block

        if (handle_system_resize(new_size)) return 1; // if resize fails then malloc error
    }
    *retvalue = handle_first_available;
    handle_first_available = handle_system[handle_first_available].next_free_handle;

    return 0;
}

void handle_free(HANDLE handle){
    // add handle to "front" of the available handles
    handle_system[handle].next_free_handle = handle_first_available;
    handle_first_available = handle;
}

void handle_bind(HANDLE handle, void * data){
    handle_system[handle].data = data;
}

void handle_release(HANDLE handle){
    handle_system[handle].data = NULL;
}

void * handle_get(HANDLE handle){
    return handle_system[handle].data;
}
