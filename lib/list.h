#ifndef __SEED_LIST_H
#define __SEED_LIST_H

#include <stdio.h>

typedef struct listNode {
	struct listNode* prev;
	struct listNode* next;
	void* value;
} listNode;

typedef struct listIter {
	listNode* next;
	int direction;
} listIter;

typedef struct list {
	listNode *head;
	listNode *tail;
	size_t len;
	void* (*dup) (void *ptr);
	void (*free) (void *ptr);
	int (*match) (void *ptr, void *key);
} list;

typedef struct list* List;

list* listCreate();
void  listRelease(list* list);
list* listAddNodeHead(list* list, void* value);
list* listAddNodeTail(list* list, void* value);

listIter* listGetIterator(list* list, int direction);
listNode* listNext(listIter* iter);
void listReleaseIterator(listIter* iter);

#define LIST_START_HEAD 0
#define LIST_START_TAIL 1

#endif

#ifndef __SEED_STACK_H
#define __SEED_STACK_H

typedef listNode stackNode;

typedef list stack;

typedef stack* Stack;

stack* stackCreate();
void stackRelease(stack* stack);

void* stackPop(stack* stack);
void* stackPeek(stack* stack);
void* stackPush(stack* stack, void* value);

#endif
