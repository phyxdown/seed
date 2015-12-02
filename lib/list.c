/* double linked list */
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

list* listCreate() {
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

void listRelease(list* list) {
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

list* listAddNodeHead(list* list, void* value) {
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

list* listAddNodeTail(list* list, void* value) {
	listNode* node;
	if((node = malloc(sizeof(*node))) == NULL)
		return NULL;
	node->value = value;
	if (list->len == 0 ) {
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

listIter* listGetIterator(list* list, int direction) {
	listIter* iter;
	if ((iter = malloc(sizeof(*iter))) == NULL) return NULL;
	if (direction == LIST_START_HEAD) iter->next = list->head;
	else iter->next = list->tail;
	iter->direction = direction;
	return iter;
}


listNode* listNext(listIter* iter) {
	listNode* current = iter->next;

	if (current != NULL) {
		if (iter->direction == LIST_START_HEAD)
			iter->next = current->next;
		else 
			iter->next = current->prev;
	}
	return current;
}

void listReleaseIterator(listIter* iter) {
	free(iter);
}


/* stack */
stack* stackCreate() {
	return listCreate();
}

void stackRelease(stack* stack) {
	listRelease(stack);
}

void* stackPop(stack* stack) {
	printf("0\n");
	void* value;
	printf("1\n");
	if (stack->head == NULL) {value = NULL;
	printf("2\n");}
	else {
	printf("3\n");
		value = stack->head->value;
	printf("4\n");
		if (stack->head->next == NULL) {
	printf("5\n");
			free(stack->head);
	printf("6\n");
			stack->head = stack->tail = NULL;
	printf("7\n");
		} else {
	printf("8\n");
			stack->head = stack->head->next;
	printf("9\n");
			free(stack->head->prev);
	printf("10\n");
			stack->head->prev = NULL;
	printf("11\n");
		}
		stack->len --;
	printf("12\n");
	}
	return value;

	/*
	stackNode* current;
	if ((current = stack->head) == NULL) return NULL;
	stack->head = current->next;
	if (stack->head)
		stack->head->prev = NULL;
	current->next = NULL;
	value = current->value;
	free(current);
	return value;
	*/
}

void* stackPeek(stack* stack) {
	stackNode* current;
	if ((current = stack->head) == NULL) return NULL;
	return current->value;
}

stack* stackPush(stack* stack, void* value) {
	return listAddNodeHead(stack, value);
}


/* queue */
queue* queueCreate() {
	return listCreate();
}

void queueRelease(stack* stack) {
	return listRelease(stack);
}

queue* queueEnqueue(stack* stack, void* value) {
	return listAddNodeTail(stack, value);
}

void* queueDnqueue(stack* stack) {
	return stackPop(stack);
}
