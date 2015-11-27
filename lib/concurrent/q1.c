#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

#include "q.h"

#define CAS(ptr, old, new) \
	 (__sync_bool_compare_and_swap((ptr), (old), (new)))

struct qPointer {
	qNode* target;
	int32_t tag;
};

struct q {
	qPointer tail;
	qPointer head;
};

struct qNode {
	void *value;
	qPointer next;
	qPointer prev;
};

static qNode* new_node() { return NULL; }
static void free_node() {}

q* qNew() {
	printf("New");
	/*
	q* nq;
	nq = (q*)malloc(sizeof(*nq));

	qNode* nd = new_node();
	nd->next = (qPointer){ NULL, 0 };

	nq->tail = (qPointer){ nd, 0 };
	nq->head = (qPointer){ nd, 0 };
	return nq;
	*/
	return NULL;
}
void  qRelease (q* queue) {
	printf("Release");
}

void  qEnqueue (q* queue, void *value) {
	printf("Enqueue");
	/*
	qPointer tail, temp;
	qNode* nd = new_node();
	nd->value = value;
	while(1) {
		tail = queue->tail;
		nd->next = (qPointer){ tail.target, tail.tag+1 };
                temp = (qPointer){ nd, tail.tag+1 };
		if (CAS( &(queue->tail) , tail, temp)) {
			(tail.target)->prev = (qPointer){ nd, tail.tag };
			break;
		}
	}
	*/
}

void* qDequeue (q* queue) {
	printf("Dequeue");
	return NULL;
}

