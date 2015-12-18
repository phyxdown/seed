#ifndef __SEED_CONCURRENT_H
#define __SEED_CONCURRENT_H

#include <stdlib.h>

#define Interface seed_concurrent_queue_methods
typedef struct Interface {
	int  (*enqueue)(struct Interface *q, void *v);
	int  (*dequeue)(struct Interface *q, void **v);
	void (*release)(struct Interface *q);
} Interface;

typedef struct Interface seed_concurrent_queue;
#undef Interface

/**
 * Create a lock-based queue implements enqueue(), dequeue() and release().
 * */
seed_concurrent_queue* seed_concurrent_lock_based_queue_create(size_t limit);

#endif
