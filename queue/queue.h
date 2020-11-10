#include "queue.c"

enum PID_STATE;

typedef struct Queue queue;

void init(queue* q);

int isFull(queue* q);

int isEmpty(queue* q);

void push(queue* q, int task);

int pop(queue* q);

int getPID(queue* q);

long getTask(queue* q, int PID);

enum PID_STATE getState(queue* q, int PID);

void delete(queue* q, int del);

void minSortQueue(queue* q);

void maxSortQueue(queue* q);

void printQueue(queue* q);
