#ifndef __SEED_FMPOOL_H
#define __SEED_FMPOOL_H

#include <stdio.h>
#include <stdlib.h>

typedef struct seed_fmpool_obj {
	int busy;
	char memory[];
} seed_fmpool_obj;

typedef struct seed_fmpool {
	size_t cap;
	size_t size;
	char memory[];
} seed_fmpool;

seed_fmpool* seed_fmpool_create(size_t cap, size_t size);
void seed_fmpool_release(seed_fmpool* pool);

void* seed_fmpool_alloc(seed_fmpool* pool);
void  seed_fmpool_free(void* obj);

#endif
