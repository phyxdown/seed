#ifndef __SEED_CONCURRENT_H
#define __SEED_CONCURRENT_H

#include <stdlib.h>

/**
 * Example 
 * seed_concurrent_queue* q;
 * q = seed_concurrent_queue_create(1000);
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

typedef struct seed_concurrent_queue seed_concurrent_queue;

struct seed_concurrent_queue {
	int  (*enqueue)      (seed_concurrent_queue* q, void *v);
	int  (*dequeue)      (seed_concurrent_queue* q, void **v);
	int  (*batchDequeue) (seed_concurrent_queue* q, void **v, size_t l);
	void (*release)      (seed_concurrent_queue* q);
};

seed_concurrent_queue* seed_concurrent_queue_create(size_t limit);

#endif
