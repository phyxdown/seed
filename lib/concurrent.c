#include <stdlib.h>

/* 3rd party 5*/
#include <pthread.h>
#define Mutex pthread_mutex_t
#define mutex_init    pthread_mutex_init
#define mutex_release free
#define mutex_lock    pthread_mutex_lock
#define mutex_unlock  pthread_mutex_unlock

/* seed 5*/
#include "fmpool.h"
#define Pool fmpool
#define pool_create  fmpoolCreate
#define pool_release free
#define pool_alloc   fmpoolAlloc
#define pool_free    fmpoolFree

/* header 1*/
#include "concurrent.h"
#define Interface seed_concurrent_queue

/* internal 9*/
#define Queue  _seed_concurrent_queue
#define Node   _seed_concurrent_queue_node
#define Status _seed_concurrent_queue_status

#define OK          _seed_concurrent_queue_status_ok
#define FULL        _seed_concurrent_queue_status_full
#define ERR_INVALID _seed_concurrent_queue_status_err_invalid
#define ERR_LOCK    _seed_concurrent_queue_status_err_lock
#define ERR_UNLOCK  _seed_concurrent_queue_status_err_unlock

#define itos(P) \
	((Queue*)(((char*)(P)) - sizeof(Queue)))

typedef struct Queue  Queue;
typedef struct Node   Node;
typedef enum   Status Status;

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

enum Status {
	ERR_LOCK = -1, 
	ERR_UNLOCK = -2, 
};

static int enqueue(Interface *queue, void *value) {
	Queue* q = itos(queue);
	Node* nd; nd = (Node*)pool_alloc(q->pool);
	if (nd == NULL) return 0;
	nd->value = value;
	if (0 != mutex_lock(q->mutex)) return ERR_LOCK;
	if (q->head == NULL) {
		q->tail = q->head = nd;
		nd->prev = nd->next = NULL;
	} else {
		nd->prev = NULL;
		nd->next = q->head;
		nd->next->prev = nd;
		q->head = nd;
	}
	if (0 != mutex_unlock(q->mutex)) return ERR_UNLOCK;
	return 1;
}

static int dequeue(Interface *queue, void **value) {
	Queue* q = itos(queue);
	if (0 != mutex_lock(q->mutex)) return ERR_LOCK;
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
		if (0 != mutex_unlock(q->mutex)) return ERR_UNLOCK;
		return 1;
	}
	if (0 != mutex_unlock(q->mutex)) return ERR_UNLOCK;
	return 0;
}

static int batchDequeue(Interface *queue, void **value, int length) {
	Queue* q = itos(queue);
	if (0 != mutex_lock(q->mutex)) return ERR_LOCK;
	if (q->tail != NULL) {
		int i;
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
		if (0 != mutex_unlock(q->mutex)) return ERR_UNLOCK;
		return i;
	}
	if (0 != mutex_unlock(q->mutex)) return ERR_UNLOCK;
	return 0;
}

static void release(Interface *queue) {
	Queue* q = itos(queue);
	mutex_release(q->mutex);
	pool_release(q->pool);
	free(q);
}

Interface*
seed_concurrent_queue_create(size_t limit) {
	Queue* q;
	if ((q = malloc(sizeof(Queue)+sizeof(Interface))) == NULL) {
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

	Interface* m = (Interface*)q->methods;
	m->enqueue       = &enqueue;
	m->dequeue       = &dequeue;
	m->batchDequeue = &batchDequeue;
	m->release       = &release;
	return m;
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
#undef Interface

/* internal 9*/
#undef Queue
#undef Node
#undef Status
#undef OK
#undef FULL
#undef ERR_INVALID
#undef ERR_LOCK
#undef ERR_UNLOCK
#undef itos
