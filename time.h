#ifndef __SEED_TIME_H
#define __SEED_TIME_H

#define __SEED_TIME_API extern

#include <stdint.h>

typedef struct {
	int64_t sec;    /* sec gives the number of seconds eclapsed since 1-1-1 00:00:00 */
	int32_t nsec;   /* nanoseconds [0, 999999999] */
	int16_t offset;
} __seed_timestamp;

typedef __seed_timestamp* __seed_timestamp_t;

typedef __seed_timestamp_t Time;

__SEED_TIME_API __seed_timestamp_t timeNow();

__SEED_TIME_API int timeAfter(__seed_timestamp_t t, __seed_timestamp_t u);
__SEED_TIME_API int timeBefore(__seed_timestamp_t t, __seed_timestamp_t u);
__SEED_TIME_API int timeEqual(__seed_timestamp_t t, __seed_timestamp_t u);

__SEED_TIME_API int32_t timeHour(__seed_timestamp_t t);
__SEED_TIME_API int32_t timeMinute(__seed_timestamp_t t);

__SEED_TIME_API char* timeFormat(__seed_timestamp_t t);
#endif
