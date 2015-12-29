#ifndef __SEED_CONCURRENT_H
#define __SEED_CONCURRENT_H

#include <stdlib.h>

/**
 * Example 
 * seed_concurrent_queue* q;
 * q = seed_concurrent_lock_based_queue_create(1000);
 *
 * char* a = (char*)malloc(4);
 * int r = q->enqueue(q, a);
 * if (r < 0);
 * if (r == 0) free(a);
 * if (r == 1);
 *
 * char* b;
 * int = q->dequeue(q, &b); == 0;
 * if (r < 0);
 * if (r == 0);
 * if (r == 1) {
 * 	do_something_with(b);
 * 	free(b);
 * }
 * 
 * char* c[10];
 * int r = q->batch_dequeue(q, c, 10);
 * if (r < 0);
 * if (r == 0);
 * if (r > 0) {
 * 	int i;
 * 	for (i = 0; i < r; i++) {
 * 		do_something_with(c[i]);
 * 		free(c[i]);
 * 	}
 * }
 *
 * q->release(q);
 *
 * */

typedef struct seed_concurrent_queue_methods seed_concurrent_queue;

struct seed_concurrent_queue_methods {
	int  (*enqueue)       (struct seed_concurrent_queue_methods *q, void *v);
	int  (*dequeue)       (struct seed_concurrent_queue_methods *q, void **v);
	int  (*batch_dequeue) (struct seed_concurrent_queue_methods *q, void **v, int l);
	void (*release)       (struct seed_concurrent_queue_methods *q);
};

/**
 * Create a lock-based queue implements enqueue(), dequeue() and release().
 * */
seed_concurrent_queue* seed_concurrent_lock_based_queue_create(size_t limit);

#endif
