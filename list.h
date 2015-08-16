#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>

/* general double linked list */

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

list* listCreate();
void  listRelease(list* list);
list* listAddNodeHead(list* list, void* value);
list* listAddNodeTail(list* list, void* value);

listIter* listGetIterator(list* list, int direction);
listNode* listNext(listIter* iter);
void listReleaseIterator(listIter* iter);

#define LIST_START_HEAD 0
#define LIST_START_TAIL 1

#endif /* __LIST_H__ */
