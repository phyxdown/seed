/* 3rd party 5*/
#include <pthread.h>
#define Mutex pthread_mutex_t
#define mutex_init    pthread_mutex_init
#define mutex_release free
#define mutex_lock    pthread_mutex_lock
#define mutex_unlock  pthread_mutex_unlock
	/**
	 * Programming logic in this file MUST ensure that pthread_mutex_lock
	 * & pthread_mutex_unlock doesn't return errors.
	 * */

/* seed 5*/
#include "fmpool.h"
#define Pool fmpool
#define pool_create  fmpoolCreate
#define pool_release free
#define pool_alloc   fmpoolAlloc
#define pool_free    fmpoolFree

/* header 1*/
#include "concurrent.h"
#define IQueue seed_concurrent_queue

/* internal 3*/
#define Queue  _seed_concurrent_queue
#define Node   _seed_concurrent_queue_node
#define itos(P) \
	((Queue*)(((char*)(P)) - sizeof(Queue)))

typedef struct Queue  Queue;
typedef struct Node   Node;

struct Queue {
	Node *tail; 
	Node *head; 
	Mutex *mutex;
	Pool *pool;
	void *methods[];
};

struct Node {
	Node *next; 
	Node *prev; 
	void *value;
};

static size_t enqueue(IQueue *queue, void *value) {
	Queue* q = itos(queue);
	Node* nd; nd = (Node*)pool_alloc(q->pool);
	if (nd == NULL) return 0;
	nd->value = value;
	mutex_lock(q->mutex);
	if (q->head == NULL) {
		q->tail = q->head = nd;
		nd->prev = nd->next = NULL;
	} else {
		nd->prev = NULL;
		nd->next = q->head;
		nd->next->prev = nd;
		q->head = nd;
	}
	mutex_unlock(q->mutex);
	return 1;
}

static size_t dequeue(IQueue *queue, void **value) {
	Queue* q = itos(queue);
	mutex_lock(q->mutex);
	if (q->tail != NULL) {
		*value = q->tail->value;
		if (q->tail->prev == NULL) {
			pool_free(q->tail);
			q->head = q->tail = NULL;
		} else {
			q->tail = q->tail->prev;
			pool_free(q->tail->next);
			q->tail->next = NULL;
		}
		mutex_unlock(q->mutex);
		return 1;
	}
	mutex_unlock(q->mutex);
	return 0;
}

static size_t batchDequeue(IQueue *queue, void **value, size_t length) {
	Queue* q = itos(queue);
	mutex_lock(q->mutex);
	if (q->tail != NULL) {
		size_t i;
		for (i = 0; i < length; i++) {
			*(value + i) = q->tail->value;
			if (q->tail->prev == NULL) {
				pool_free(q->tail);
				q->head = q->tail = NULL;
				i++;
				break;
			} else {
				q->tail = q->tail->prev;
				pool_free(q->tail->next);
				q->tail->next = NULL;
			}
		}
		mutex_unlock(q->mutex);
		return i;
	}
	mutex_unlock(q->mutex);
	return 0;
}

static void release(IQueue *queue) {
	Queue* q = itos(queue);
	mutex_release(q->mutex);
	pool_release(q->pool);
	free(q);
}

IQueue*
seed_concurrent_queue_create(size_t limit) {
	Queue* q;
	if ((q = malloc(sizeof(Queue)+sizeof(IQueue))) == NULL) {
		return NULL;
	}
	if ((q->mutex = (Mutex*)malloc(sizeof(Mutex))) == NULL) {
		free(q);
		return NULL;
	}
	if ((q->pool = pool_create(limit, sizeof(Node))) == NULL) {
		mutex_release(q->mutex);
		free(q);
		return NULL;
	}
	mutex_init(q->mutex, NULL);
	q->tail = q->head = NULL;

	IQueue* queue = (IQueue*)q->methods;
	queue->enqueue      = &enqueue;
	queue->dequeue      = &dequeue;
	queue->batchDequeue = &batchDequeue;
	queue->release      = &release;
	return queue;
}
/* 3rd party 5*/
#undef Mutex
#undef mutex_init
#undef mutex_release
#undef mutex_lock
#undef mutex_unlock

/* seed 5*/
#undef Pool
#undef pool_create
#undef pool_release
#undef pool_alloc
#undef pool_free

/* header 1*/
#undef IQueue

/* internal 3*/
#undef Queue
#undef Node
#undef itos
