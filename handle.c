#include "handle.h"

#include <limits.h>
#include <stdlib.h>

static struct handle_node * handle_system;

int handle_init(){
    handle_system = malloc(sizeof(struct handle_node));
    handle_system->next_free_handle = 0;
    handle_system->next = NULL;
}

int handle_cleanup(){
    while (handle_system != NULL) {
        struct handle_node * curNode = handle_system;
        handle_system = handle_system->next;
        free(curNode);
    } 
}

int handle_alloc(HANDLE * handle){
    if (handle_system->next == NULL && handle_system->next_free_handle < UINT_MAX) {
        *handle = (handle_system->next_free_handle)++;
    } else {
        *handle = handle_system->next_free_handle;
        struct handle_node * temp = handle_system;
        handle_system = handle_system->next;
        free(temp);
    }

}

int handle_free(HANDLE handle){
    // add handle to beginning of handle_system
    struct handle_node * new_free_handle = malloc(sizeof(struct handle_node));
    new_free_handle->next_free_handle = handle;
    new_free_handle->next = handle_system;
    handle_system = new_free_handle;
}

int handle_bind(HANDLE handle, void * data){

}

int handle_release(HANDLE handle){

}
