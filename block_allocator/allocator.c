#include "allocator.h"

#include <err.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

struct blk_allocator *blka_new(void)
{
    struct blk_allocator *new = calloc(1, sizeof(struct blk_allocator));
    if (new == NULL)
        return NULL;
    new->meta = NULL;
    return new;
}

void blka_delete(struct blk_allocator *blka)
{
    struct blk_meta *met = blka->meta;

    while (met != NULL)
    {
        struct blk_meta *tmp = met;
        met = met->next;
        tmp->next = NULL;
        blka_free(tmp);
    }

    free(blka);
}

struct blk_meta *blka_alloc(struct blk_allocator *blka, size_t size)
{
    long conf = sysconf(_SC_PAGESIZE);
    long test_size = size;
    const long keep_init_conf = conf;
    while (test_size > conf)
        conf += keep_init_conf;
    struct blk_meta *new = NULL;
    new = mmap(NULL, conf, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
               -1, 0);
    if (new == MAP_FAILED)
        return NULL;
    size_t remaining = conf - sizeof(struct blk_meta);
    new->size = remaining;
    new->next = blka->meta;
    blka->meta = new;

    return new;
}

void blka_free(struct blk_meta *blk)
{
    munmap(blk, blk->size);
}

void blka_pop(struct blk_allocator *blka)
{
    if (blka == NULL)
        return;
    if (blka->meta == NULL)
        return;
    struct blk_meta *m = blka->meta;
    if (m->next == NULL)
    {
        blka_free(m);
        blka->meta = NULL;
    }
    else
    {
        struct blk_meta *poped = m;
        m = m->next;
        blka->meta = m;
        blka_free(poped);
    }
}
