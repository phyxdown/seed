#ifndef __SEED_CONCURRENT_H
#define __SEED_CONCURRENT_H

typedef struct cqmthz {
	void  (*Enqueue)(struct cqmthz *queue, void *value);
	void* (*Dequeue)(struct cqmthz *queue);
	void  (*Release)(struct cqmthz *queue);
} cqmthz;

typedef struct cqmthz* ConcurrentQueue;

ConcurrentQueue NewSimpleBlockingQueue();
ConcurrentQueue NewOptimisitcFIFOQueue();
#endif
