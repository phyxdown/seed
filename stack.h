#ifndef __STACK_H__
#define __STACK_H__

typedef struct stackNode {
	struct listNode* next;
	void* value;
} stackNode;

typedef struct stack {
	listNode *top;
	void (*free) (void* ptr);
} stack;

stack* stackCreate();
void stackRelease(stack* stack);

void* pop(stack* stack);
void* peek(stack* stack);
void* push(stack* stack, void* value);

#endif /* __STACK_H */
