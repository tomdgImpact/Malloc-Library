#include "my_malloc.h"

size_t align(size_t size)
{
    size_t to_align = sizeof(long double);

    while ((size % to_align) != 0)
        ++size;
    return size;
}

void set_data(struct page *p, size_t size)
{
    size_t check = 0;
    size_t i = 0;
    size_t res = 0;
    for (; i < 4; i++)
    {
        check = check_free(p->freed[i], &res);
        if (check != 0)
            break;
    }
    if (i == 4)
    {
        struct page *tmp = create(size);
        tmp->data = (char *)tmp + sizeof(struct page) + 1 + (size * res);
    }
    else
        p->data = (char *)p + sizeof(struct page) + 1 + (size * res);
    p->freed[i] = make_used(check, p->freed[i]);
}

struct page *correct_page(size_t size)
{
    struct page *p = begin;
    size = align(size);
    for (; p->next != NULL; p = p->next)
    {
        if (p->empty == 1)
            return p;
        else if (p->size <= size)
            return p;
    }
    if (p->size <= size)
        return p;
    p = create(size);
    return p;
}

int is_empty(struct page *p)
{
    size_t res = 0;
    for (size_t i = 0; i < 4; i++)
    {
        size_t c = check_free(p->freed[i], &res);
        if (c == 1)
            return 0;
    }
    return 1;
}

unsigned long long make_used(size_t nb_block, unsigned long long freed)
{
    unsigned long long mask = ~(1 << (nb_block - 1));
    return freed & mask;
}
