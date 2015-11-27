#ifndef __SEED_Q1_H
#define __SEED_Q1_H

/* Implementation of Q1 is an optimistic approach to lock-free FIFO queues */

struct q1Pointer;
struct q1;
struct q1Node;

typedef struct q1Pointer q1Pointer;
typedef struct q1        q1;
typedef struct q1Node    q1Node;

typedef q1* OptimisticFIFOQueue;

q1*   q1New     ();
void  q1Release (q1* queue);

void  q1Enqueue (q1* queue, void *value);
void* q1Dequeue (q1* queue);

#endif
