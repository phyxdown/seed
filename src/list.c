/* double linked list */
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

seed_list* seed_list_create() {
	seed_list* list;
	if ((list = malloc(sizeof(*list))) == NULL) 
		return NULL;
	list->head = list->tail = NULL;
	list->len = 0;
	list->dup = NULL;
	list->free = NULL;
	list->match = NULL;
	return list;
}

void seed_list_release(seed_list* list) {
	if (list == NULL) return;
	size_t len;
	seed_list_node* current;
	seed_list_node* next;

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

seed_list* seed_list_add_node_head(seed_list* list, void* value) {
	seed_list_node* node;
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

seed_list* seed_list_add_node_tail(seed_list* list, void* value) {
	seed_list_node* node;
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

seed_list_iterator* seed_list_get_iterator(seed_list* list, int direction) {
	seed_list_iterator* iter;
	if ((iter = malloc(sizeof(*iter))) == NULL) return NULL;
	if (direction == SEED_LIST_START_HEAD) iter->next = list->head;
	else iter->next = list->tail;
	iter->direction = direction;
	return iter;
}


seed_list_node* seed_list_next(seed_list_iterator* iter) {
	seed_list_node* current = iter->next;
	if (current != NULL) {
		if (iter->direction == SEED_LIST_START_HEAD)
			iter->next = current->next;
		else 
			iter->next = current->prev;
	}
	return current;
}

void seed_list_release_iterator(seed_list_iterator* iter) {
	if (NULL != iter) free(iter);
}

seed_stack* seed_stack_create() {
	return seed_list_create();
}

void seed_stack_release(seed_stack* stack) {
	seed_list_release(stack);
}

void* seed_stack_pop(seed_stack* stack) {
	void* value;
	if (stack->head == NULL) value = NULL;
	else {
		value = stack->head->value;
		if (stack->head->next == NULL) {
			free(stack->head);
			stack->head = stack->tail = NULL;
		} else {
			stack->head = stack->head->next;
			free(stack->head->prev);
			stack->head->prev = NULL;
		}
		stack->len --;
	}
	return value;
}

void* seed_stack_peek(seed_stack* stack) {
	seed_stack_node* current;
	if ((current = stack->head) == NULL) return NULL;
	return current->value;
}

seed_stack* seed_stack_push(seed_stack* stack, void* value) {
	return seed_list_add_node_head(stack, value);
}

seed_queue* seed_queue_create() {
	return seed_list_create();
}

void seed_queue_release(seed_queue* queue) {
	seed_list_release(queue);
}

seed_queue* seed_queue_enqueue(seed_queue* queue, void* value) {
	return seed_list_add_node_tail(queue, value);
}

void* seed_queue_dnqueue(seed_queue* queue) {
	return seed_stack_pop(queue);
}
