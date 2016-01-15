#include "macros.h"
#include <sys/time.h>
	/* strcut timeval
	 * struct timespec
	 * */
#include <stdlib.h>
	/* malloc
	 * free
	 * */

/* thread lib 11 */
#include <pthread.h>
#define Thread        pthread_t
#define thread_create pthread_create
#define thread_join   pthread_join
#define Mutex         pthread_mutex_t
#define mutex_init    pthread_mutex_init
#define mutex_lock    pthread_mutex_lock
#define mutex_unlock  pthread_mutex_unlock
#define Cond          pthread_cond_t
#define cond_init     pthread_cond_init
#define cond_wait     pthread_cond_timedwait
#define cond_signal   pthread_cond_signal

/* queue lib 2 */
#include "concurrent.h"
#define IQueue       seed_concurrent_queue
	/* ->enqueue      (IQueue, void*)
	 * ->batchDequeue (IQueue, void**, int)
	 * ->release      (IQueue)
	 * */
#define IQueueCreate seed_concurrent_queue_create

/* header 3 */
#include "aggregator.h"
#define IAggregator       seed_aggregator
	/* ->add     (IAggregator, void*)
	 * ->release (IAggregator)
	 * */
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
	Mutex        mutex;
	Cond         cond;
	volatile int isEmpty;
	volatile int status;
	void*        methods[];
};

static int add(IAggregator* aggregator, void* item) {
	Aggregator* a = itos(aggregator);
	if (a->status != WORKING) return 0;
	IQueue* q = a->queue;
	int r = q->enqueue(q, item);
	if (r == 1) {
		a->isEmpty = 0;
		cond_signal(&a->cond);
		return 1;
	} else return r;
}

static void release(IAggregator* aggregator) {
	Aggregator* a = itos(aggregator);
	a->status = CLOSING;
	thread_join(a->thread, NULL);
	a->queue->release(a->queue);
	a->status = CLOSED;
	free(a);
}

static void collect_and_handle(Aggregator* a) {
	IAggregator* m = (IAggregator*)a->methods;
	IQueue* q = a->queue;
	FHandler* h = a->handler;

	void *b[BUFFER_SIZE];
	size_t r = q->batchDequeue(q, b, BUFFER_SIZE);
	size_t i;
	for (i = 0; i < r; i++) {
		if (h(b[i]) < 0) m->add(m, b[i]);
	}
}

static void* collector(void* arg) {
	Aggregator* a = (Aggregator*)arg;
	while(a->status == WORKING) { 
		if (a->isEmpty) {
			struct timeval now;
			struct timespec outtime;
			outtime.tv_sec = now.tv_sec + 3;
			outtime.tv_nsec = now.tv_usec * 1000;
			mutex_lock(&a->mutex);
			cond_wait(&a->cond, &a->mutex, &outtime);
			mutex_unlock(&a->mutex);
		}
		collect_and_handle(a);
		a->isEmpty = 1;
	}
	collect_and_handle(a);
	return NULL;
}

IAggregator* IAggregatorCreate(FHandler handler) {
	Aggregator* a;
	if (!(a = malloc(sizeof(Aggregator) + sizeof(IAggregator)))) return NULL;
	if (mutex_init(&a->mutex, NULL) < 0 || cond_init(&a->cond, NULL) < 0) {
		free(a);
		return NULL;
	}
	a->handler  = handler;

	if (!(a->queue = IQueueCreate(BUFFER_SIZE))) {
		free(a);
		return NULL;
	}

	IAggregator* aggregator = (IAggregator*)a->methods;
	aggregator->add     = &add;
	aggregator->release = &release;

	if ((thread_create(&a->thread, NULL, &collector, a)) < 0) {
		a->queue->release(a->queue);
		free(a);
		return NULL;
	}
	a->isEmpty = 1;
	a->status = WORKING;
	return aggregator;
}

/* thread lib 11 */
#undef Thread
#undef thread_create
#undef thread_join
#undef Mutex
#undef mutex_init
#undef mutex_lock
#undef mutex_unlock
#undef Cond
#undef cond_init
#undef cond_wait
#undef cond_signal

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


#ifdef  TEST_SEED_AGGREGATOR
#include <stdio.h>
#include <pthread.h>

static int tsa1_count = 0;

static int tsa1_handler(void* item) {
	int* n = (int*)item;
	tsa1_count += *n;
	free(item);
	return 0;
}

static void* tsa1_func(void* arg) {
	seed_aggregator* agg = (seed_aggregator*)arg;
	int n = 10000;
	while (n--) {
		int* i = malloc(sizeof(int));
		*i = 1;
		if (agg->add(agg, i) == 0) free(i);
	}
	return NULL;
}
/**
 * Respectvely, 2 threads loop to add 1 to tsa1_count 10000 times.
 * The result is expected to be 20000.
 * */
static void test_seed_aggregator_plus_to_20000() {
	seed_aggregator* agg = seed_aggregator_create(&tsa1_handler);
	if (agg != NULL) {
		pthread_t t1, t2;
		pthread_create(&t1, NULL, &tsa1_func, agg); pthread_create(&t2, NULL, &tsa1_func, agg);
		pthread_join(t1, NULL);                     pthread_join(t2, NULL);
		agg->release(agg);
	} else printf("aggregator == NULL\n");
	if (tsa1_count == 20000) printf("plus to 20000: test pass\n");
}

int main() {
	test_seed_aggregator_plus_to_20000();
	return 0;
}
#endif
