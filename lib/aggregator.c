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
#define FReleaser         seed_aggregator_releaser

/* internal %d */
#define Aggregator _seed_aggregator
#define itos(P) \
	((Aggregator*)(((char*)(P)) - sizeof(Aggregator)))

typedef struct Aggregator Aggregator;

struct Aggregator {
	FHandler*  handler;
	FReleaser* releaser;
	IQueue*    queue;
	Thread     thread;
	void*      methods[];
};

static void add(IAggregator* aggregator, void* item) {
	Aggregator* a = itos(aggregator);
	IQueue* q = a->queue;
	int r = q->enqueue(q, item);
	if (r < 0);
	if (r == 0) a->releaser(item);
	if (r == 1);
}
static void release(IAggregator* agg) {}

static void* collector(void* arg) {
	Aggregator* agg      = (Aggregator*)arg;
	IAggregator* methods = (IAggregator*)agg->methods;
	IQueue* queue        = agg->queue;
	FHandler* handler    = agg->handler;
	FReleaser* releaser  = agg->releaser;

	while(1) {
		void *b[1000];
		int r = queue->batchDequeue(queue, b, 1000);
		if (r < 0);
		if (r == 0) continue;
		int i;
		int seconds = 0;
		for (i = 0; i < r; i++) {
			if (handler(b[i]) < 0) { 
				methods->add(methods, b[i]);
				seconds = seconds + (seconds < 4);
				sleep(seconds);
			} else releaser(b[i]);
		}
	}
	return NULL;
}

IAggregator* IAggregatorCreate(FHandler handler, FReleaser releaser, int bufferSize) {
	Aggregator* agg;
	if (!(agg = malloc(sizeof(Aggregator) + sizeof(IAggregator)))) return NULL;
	if (!(agg->queue = IQueueCreate(bufferSize))) {
		free(agg);
		return NULL;
	}
	agg->handler  = handler;
	agg->releaser = releaser;

	IAggregator* m = (IAggregator*)agg->methods;
	m->add     = &add;
	m->release = &release;

	if ((thread_create(&agg->thread, NULL, &collector, agg)) < 0) {
		agg->queue->release(agg->queue);
		free(agg);
		return NULL;
	}
	return m;
}
