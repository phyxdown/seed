#include "concurrent.h"
#include "aggregator.h"

typedef struct _seed_aggregator _seed_aggregator;

struct _seed_aggregator {
	seed_aggregator_handler* handler;
	seed_concurrent_queue* queue;
};
