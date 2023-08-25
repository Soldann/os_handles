#ifndef HANDLE_H
#define HANDLE_H

#include <limits.h>

#define HANDLE_SYSTEM_INIT_SIZE 1
#define HANDLE_NULL UINT_MAX

#define DEBUG_HANDLE 0

#if DEBUG_HANDLE
    #include <stdio.h>
    #define DEBUG_PRINT(str) printf(str);
    #define DEBUG_PRINT_ARG(str,arg) printf(str,arg);
#else 
    #define DEBUG_PRINT(str);
    #define DEBUG_PRINT_ARG(str,num);
#endif

typedef unsigned int HANDLE;

struct handle_node {
    HANDLE next_free_handle;
    void * data;
};

/**
 * Initializes the data structures required for the handle tracking system
 *  MUST be run prior to using any other Handle System function
 * 
 * @return 0 for success or 1 for malloc failure
 */
int handle_init();


/**
 * Frees the data structures used for handle tracking
 *  MUST be called prior to ending the program
 * 
 */
void handle_cleanup();

/**
 * Allocates a new handle to be bound and used. May resize the handle data structure if needed
 * 
 * @return A valid handle on success or HANDLE_NULL on failure. If failure, will write the error code to errno
 */
HANDLE handle_alloc();

/**
 * Will call handle_release first, then will free up the handle passed to allow it to be allocated again
 * 
 * @param handle The handle to be freed
 */
void handle_free(HANDLE handle);

/**
 * Binds data to a specific handle
 * 
 * @param handle The handle to which the data will be bound
 * @param data The data structure to bind to this handle. Can be arbitrary type
 */
void handle_bind(HANDLE handle, void * data);

/**
 * Unbinds data from the specified handle. Note this does not free the handle to be reallocated again,
 *  nor does it free the block of memory the handle was originally bound to. Safe to call after free(handle_get(<handle>))
 * 
 * @param handle The handle who will be unbound from its corresponding data
 */
void handle_reset(HANDLE handle);

/**
 * Returns the data to which the handle is currently bound
 * 
 * @param handle The handle to which you are requesting the bound data
 * 
 * @return Pointer to the data bound to that handle. If NULL, the handle is either unbound or invalid
 */
void * handle_get(HANDLE handle);

#endif
