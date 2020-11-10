#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int main(){
	int i;
	queue q;

	init(&q);

	push(&q, 9);
	push(&q, 8);
	push(&q, 7);
	push(&q, 6);
	push(&q, 5);
	push(&q, 4);
	push(&q, 3);
	push(&q, 2);
	push(&q, 1);
	push(&q, 50);
	push(&q, 14);
	push(&q, 25);
	push(&q, 35);
	push(&q, 44);
	push(&q, 41);

	minSortQueue(&q);
	
	for (i = 0; i<14; i++){
		printf("%d: %d -> \n", i % q.rear, pop(&q));
	}
	printf("================================\n");
	delete(&q, 5);
	delete(&q, 2);
	
	for (i = 0; i<25; i++){
		printf("%d: %d -> \n", i % q.rear, pop(&q));
	}

	return 0;
}
