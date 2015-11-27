#ifndef __SEED_Q_H
#define __SEED_Q_H

/* Implementation of Q is an optimistic approach to lock-free FIFO queues */

struct qPointer;
struct q;
struct qNode;

typedef struct qPointer qPointer;
typedef struct q        q;
typedef struct qNode    qNode;

typedef q* ConcurrentQueue;

q*    qNew     ();
void  qRelease (q* queue);

void  qEnqueue (q* queue, void *value);
void* qDequeue (q* queue);

#endif
