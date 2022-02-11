#include "bucket.h"

#include <stdint.h>
#include <unistd.h>

struct page *begin = NULL;

size_t power_two(size_t s)
{
    size_t pow = 1;
    pow <<= 4;
    while (s > pow)
        pow <<= 1;
    return pow;
}

static void set_all_free(struct page *p)
{
    for (size_t i = 0; i < 4; i++)
    {
        unsigned long long fr = p->freed[i];
        for (size_t j = 0; j < 64; j++)
            fr = set_free(j, fr);
        p->freed[i] = fr;
    }
}

struct page *create(size_t size)
{
    size_t conf = sysconf(_SC_PAGESIZE);
    size_t keep = conf;
    while (size >= conf)
        conf += keep;
    struct page *p = mmap(NULL, conf, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (p == MAP_FAILED)
        return NULL;
    if (begin == NULL)
    {
        begin = p;
        p->next = NULL;
    }
    else
    {
        p->next = begin;
        begin = p;
    }
    /* set the page as empty because we just created it */
    p->empty = 1;
    p->size = size;
    set_all_free(p);
    return p;
}

size_t check_free(unsigned long long freed, size_t *res)
{
    /* we will go through each bit of the freed variable */
    size_t mask = 1;
    for (size_t i = 0; i < 64; i++)
    {
        mask <<= 1;
        unsigned short test = freed & mask;
        if (test != 0)
        {
            *res = i;
            return 1;
        }
    }
    return 0;
}

unsigned long long set_free(size_t nb_block, unsigned long long freed)
{
    size_t mask = 1 << nb_block;
    freed |= mask;
    return freed;
}
