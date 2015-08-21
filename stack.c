#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

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
	return stack->top->value;
}

void*
stackPush(stack* stack, void* value) {}
