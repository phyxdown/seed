#include <stdlib.h>

#include <pthread.h>

#include "concurrent.h"

typedef struct seed_concurrent_lock_based_queue        seed_concurrent_lock_based_queue;
typedef struct seed_concurrent_lock_based_queue_node   seed_concurrent_lock_based_queue_node;
typedef enum   seed_concurrent_lock_based_queue_status seed_concurrent_lock_based_queue_status;


/* 3rd party*/
#define Mutex pthread_mutex_t
#define mutex_init   pthread_mutex_init
#define mutex_lock   pthread_mutex_lock
#define mutex_unlock pthread_mutex_unlock

/* header*/
#define interface seed_concurrent_queue_methods

/* internal */
#define Queue  seed_concurrent_lock_based_queue
#define Node   seed_concurrent_lock_based_queue_node
#define Status seed_concurrent_lock_based_queue_status

#define OK          seed_concurrent_lock_based_queue_status_ok
#define ERR_INVALID seed_concurrent_lock_based_queue_status_err_invalid
#define ERR_LOCK    seed_concurrent_lock_based_queue_status_err_lock

#define itos(P) \
	((Queue*)(((char*)(P)) - sizeof(Queue)))

struct Queue {
	Node *tail; 
	Node *head; 
	Mutex *mutex;
	void *methods[];
};

struct Node {
	Node *next; 
	Node *prev; 
	void *value;
};

enum Status {
	OK = 0, 
	ERR_INVALID = -1, 
	ERR_LOCK = -2, 
};

static int enqueue(interface *queue, void *value) {
	Queue* q = itos(queue);
	if (q == NULL) return ERR_INVALID;
	if (0 != mutex_lock(q->mutex)) return ERR_LOCK;
	Node* nd; nd = malloc(sizeof(*nd));
	nd->value = value;
	if (q->head == NULL) {
		q->tail = q->head = nd;
		nd->prev = nd->next = NULL;
	} else {
		nd->prev = NULL;
		nd->next = q->head;
		nd->next->prev = nd;
		q->head = nd;
	}
	if (0 != mutex_unlock(q->mutex)) return ERR_LOCK;
	return OK;
}
static int dequeue(interface *queue, void **value) {
	Queue* q = itos(queue);
	if (q == NULL) return ERR_INVALID;
	if (0 != mutex_lock(q->mutex)) return ERR_LOCK;
	if (q->tail == NULL) {
		*value = NULL;
	} else {
		*value = q->tail->value;
		if (q->tail->prev == NULL) {
			free(q->tail);
			q->head = q->tail = NULL;
		} else {
			q->tail = q->tail->prev;
			free(q->tail->next);
			q->tail->next = NULL;
		}
	}
	if (0 != mutex_unlock(q->mutex)) return ERR_LOCK;
	return OK;
}
static void release(interface *queue) {
	if (queue == NULL) return;
	Queue* q = itos(queue);
	free(q->mutex);
	free(q);
}

interface*
seed_concurrent_lock_based_queue_create() {
	Queue* q;
	if ((q = malloc(sizeof(Queue)+sizeof(struct interface))) == NULL) {
		return NULL;
	}
	if ((q->mutex = (Mutex*)malloc(sizeof(Mutex))) == NULL) {
		free(q);
		return NULL;
	}
	mutex_init(q->mutex, NULL);
	q->tail = q->head = NULL;

	interface* m = (interface*)q->methods;
	m->enqueue = &enqueue;
	m->dequeue = &dequeue;
	m->release = &release;
	return m;
}
/* 3rd party*/
#undef Mutex
#undef mutex_init
#undef mutex_lock
#undef mutex_unlock

/* header*/
#undef interface

/* internal */
#undef Queue
#undef Node
#undef Status
#undef OK
#undef ERR_INVALID
#undef ERR_LOCK
#undef itos
