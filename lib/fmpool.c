#include "fmpool.h"

#define fmpoolFobj(P) \
	((fobj*)(((char*)(P)) - sizeof(fobj)))

fmpool* 
fmpoolCreate(size_t cap, size_t size) {
	fmpool* p;
	if ((p = calloc(1, sizeof(fmpool)+(sizeof(fobj)+size)*cap)) == NULL) return NULL;
	p->size = sizeof(fobj) + size;
	p->cap = cap * p->size;
	return p;
}

void*
fmpoolAlloc(fmpool* p) {
	fobj* o;
	int   b = 1;
	while (b) {
		p->last += p->size;
		p->last %= p->cap;
		o = (fobj*)&p->memory[p->last];
		b = o->busy;
	}
	o->busy = 1;
	return (void*)&o->memory[0];
}

void
fmpoolFree(void* obj) {
	fmpoolFobj(obj)->busy = 0;
}
