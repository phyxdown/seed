#include <stdlib.h>

#include <pthread.h>

#include "api.h"

typedef struct seed_concurrent_bq      seed_concurrent_bq;
typedef struct seed_concurrent_bq_node seed_concurrent_bq_node;

struct seed_concurrent_bq {
	seed_concurrent_bq_node *tail; 
	seed_concurrent_bq_node *head; 
	pthread_mutex_t         *mutex;
	void                    *methods[];
};

struct seed_concurrent_bq_node {
	seed_concurrent_bq_node *next; 
	void                    *value;
};

static void  seed_concurrent_bq_enqueue(cqmthz *queue, void *value) {}
static void* seed_concurrent_bq_dequeue(cqmthz *queue) {
	return NULL;
}
static void  seed_concurrent_bq_release(cqmthz *queue) {}

cqmthz*
NewSimpleBlockingQueue() {
	seed_concurrent_bq* bq;
	if ((bq = malloc(sizeof(struct seed_concurrent_bq)+sizeof(struct cqmthz))) == NULL) {
		return NULL;
	}
	if ((bq->mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t))) == NULL) {
		free(bq);
		return NULL;
	}
	pthread_mutex_init(bq->mutex, NULL);
	bq->tail = bq->head = NULL;

	cqmthz* m = (cqmthz*)bq->methods;
	m->Enqueue = &seed_concurrent_bq_enqueue;
	m->Dequeue = &seed_concurrent_bq_dequeue;
	m->Release = &seed_concurrent_bq_release;
	return m;
}
