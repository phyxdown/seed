#ifndef __SEED_AGGREGATOR_H
#define __SEED_AGGREGATOR_H

typedef struct seed_aggregator seed_aggregator;

struct seed_aggregator {
	void (*add) (seed_aggregator* agg, void* args);
	void (*release) (seed_aggregator* agg);
};

typedef int seed_aggregator_handler(void* args);
typedef void seed_aggregator_releaser(void* args);

seed_aggregator* seed_aggregator_create(seed_aggregator_handler* handler, 
		seed_aggregator_releaser* releaser, int bufferSize);

#endif
