#ifndef __SEED_AGGREGATOR_H
#define __SEED_AGGREGATOR_H

typedef struct seed_aggregator seed_aggregator;

struct seed_aggregator {
	void (*add) (seed_aggregator* agg, void* args);
	void (*release) (seed_aggregator* agg);
};

typedef void seed_aggregator_handler(void* args);

seed_aggregator* seed_aggregator_create(seed_aggregator_handler* handler);

#endif
