#ifndef __SEED_CONCURRENT_H
#define __SEED_CONCURRENT_H

#include <stdlib.h>

/**
 * Example 
 * seed_concurrent_queue* q;
 * q = seed_concurrent_lock_based_queue_create(1000);
 *
 * char* a = (char*)malloc(4);
 * if (q->enqueue(q, a) < 0) {
 * 	free(a);
 * }
 *
 * char* b;
 * if (q->dequeue(q, &b) < 0) {
 * 	if (b != NULL) {
 * 		do_something_with(b);
 * 		free(b);
 * 	}
 * }
 *
 * q->release(q);
 *
 * */

typedef struct seed_concurrent_queue_methods seed_concurrent_queue;

struct seed_concurrent_queue_methods {
	int  (*enqueue)(struct seed_concurrent_queue_methods *q, void *v);
	int  (*dequeue)(struct seed_concurrent_queue_methods *q, void **v);
	void (*release)(struct seed_concurrent_queue_methods *q);
};

/**
 * Create a lock-based queue implements enqueue(), dequeue() and release().
 * */
seed_concurrent_queue* seed_concurrent_lock_based_queue_create(size_t limit);

#endif
