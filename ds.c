/* list.c */
#include <stdio.h>
#include <stdlib.h>

#include "ds.h"

list*
listCreate() {
	list* list;
	if ((list = malloc(sizeof(*list))) == NULL) 
		return NULL;
	list->head = list->tail = NULL;
	list->len = 0;
	list->dup = NULL;
	list->free = NULL;
	list->match = NULL;
	return list;
}

void
listRelease(list* list) {
	size_t len;
	listNode* current;
	listNode* next;

	current = list->head;
	len = list->len;
	while(len--) {
		next = current->next;
		if (list->free) list->free(current->value);
		free(current);
		current = next;
	}
	free(list);
}

list*
listAddNodeHead(list* list, void* value) {
	listNode* node;
	if((node = malloc(sizeof(*node))) == NULL)
		return NULL;
	node->value = value;
	if (list->len == 0) {
		list->head = list->tail = node;
		node->prev = node->next = NULL;
	} else {
		node->prev = NULL;
		node->next = list->head;
		node->next->prev = node;
		list->head = node;
	}
	list->len ++;
	return list;
}

list*
listAddNodeTail(list* list, void* value) {
	listNode* node;
	if((node = malloc(sizeof(*node))) == NULL)
		return NULL;
	node->value = value;
	if (list->len = 0 ) {
		list->head = list->tail = node;
		node->prev = node->next = NULL;
	} else {
		node->prev = list->tail;
		node->next = NULL;
		node->prev->next = node;
		list->tail = node;
	}
	list->len ++;
	return list;
}

listIter*
listGetIterator(list* list, int direction) {
	listIter* iter;
	if ((iter = malloc(sizeof(*iter))) == NULL) return NULL;
	if (direction == LIST_START_HEAD) iter->next = list->head;
	else iter->next = list->tail;
	iter->direction = direction;
	return iter;
}


listNode*
listNext(listIter* iter) {
	listNode* current = iter->next;

	if (current != NULL) {
		if (iter->direction == LIST_START_HEAD)
			iter->next = current->next;
		else 
			iter->next = current->prev;
	}
	return current;
}

void
listReleaseIterator(listIter* iter) {
	free(iter);
}


/* stack.c */
#include <stdio.h>
#include <stdlib.h>

#include "ds.h"

stack*
stackCreate() {
	stack* stack;
	if ((stack = malloc(sizeof(*stack))) == NULL) 
		return NULL;
	stack->top = NULL;
	stack->free = NULL;
	return stack;
}

void
stackRelease(stack* stack) {}

void*
stackPop(stack* stack) {
	void* value;
	stackNode* current;
	if ((current = stack->top) == NULL)
		return NULL;
	stack->top = current->next;
	current->next = NULL;
	value = current->value;
	free(current);
	return value;
}

void*
stackPeek(stack* stack) {
	stackNode* current;
	if ((current = stack->top) == NULL) return NULL;
	return current->value;
}

void*
stackPush(stack* stack, void* value) {
	stackNode* node;
	if ((node = malloc(sizeof(*node))) == NULL) return NULL;
	node->value = value;
	node->next = stack->top;
	stack->top = node;
	return value;
}
