/* thread lib 3 */
#include <unistd.h>
	/* sleep
	 * */

#include <pthread.h>
#define Thread        pthread_t
#define thread_create pthread_create
#define thread_join   pthread_join

/* queue lib 2 */
#include "concurrent.h"
#define IQueueCreate seed_concurrent_queue_create
#define IQueue       seed_concurrent_queue
	/* ->enqueue      (IQueue, void*)
	 * ->batchDequeue (IQueue, void**, int)
	 * ->release      (IQueue)
	 * */

/* header 3 */
#include "aggregator.h"
#define IAggregator       seed_aggregator
#define IAggregatorCreate seed_aggregator_create
#define FHandler          seed_aggregator_handler

/* internal 6 */
#define Aggregator _seed_aggregator
#define itos(P) \
	((Aggregator*)(((char*)(P)) - sizeof(Aggregator)))

#define WORKING 2
#define CLOSING 1
#define CLOSED  0

#define BUFFER_SIZE 1000

typedef struct Aggregator Aggregator;

struct Aggregator {
	FHandler*    handler;
	IQueue*      queue;
	Thread       thread;
	volatile int status;
	void*        methods[];
};

static int add(IAggregator* aggregator, void* item) {
	Aggregator* a = itos(aggregator);
	if (a->status != WORKING) return 0;
	IQueue* q = a->queue;
	return q->enqueue(q, item);
}

static void release(IAggregator* aggregator) {
	Aggregator* a = itos(aggregator);
	a->status = CLOSING;
	thread_join(a->thread, NULL);
	a->queue->release(a->queue);
	a->status = CLOSED;
	free(a);
}

static void collect_and_handle(Aggregator* agg) {
	Aggregator* a = agg;
	IAggregator* m = (IAggregator*)a->methods;
	IQueue* q = a->queue;
	FHandler* h = a->handler;
	void *b[BUFFER_SIZE];
	int r = q->batchDequeue(q, b, BUFFER_SIZE);
	if (r < 0); if (r == 0);
	int i;
	int s = 0;
	for (i = 0; i < r; i++) {
		if (h(b[i]) < 0) { 
			m->add(m, b[i]);
			s = s + (s < 4);
			sleep(s);
		}
	}
}

static void* collector(void* arg) {
	Aggregator* a = (Aggregator*)arg;
	while(a->status == WORKING) collect_and_handle(a);
	collect_and_handle(a);
	return NULL;
}

IAggregator* IAggregatorCreate(FHandler handler) {
	Aggregator* a;
	if (!(a = malloc(sizeof(Aggregator) + sizeof(IAggregator)))) return NULL;
	if (!(a->queue = IQueueCreate(BUFFER_SIZE))) {
		free(a);
		return NULL;
	}
	a->handler  = handler;

	IAggregator* m = (IAggregator*)a->methods;
	m->add     = &add;
	m->release = &release;

	if ((thread_create(&a->thread, NULL, &collector, a)) < 0) {
		a->queue->release(a->queue);
		free(a);
		return NULL;
	}
	a->status = WORKING;
	return m;
}

/* thread lib 3 */
#undef Thread
#undef thread_create
#undef thread_join

/* queue lib 2 */
#undef IQueue
#undef IQueueCreate

/* header 3 */
#undef IAggregator
#undef IAggregatorCreate
#undef FHandler

/* internal 6 */
#undef Aggregator
#undef itos
#undef WORKING
#undef CLOSING
#undef CLOSED
#undef BUFFER_SIZE
