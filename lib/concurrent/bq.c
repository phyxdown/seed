#include <stdlib.h>

#include <pthread.h>

#include "api.h"

typedef struct seed_concurrent_bq        seed_concurrent_bq;
typedef struct seed_concurrent_bq_node   seed_concurrent_bq_node;
typedef enum   seed_concurrent_bq_status seed_concurrent_bq_status;

#define seed_concurrent_bq_header(P) \
	((seed_concurrent_bq*)(((char*)(P)) - sizeof(seed_concurrent_bq)))


struct seed_concurrent_bq {
	seed_concurrent_bq_node *tail; 
	seed_concurrent_bq_node *head; 
	pthread_mutex_t         *mutex;
	void                    *methods[];
};

struct seed_concurrent_bq_node {
	seed_concurrent_bq_node *next; 
	seed_concurrent_bq_node *prev; 
	void                    *value;
};

enum seed_concurrent_bq_status {
	seed_concurrent_bq_status_ok = 0, 
	seed_concurrent_bq_status_err_invalue = -1, 
	seed_concurrent_bq_status_err_lock = -2, 
};

static int seed_concurrent_bq_enqueue(cqmthz *q, void *v) {
	seed_concurrent_bq* bq = seed_concurrent_bq_header(q);
	if (bq == NULL) return seed_concurrent_bq_status_err_invalue;
	if (0 != pthread_mutex_lock(bq->mutex)) return seed_concurrent_bq_status_err_lock;
	seed_concurrent_bq_node* nd; nd = malloc(sizeof(*nd));
	nd->value = v;
	if (bq->head == NULL) {
		bq->tail = bq->head = nd;
		nd->prev = nd->next = NULL;
	} else {
		nd->prev = NULL;
		nd->next = bq->head;
		nd->next->prev = nd;
		bq->head = nd;
	}
	if (0 != pthread_mutex_unlock(bq->mutex)) return seed_concurrent_bq_status_err_lock;
	return seed_concurrent_bq_status_ok;
}
static int seed_concurrent_bq_dequeue(cqmthz *q, void **v) {
	seed_concurrent_bq* bq = seed_concurrent_bq_header(q);
	if (bq == NULL) return seed_concurrent_bq_status_err_invalue;
	if (0 != pthread_mutex_lock(bq->mutex)) return seed_concurrent_bq_status_err_lock;
	if (bq->tail == NULL) {
		*v = NULL;
	} else {
		*v = bq->tail->value;
		if (bq->tail->prev == NULL) {
			free(bq->tail);
			bq->head = bq->tail = NULL;
		} else {
			bq->tail = bq->tail->prev;
			free(bq->tail->next);
			bq->tail->next = NULL;
		}
	}
	if (0 != pthread_mutex_unlock(bq->mutex)) return seed_concurrent_bq_status_err_lock;
	return seed_concurrent_bq_status_ok;
}
static void seed_concurrent_bq_release(cqmthz *q) {
	seed_concurrent_bq* bq = seed_concurrent_bq_header(q);
	free(bq->mutex);
	free(bq);
}

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
