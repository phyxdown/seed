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

void stackRelease(stack* stack) {
}
