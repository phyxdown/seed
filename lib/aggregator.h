#ifndef __SEED_AGGREGATOR_H
#define __SEED_AGGREGATOR_H
typedef struct seed_aggregator seed_aggregator;
struct seed_aggregator {
	int (*add) (seed_aggregator* aggregator, void* item);
	void (*release) (seed_aggregator* aggregator);
};
typedef int seed_aggregator_handler(void* item);
seed_aggregator* seed_aggregator_create(seed_aggregator_handler* handler);
#endif
