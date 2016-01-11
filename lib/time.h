#ifndef __SEED_TIME_H
#define __SEED_TIME_H

#include <stdint.h>

typedef struct seed_time seed_time;

struct seed_time {
	int64_t sec;
	int32_t nsec;
	int16_t offset;
};

seed_time* seed_time_now(int tz);
void seed_time_release(seed_time* t);

int seed_time_after(seed_time* t, seed_time* u);
int seed_time_before(seed_time* t, seed_time* u);
int seed_time_equal(seed_time* t, seed_time* u);

int32_t seed_time_hour(seed_time* t);
int32_t seed_time_minute(seed_time* t);
int32_t seed_time_second(seed_time* t);

/**
 * return the time (in us) elapsed from t2 to t1.
 * */
int64_t seed_time_since(seed_time* t1, seed_time* t2);

/**
 * return a 1991-01-30 12:00:00.990 like timestamp.
 *
 * USAGE
 *
 * 	seed_time* t = seed_time_now(8);
 * 	if (!!t) {
 * 		char* ts = seed_time_format(t);
 * 		if (!!ts) dosomething(ts);
 * 		free(ts);
 * 	}
 * 	free(t);
 * */
char* seed_time_format(seed_time* t);

/**
 * USAGE
 *
 * 	int32_t h;
 * 	seed_time* t = seed_time_now(8);
 * 	if (!!t) h = seed_time_hourage(t);
 * 	free(t);
 * 	dosomething(h);
 * */
int32_t seed_time_hourage(seed_time* t);
#endif
