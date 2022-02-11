#include "bucket.h"
#include "my_malloc.h"

__attribute__((visibility("default"))) void *malloc(size_t size)
{
    if (size == 0)
        return NULL;
    size = align(size);
    struct page *p = NULL;
    if (begin == NULL)
    {
        p = create(size);
        if (p == NULL)
            return NULL;
        begin = p;
    }
    else
        p = correct_page(size);
    if (p == NULL)
        return NULL;
    set_data(p, size);
    p->empty = 0;
    return p->data;
}

__attribute__((visibility("default"))) void free(void *ptr)
{
    if (ptr == NULL)
        return;
    size_t conf = sysconf(_SC_PAGESIZE);
    size_t i = 1;
    struct page *check = begin;
    struct page *p = ptr;
    while (check <= p)
    {
        check += conf;
        i++;
    }
    check -= conf;
    i -= 1;
    size_t val = 64;
    size_t index = 0;
    while (val < i)
    {
        val += 64;
        index++;
    }
    /* the corresponding bit to free and check for emptyness */
    check->freed[index] = set_free(index, check->freed[index]);
    if (is_empty(check) == 1)
    {
        int test = munmap(check, conf);
        if (test == -1)
            errx(1, "cannot free ptr");
    }
}

__attribute__((visibility("default"))) void *realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return malloc(size);
    else if (size == 0)
        return NULL;
    if (ptr != NULL && size == 0)
    {
        free(ptr);
        return NULL;
    }
    /* case where size is different */
    size = align(size);
    size_t conf = sysconf(_SC_PAGESIZE);
    size_t i = 1;
    struct page *check = begin;
    struct page *p = ptr;
    while (check <= p)
    {
        check += conf;
        i++;
    }
    check -= conf;
    i -= 1;
    size_t val = 64;
    size_t index = 0;
    while (val < i)
    {
        val += 64;
        index++;
    }
    // after finding the beginning of the memory area we want to realloc 
    // we need to know if size is equal, less or more than previous one
    return NULL;
}

__attribute__((visibility("default"))) void *calloc(size_t nmemb, size_t size)
{
    if (size == 0 || nmemb == 0)
        return NULL;
    size_t check_mul = 0;
    if (__builtin_mul_overflow(nmemb, size, &check_mul) != 0)
        return NULL;
    size = check_mul;
    size = align(size);
    struct page *p = NULL;
    if (begin == NULL)
        p = create(size);
    else
        p = correct_page(size);
    if (p == NULL)
        return NULL;
    set_data(p, size);
    memset(p->data, 0, size);
    return p->data;
}
