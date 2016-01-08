/* pthread lib %d */
#include <pthread.h>
#define Thread        pthread_t
#define thread_create pthread_create
#define thread_join   pthread_join

/* queue lib %d */
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

/* internal %d */
#define Aggregator _seed_aggregator
#define itos(P) \
	((Aggregator*)(((char*)(P)) - sizeof(Aggregator)))
#define WORKING 2
#define CLOSING 1
#define CLOSED  0

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

static void release(IAggregator* agg) {}

static void collect_and_handle(Aggregator* agg) {
	Aggregator* a = agg;
	IAggregator* m = (IAggregator*)a->methods;
	IQueue* q = a->queue;
	FHandler* h = a->handler;
	void *b[1000];
	int r = q->batchDequeue(q, b, 1000);
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

IAggregator* IAggregatorCreate(FHandler handler, int bufferSize) {
	Aggregator* agg;
	if (!(agg = malloc(sizeof(Aggregator) + sizeof(IAggregator)))) return NULL;
	if (!(agg->queue = IQueueCreate(bufferSize))) {
		free(agg);
		return NULL;
	}
	agg->handler  = handler;

	IAggregator* m = (IAggregator*)agg->methods;
	m->add     = &add;
	m->release = &release;

	if ((thread_create(&agg->thread, NULL, &collector, agg)) < 0) {
		agg->queue->release(agg->queue);
		free(agg);
		return NULL;
	}
	agg->status = WORKING;
	return m;
}
