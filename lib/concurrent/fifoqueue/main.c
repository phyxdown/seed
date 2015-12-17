#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct q1Pointer {
	unsigned long tag;
	unsigned long target;
} q1Pointer;


int main() {
	int *pa = (int*)malloc(4);
	int i = 2;
	memcpy(pa, &i, 1);

	printf("%d %p\n", *pa, pa) ;

	if ( (unsigned long)pa <= (unsigned long)0xffffffff) {
		q1Pointer p;
		p.target = (unsigned long)pa;
		p.tag = 0x1;
		
		printf("%p\n", p) ;
		
		unsigned long t = (unsigned long)p.target;
		printf("%d\n", *((int*)t));
		printf("%x\n", p.tag);
	} else {
		return 1;
	}
}
