#ifndef __STACK_H__
#define __STACK_H__

typedef struct stackNode {
	struct stackNode* next;
	void* value;
} stackNode;

typedef struct stack {
	stackNode *top;
	void (*free) (void* ptr);
} stack;

stack* stackCreate();
void stackRelease(stack* stack);

void* stackPop(stack* stack);
void* stackPeek(stack* stack);
void* stackPush(stack* stack, void* value);

#endif /* __STACK_H */
