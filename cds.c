#include <stdlib.h>
#include "cds.h"

void
cEnqueue(cQueue* cqueue, void* value) {
}

void*
cDequeue(cQueue* cqueue) {
	return NULL;
}

cQueue* cqueueCreate(size_t capacity) {
	cQueue* cqueue;
	if ((cqueue = malloc(sizeof(*cqueue))) == NULL) return NULL;
	if ((cqueue->buffer = malloc(sizeof(void*)*capacity)) == NULL) {
		free(cqueue);
		return NULL;
	}
	cqueue->capacity = capacity;
	cqueue->size = cqueue->in = cqueue->out = 0;
	pthread_mutex_init(&cqueue->mutex, NULL);
	pthread_cond_init(&cqueue->cond_full, NULL);
	pthread_cond_init(&cqueue->cond_empty, NULL);
	return NULL;
}
