#ifndef HANDLE_H
#define HANDLE_H

#include <limits.h>

#define HANDLE_SYSTEM_INIT_SIZE 1
#define HANDLE_NULL UINT_MAX

typedef unsigned int HANDLE;

union handle_node {
    HANDLE next_free_handle;
    void * data;
};

int handle_init(); // initialize the handle system

void handle_cleanup(); //deallocate the handle system

HANDLE handle_alloc();

void handle_free(HANDLE handle);

void handle_bind(HANDLE handle, void * data); // handles arbitrary data type

void handle_release(HANDLE handle);

void * handle_get(HANDLE handle);

#endif
