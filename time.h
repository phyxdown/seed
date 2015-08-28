/* __TIME_H__ */
#ifndef __TIME_H__
#define __TIME_H__

#include <stdint.h>

typedef struct {
	int64_t sec;    /* sec gives the number of seconds eclapsed since 1-1-1 00:00:00 */
	int32_t nsec;   /* nanoseconds [0, 999999999] */
	int16_t offset;
} timestamp;

typedef timestamp* Timestamp;
Timestamp timeNow();

int timeAfter(Timestamp t, Timestamp u);
int timeBefore(Timestamp t, Timestamp u);
int timeEqual(Timestamp t, Timestamp u);

int32_t timeHour(Timestamp t);
int32_t timeMinute(Timestamp t);

typedef int Month;
typedef int Weekday;

char* timeFormat(Timestamp t);
#endif/* __TIME_H__ */
