#ifndef __SEED_FMPOOL_H
#define __SEED_FMPOOL_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int spare;
	char memory[];
} fobj;

typedef struct {
	size_t cap;
	size_t size;
	char memory[];
} fmpool;

fmpool* fmpoolCreate(size_t cap, size_t size);
void*   fmpoolGet(fmpool* pool);
void    fmpoolFree(void* obj);

#endif
