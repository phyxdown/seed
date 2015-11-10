#ifndef __SEED_FMPOOL_H
#define __SEED_FMPOOL_H

/**
 * EXAMPLE
 *
 *	typedef struct {
 *		size_t age;
 *		size_t height;
 *	} wife;
 *	
 *	int main() {
 *		fmpool* p = fmpoolCreate(64, sizeof(wife));
 *		wife* w = (wife*)fmpoolAlloc(p);
 *		w->age = 23;
 *		w->height = 169;
 *		printf("My wife is %d year's old.", w->age);
 *		fmpoolFree(w);
 *	}
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int    busy;
	char   memory[];
} fobj;

typedef struct {
	size_t cap;
	size_t size;
	size_t last;
	char   memory[];
} fmpool;

fmpool* fmpoolCreate(size_t cap, size_t size);
void*   fmpoolAlloc(fmpool* pool);
void    fmpoolFree(void* obj);

#endif
