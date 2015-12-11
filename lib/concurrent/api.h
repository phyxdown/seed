#ifndef __SEED_CONCURRENT_H
#define __SEED_CONCURRENT_H

typedef struct cqmthz {
	int  (*Enqueue)(struct cqmthz *q, void *v);
	int  (*Dequeue)(struct cqmthz *q, void **v);
	void (*Release)(struct cqmthz *q);
} cqmthz;

cqmthz* NewSimpleBlockingQueue();
void FreeSimpleBlockingQueue(cqmthz* q);

cqmthz* NewOptimisitcFIFOQueue();
#endif
