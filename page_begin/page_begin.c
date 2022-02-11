#include "page_begin.h"

void *page_begin(void *ptr, size_t page_size)
{
    char *p = 0;
    char *to_check = ptr;
    while (p <= to_check)
        p += page_size;
    return p - page_size;
}
