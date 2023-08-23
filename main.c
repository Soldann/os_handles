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
    handle_free(1);
    handle_alloc(&h1);
    printf("%i\n", h1);

    handle_cleanup();
}
