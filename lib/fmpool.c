#include "fmpool.h"

#define fmpoolFobj(P) \
	((fobj*)(((char*)(P)) - sizeof(fobj)))

#define CAS(ptr, old, new) \
	 (__sync_bool_compare_and_swap((ptr), (old), (new)))

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
	int size = p->size;
	int cap = p->cap;
	int count = cap/size;
	int i, b, last;
	fobj* o;
	for (i = 0; i < count; i++) {
		last += size;
		last %= cap;
		o = (fobj*)&p->memory[last];
		b = o->busy;
		if (!b && CAS(&fmpoolFobj(o)->busy, 0, 1)) {
			return (void*)&o->memory[0];
		}
	}
	return NULL;
}

void
fmpoolFree(void* obj) {
	CAS(&fmpoolFobj(obj)->busy, 1, 0);
}
