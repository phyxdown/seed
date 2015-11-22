#ifndef __SEED_Q2_H
#define __SEED_Q2_H

struct q2Pointer;
struct q2;
struct q2Node;

typedef struct q2Pointer q2Pointer;
typedef struct q2        q2;
typedef struct q2Node    q2Node;

typedef q2* SimpleBlockingQueue;

q2*   q2New     ();
void  q2Release (q2* queue);

void  q2Enqueue (q2* queue, void *value);
void* q2Dequeue (q2* queue);

#endif
