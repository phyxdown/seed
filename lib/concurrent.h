#ifndef __SEED_CONCURRENT_H
#define __SEED_CONCURRENT_H

#include <stdlib.h>

#define interface seed_concurrent_queue_methods
typedef struct interface {
	int  (*enqueue)(struct interface *q, void *v);
	int  (*dequeue)(struct interface *q, void **v);
	void (*release)(struct interface *q);
} interface;

typedef struct interface seed_concurrent_queue;
#undef interface

/**
 * Create a lock-based queue implements enqueue(), dequeue() and release().
 * */
seed_concurrent_queue* seed_concurrent_lock_based_queue_create(size_t limit);

#endif
