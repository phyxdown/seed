#include "q2.h"

struct q2 {
	q2Node *tail;
	q2Node *head;
};

struct q2Node {
	void *value;
	q2Node *next;
	q2Node *prev;
};


