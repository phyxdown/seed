#include <stdlib.h>

typedef struct seed_concurrent_lock_based_queue        seed_concurrent_lock_based_queue;
typedef struct seed_concurrent_lock_based_queue_node   seed_concurrent_lock_based_queue_node;
typedef enum   seed_concurrent_lock_based_queue_status seed_concurrent_lock_based_queue_status;

/* 3rd party 4*/
#include <pthread.h>
#define Mutex pthread_mutex_t
#define mutex_init   pthread_mutex_init
#define mutex_lock   pthread_mutex_lock
#define mutex_unlock pthread_mutex_unlock

/* seed 5*/
#include "fmpool.h"
#define Pool fmpool
#define pool_create  fmpoolCreate
#define pool_release free
#define pool_alloc   fmpoolAlloc
#define pool_free    fmpoolFree

/* header 1*/
#include "concurrent.h"
#define Interface seed_concurrent_queue_methods

/* internal 9*/
#define Queue  seed_concurrent_lock_based_queue
#define Node   seed_concurrent_lock_based_queue_node
#define Status seed_concurrent_lock_based_queue_status

#define OK          seed_concurrent_lock_based_queue_status_ok
#define ERR_FULL    seed_concurrent_lock_based_queue_status_err_full
#define ERR_INVALID seed_concurrent_lock_based_queue_status_err_invalid
#define ERR_LOCK    seed_concurrent_lock_based_queue_status_err_lock
#define ERR_UNLOCK  seed_concurrent_lock_based_queue_status_err_unlock

#define itos(P) \
	((Queue*)(((char*)(P)) - sizeof(Queue)))

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
	OK = 0, 
	ERR_FULL = -1,
	ERR_INVALID = -2, 
	ERR_LOCK = -3, 
	ERR_UNLOCK = -4, 
};

static int enqueue(Interface *queue, void *value) {
	Queue* q = itos(queue);
	if (q == NULL) return ERR_INVALID;
	Node* nd; nd = (Node*)pool_alloc(q->pool);
	if (nd == NULL) return ERR_FULL;
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
	return OK;
}
static int dequeue(Interface *queue, void **value) {
	Queue* q = itos(queue);
	if (q == NULL) return ERR_INVALID;
	if (0 != mutex_lock(q->mutex)) return ERR_LOCK;
	if (q->tail == NULL) {
		*value = NULL;
	} else {
		*value = q->tail->value;
		if (q->tail->prev == NULL) {
			pool_free(q->tail);
			q->head = q->tail = NULL;
		} else {
			q->tail = q->tail->prev;
			pool_free(q->tail->next);
			q->tail->next = NULL;
		}
	}
	if (0 != mutex_unlock(q->mutex)) return ERR_UNLOCK;
	return OK;
}
static void release(Interface *queue) {
	if (queue == NULL) return;
	Queue* q = itos(queue);
	free(q->mutex);
	free(q);
}

Interface*
seed_concurrent_lock_based_queue_create(size_t limit) {
	Queue* q;
	if ((q = malloc(sizeof(Queue)+sizeof(struct Interface))) == NULL) {
		return NULL;
	}
	if ((q->mutex = (Mutex*)malloc(sizeof(Mutex))) == NULL) {
		free(q);
		return NULL;
	}
	if ((q->pool = pool_create(limit, sizeof(Node))) == NULL) {
		free(q);
		free(q->mutex);
		return NULL;
	}
	mutex_init(q->mutex, NULL);
	q->tail = q->head = NULL;

	Interface* m = (Interface*)q->methods;
	m->enqueue = &enqueue;
	m->dequeue = &dequeue;
	m->release = &release;
	return m;
}
/* 3rd party 4*/
#undef Mutex
#undef mutex_init
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
#undef ERR_FULL
#undef ERR_INVALID
#undef ERR_LOCK
#undef ERR_UNLOCK
#undef itos
