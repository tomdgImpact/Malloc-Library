#pragma once

#include <err.h>
#include <stddef.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

struct page
{
    struct page *next; // points to the next bucket
    size_t size;
    unsigned long long freed[4];
    char empty;
    void *data;
};


/* global variable that points to the begining of the first page */
extern struct page *begin;
/* compute the smallest power of two greater than or equal to s */
size_t power_two(size_t s);

/*
** called only if there isn't any page mapped or not enough space
** return a pointer to the begining of the new created page
*/
struct page *create(size_t size);


/* 
** if needed, function to retrieve the begining of the page ptr belongs to.
** return the pointer to the begining of the page
*/
void *page_begin(void *ptr, size_t page_size);


/*
** function that check the bits of freed to know which block is free
** return the position of the free block
*/
size_t check_free(unsigned long long freed, size_t *res);

/* set the nth block to free */
unsigned long long set_free(size_t nb_block, unsigned long long freed);
