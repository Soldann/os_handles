#ifndef HANDLE_H
#define HANDLE_H

typedef unsigned int HANDLE;

struct handle_node {
    HANDLE next_free_handle;
    struct handle_node * next;
};

int handle_init(); // initialize the handle system

int handle_cleanup(); //deallocate the handle system

int handle_alloc(HANDLE * handle);

int handle_free(HANDLE handle);

int handle_bind(HANDLE handle, void * data); // handles arbitrary data type

int handle_release(HANDLE handle);

#endif
