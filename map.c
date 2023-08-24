#ifndef MAP_H
#define MAP_H

#include "handle.h"

struct handle_map {
    unsigned int size;
    void * data;
};

int handle_map_insert(HANDLE handle, void * data);

void * handle_map_at(HANDLE handle);

#endif