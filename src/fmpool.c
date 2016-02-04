#include "fmpool.h"

#define itos(P) \
	((seed_fmpool_obj*)(((char*)(P)) - sizeof(seed_fmpool_obj)))

#define CAS(ptr, old, new) \
	 (__sync_bool_compare_and_swap((ptr), (old), (new)))

seed_fmpool* 
seed_fmpool_create(size_t cap, size_t size) {
	seed_fmpool* p;
	if ((p = calloc(1, sizeof(seed_fmpool)+(sizeof(seed_fmpool_obj)+size)*cap)) == NULL) return NULL;
	p->size = sizeof(seed_fmpool_obj) + size;
	p->cap = cap * p->size;
	return p;
}

void seed_fmpool_release(seed_fmpool* pool) {
	if (NULL != pool)
		free(pool);
}

void*
seed_fmpool_alloc(seed_fmpool* p) {
	int size = p->size;
	int cap = p->cap;
	int count = cap/size;
	int i, b, last;
	seed_fmpool_obj* o;
	last = 0;
	for (i = 0; i < count; i++) {
		o = (seed_fmpool_obj*)&p->memory[last];
		b = o->busy;
		if (!b && CAS(&o->busy, 0, 1)) {
			return (void*)&o->memory[0];
		}
		last += size;
		last %= cap;
	}
	return NULL;
}

void
seed_fmpool_free(void* obj) {
	CAS(&(itos(obj)->busy), 1, 0);
}

#undef itos
#undef CAS
