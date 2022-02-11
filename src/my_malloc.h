#pragma once

#include "bucket.h"

/* function in charge of resize the size to be align with long double */
size_t align(size_t size);

/* Set the data pointer to the right location in memory */
void set_data(struct page *p, size_t size);

/* Search for a page of size size in the page struct linked list */
struct page *correct_page(size_t size);

/* check if a page is empty, set page->empty to 1 */
int is_empty(struct page *p);

unsigned long long make_used(size_t nb_block, unsigned long long freed);

