#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#include "queue/queue.h"

// struct timespec
// tv_sec, tv_nsec
// clock_gettime(CLOCK_MONOTONIC, &before);
//
// exit(EXIT_SUCCESS)
// exit(EXIT_FAILURE)

#define milliToSec 1000000
#define microToSec 1000000000

queue q;
queue core;

int nproc;
int resol;

typedef enum SCHEDULING_METHOD {FCFS, SJF, HRN}sched;
typedef enum BOOLEAN {TRUE, FALSE} boolean;

sched sch = FCFS;

static unsigned long GetDeltaTime(struct timespec* start, struct timespec* end){
	return (end->tv_sec * milliToSec + end->tv_nsec) - (start->tv_sec * milliToSec + start->tv_nsec);
}

// work
static int Load(long t){
	// 받은 t / resol 만큼 while처리 합니다
	// 추후 setTask를 통하여 남은 task를 저장하고 
	// 남은 task가 0이면 큐를 삭제하던가 말던가
	long thread = getThread(&q, getPID(&q));

	printf("<%d>task amount %ld / %ld\n", getPID(&q), getTask(&q, getPID(&q)), thread);
	
	// setTask -> thread만큼 task를 실행, 감소
	if (setTask(&q, getPID(&q), thread)){
		// 작업 시간
		struct timespec task_time;
		clock_gettime(CLOCK_MONOTONIC, &task_time);
		
		// 결과 출력
		// printf("<%d>done (%ld ns)\n", getPID(&q), GetDeltaTime(getArrTime(&q, getPID(&q)), &task_time));
		printf("<%d>done (%ld ns)\n", getPID(&q), task_time.tv_sec * 1000000 + task_time.tv_nsec);

		delete(&q, getPID(&q));
	}else{
		printf("leaf_task_amount %ld\n", getTask(&q, getPID(&q)));
	}

	while(thread--);
}

static void child(){
	// int stop = 0;
	int i;
	int Ncore, PID, task;	

	int loop = 0;
	
	// divid task time into resolution
	setThread(&q, resol);
	
	// sort process as defined scheduler method
	switch (sch){
		// first in first out
		case FCFS:
			break;
		// small task first
		case SJF:
			minSortQueue(&q);
			break;
		// highest respone ratio next
		case HRN:
			break;

		default:
			puts("Please Select CPU Scheduler\n");
			fflush(NULL);

			break;
	}

	while(pop(&q) != -1)
	{
		loop++;

		Ncore = pop(&core);
		
		PID = getPID(&q);
		task = getTask(&q, PID);
		
		printf("Ncore<%d>\n", Ncore);
		Load(task);

		printf("\n");

		// stop++;
		// if (stop > 30)
		// 	break;
	}
	
	printf("<%d> per Loop\n", loop);
	exit(EXIT_SUCCESS);
}

static void parent(){
	wait(NULL);
}

int main(long argc, char* args[]){
	int task;
	int PID;

	// process routine
	int i;

	// get option
	if (argc < 3){
		puts("main <nproc><resol>\n");
		fflush(NULL);

		exit(EXIT_FAILURE);
	}
	
	nproc = atoi(args[1]);
	resol = atoi(args[2]);

	if (nproc < 1){
		puts("You should get nproc values 1 or more.\n");
		fflush(NULL);
		
		exit(EXIT_FAILURE);
	}
	if (resol < 1){
		puts("You should get resolution values 1 or more.\n");
		fflush(NULL);

		exit(EXIT_FAILURE);
	}
	
	// coreQueue
	init(&core);

	for (i = 0; i < nproc; i++)
		push(&core, i);

	// init readyQueue
	init(&q);
	
	// insert Task
	push(&q, 1100000000UL);
	push(&q, 600000000UL);
	push(&q, 500000000UL);
	push(&q, 400000000UL);
	push(&q, 300000000UL);
	push(&q, 800000000UL);
	push(&q, 200000000UL);
	push(&q, 100000000UL);

	printQueue(&q);
	
	PID = fork();
	
	if (PID < 0){
		puts("Can't created or duplicated new process\n");
		fflush(NULL);
		
		exit(EXIT_FAILURE);
	}
	else if (PID == 0){
		child();
	}
	else{
		parent();
	}
	
	exit(EXIT_SUCCESS);
}
