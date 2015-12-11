#ifndef __SEED_CONCURRENT_H
#define __SEED_CONCURRENT_H

typedef struct cqmthz {
	int  (*Enqueue)(struct cqmthz *q, void *v);
	int  (*Dequeue)(struct cqmthz *q, void **v);
	void (*Release)(struct cqmthz *q);
} cqmthz;

typedef struct cqmthz* ConcurrentQueue;

ConcurrentQueue NewSimpleBlockingQueue();
void FreeSimpleBlockingQueue(ConcurrentQueue q);
ConcurrentQueue NewOptimisitcFIFOQueue();
#endif
