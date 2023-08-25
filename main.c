#include <stdio.h>

#include "handle.h"
#include "my_os.h"

int main(){
    handle_init();

    HANDLE h1;
    h1 = handle_alloc();
    printf("%i\n", h1);
    h1 = handle_alloc();
    printf("%i\n", h1);
    h1 = handle_alloc();
    printf("%i\n", h1);

    handle_init(); // won't do anything because handle_system already initialized

    handle_free(1);
    h1 = handle_alloc();
    printf("%i\n", h1);
    h1 = handle_alloc();
    printf("%i\n", h1);

    int x = 3;
    handle_bind(0, &x);
    *(int *) handle_get(0) = 10;
    printf("X is %i\n", x);

    HANDLE f1 = my_os_open("test.txt\n");
    char buffer[100];
    my_os_read(f1, buffer, 100);
    printf("%s", buffer);
    my_os_close(f1);

    handle_cleanup();
}
