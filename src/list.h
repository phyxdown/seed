#ifndef __SEED_LIST_H
#define __SEED_LIST_H

#include <stdio.h>

typedef struct seed_list_node {
	struct seed_list_node* prev;
	struct seed_list_node* next;
	void* value;
} seed_list_node;

typedef struct seed_list_iterator {
	seed_list_node* next;
	int direction;
} seed_list_iterator;

typedef struct seed_list {
	seed_list_node* head;
	seed_list_node* tail;
	size_t len;
	void* (*dup) (void *ptr);
	void (*free) (void *ptr);
	int (*match) (void *ptr, void *key);
} seed_list;

seed_list* seed_list_create();
void       seed_list_release(seed_list* list);

seed_list* seed_list_add_node_head(seed_list* list, void* value);
seed_list* seed_list_add_node_tail(seed_list* list, void* value);

seed_list_iterator* seed_list_get_iterator(seed_list* list, int direction);
seed_list_node* seed_list_next(seed_list_iterator* iter);
void                seed_list_release_iterator(seed_list_iterator* iter);

#define SEED_LIST_START_HEAD 0
#define SEED_LIST_START_TAIL 1

typedef seed_list_node seed_stack_node;
typedef seed_list seed_stack;

seed_stack* seed_stack_create();
void seed_stack_release(seed_stack* stack);

void* seed_stack_pop(seed_stack* stack);
void* seed_stack_peek(seed_stack* stack);
seed_stack* seed_stack_push(seed_stack* stack, void* value);



typedef seed_list_node seed_queue_node;
typedef seed_list seed_queue;

seed_queue* seed_queue_create();
void seed_queue_release(seed_queue* queue);

seed_queue* seed_queue_enqueue(seed_queue* queue, void* value);
void* seed_queue_dnqueue(seed_queue* queue);
#endif
