#ifndef __SEED_TIME_H
#define __SEED_TIME_H

#include <stdint.h>

typedef struct {
	int64_t sec;    /* sec gives the number of seconds eclapsed since 1-1-1 00:00:00 */
	int32_t nsec;   /* nanoseconds [0, 999999999] */
	int16_t offset;
} timestamp;

typedef timestamp* Time;

timestamp* timeNow();

int timeAfter(timestamp* t, timestamp* u);
int timeBefore(timestamp* t, timestamp* u);
int timeEqual(timestamp* t, timestamp* u);

int32_t timeHour(timestamp* t);
int32_t timeMinute(timestamp* t);

/**
 * return a 1991-01-30 12:00:00.990 like timestamp.
 *
 * USAGE
 *
 * 	Time tm_now = timeNow();
 * 	char* ts_now = timeFormat(tm_now);
 *
 * 	dosomething(ts_now);
 *
 * 	free(ts_now);
 * 	free(tm_now);
 * */
char* timeFormat(timestamp* t);
#endif
