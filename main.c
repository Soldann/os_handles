#include <stdio.h>

#include "handle.h"

int main(){
    handle_init();

    HANDLE h1;
    handle_alloc(&h1);
    printf("%i\n", h1);
    handle_alloc(&h1);
    printf("%i\n", h1);
    handle_alloc(&h1);
    printf("%i\n", h1);

    handle_init(); // won't do anything because handle_system already initialized

    handle_free(1);
    handle_alloc(&h1);
    printf("%i\n", h1);
    handle_alloc(&h1);
    printf("%i\n", h1);

    int x = 3;
    handle_bind(0, &x);
    *(int *) handle_get(0) = 10;
    printf("X is %i\n", x);

    handle_cleanup();
}
