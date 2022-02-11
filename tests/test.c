#include <stdio.h>
#include <stdlib.h>

#include "../src/bucket.h"

int main(void)
{
    int *a = malloc(sizeof(int));
    *a = 8;
    printf("%p\n", a);
    size_t size = sysconf(_SC_PAGESIZE);
    printf("%p\n", page_begin(a, size));
    free(a);
    return 0;
}
