/* __CONCURRENT_QUEUE_H__*/

#ifndef __CONCURRENT_QUEUE_H__
#define __CONCURRENT_QUEUE_H__

#include <pthread.h>
#include <sys/types.h>

typedef struct {
	void **buffer;
	size_t capacity;
	size_t size;
	size_t in;
	size_t out;
	pthread_mutex_t mutex;
	pthread_cond_t cond_full;
	pthread_cond_t cond_empty;
} cQueue;

void cEnqueue(cQueue* cqueue, void* value);
void* cDequeue(cQueue* cqueue);
cQueue* cqueueCreate(size_t capacity);

#endif /* __CONCURRENT_QUEUE_H__ */
