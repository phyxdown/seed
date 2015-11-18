#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

#include "q1.h"

struct q1Pointer {
	q1Node* target;
	int32_t tag;
};

struct q1 {
	q1Pointer tail;
	q1Pointer head;
};

struct q1Node {
	void *value;
	q1Pointer next;
	q1Pointer prev;
};

static q1Node* new_node() { return NULL; }
static void free_node() {}

q1* q1New() {
	q1* q; q = (q1*)malloc(sizeof(*q));

	q1Node* nd = new_node();
	nd->next = (q1Pointer){ NULL, 0 };

	q->tail = (q1Pointer){ nd, 0 };
	q->head = (q1Pointer){ nd, 0 };
	return q;
}
void  q1Release (q1* queue);

void  q1Enqueue (q1* queue, void *value);
void* q1Dequeue (q1* queue);

