#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct q1Pointer {
	int32_t tag;
	int32_t target;
} q1Pointer;


int main() {
	printf("%d %d\n", sizeof(struct q1Pointer), sizeof(void*));
	int *pa = (int*)malloc(4);
	int i = 1;
	memcpy(pa, &i, 1);

	printf("%d %p\n", *pa, pa) ;

	if ( (unsigned long)pa <= (unsigned long)0xffffffff) {
		q1Pointer p;
		p.target = (unsigned long)pa;
		p.tag = 0x1;
		
		printf("%p\n", p) ;
		
		unsigned long t = p.target;
		printf("%d\n", *((int*)t));
		printf("%x\n", p.tag);
	} else {
		return 1;
	}
}
