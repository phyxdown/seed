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
stackPop(stack* stack) {}

void*
stackPeek(stack* stack) {}

void*
stackPush(stack* stack, void* value) {}
