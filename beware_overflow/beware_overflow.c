#include "beware_overflow.h"

#include <err.h>
#include <stddef.h>

void *beware_overflow(void *ptr, size_t nmemb, size_t size)
{
    if (size == 0)
        return ptr;

    size_t check_mul = 0;
    if (__builtin_mul_overflow(nmemb, size, &check_mul) != 0)
        return NULL;
    char *tmp = ptr;
    tmp += check_mul;
    return tmp;
}
