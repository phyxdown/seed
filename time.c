/* time.c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "time.h"

typedef int Month;
typedef int Weekday;

__seed_timestamp*
timeNow() {
	__seed_timestamp* t;
	t = malloc(sizeof(*t));
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	t->sec = tv.tv_sec;
	t->nsec = tv.tv_usec * 1000;
	t->offset = tz.tz_minuteswest;
	return t;
}

int timeAfter(__seed_timestamp* t, __seed_timestamp* u) { return t->sec > u->sec || ((t->sec == u->sec) && t->nsec > u->nsec); }
int timeBefore(__seed_timestamp* t, __seed_timestamp* u) { return t->sec < u->sec || ((t->sec == u->sec) && t->nsec < u->nsec); }
int timeEqual(__seed_timestamp* t, __seed_timestamp* u) { return t->sec == u->sec && t->nsec == u->nsec; }

typedef struct {
	int32_t year;
	int32_t month;
	int32_t day;
} ymd;

const int secondsPerMinute = 60,
      secondsPerHour = 3600,
      secondsPerDay = 86400,
      secondsPerWeek = 604800,
      daysPer400Years = 365*400 + 97,
      daysPer100Years = 365*100 + 24,
      daysPer4Years = 365*4 + 1;

const int64_t absoluteZeroYear = 1;
const int internalYear = 1;
const int unixYear = 1970;

const int64_t absoluteToInternal = 0,
      internalToAbsolute = 0,
                           
      unixToInternal = 62135596800,
      internalToUnix = -62135596800;

static int32_t daysBefore[] = {
	0,
	32,
	31 + 28,
	31 + 28 + 31,
	31 + 28 + 31 + 30,
	31 + 28 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30,
	31 + 28 + 31 + 30 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31
};

static int64_t absSeconds(__seed_timestamp* t) { return t->sec + unixToInternal + absoluteZeroYear - t->offset*60; }

static int isLeap(int32_t year) { return year%4 == 0 && (year%100 !=0 || year%400 == 0); }

static ymd* absDate(__seed_timestamp* t, int full) {
	int64_t abs = absSeconds(t);
	int64_t d, n, y;

	ymd* ymd;
	if ((ymd = malloc(sizeof(*ymd))) == NULL) return NULL;
	
	d = abs/secondsPerDay;

	n = d/daysPer400Years;
	y = 400 * n;
	d -= daysPer400Years * n;

	n = d/daysPer100Years;
	n -= n>>2;
	y += 100*n;
	d -= daysPer100Years*n;

	n = d/daysPer4Years;
	y += 4*n;
	d -= daysPer4Years*n;

	n = d/365;
	n -= n>>2;
	y += n;
	d -= 365*n;

	int32_t year, month, day, yday, end, begin;
	ymd->year = year = (int32_t)(y+absoluteZeroYear); ymd->month = 0; ymd->day = 0; if (!full) return ymd; 
	yday = (int32_t)d;

	day = yday;

	if (isLeap(year)) {
		if (day == 31 + 29 -1) {
			ymd->month = 2;
			ymd->day = 29;
			return ymd;
		} else if (day > 31 + 29 -1) {
			day --;
		}
	}

	month = (Month)(day/31);
	end = daysBefore[month+1];
	if (day >= end) {
		month++;
		begin = end;
	} else {
		begin = daysBefore[month];
	}

	ymd->month = month + 1;
	ymd->day = day - begin + 1;
	return ymd;
}

int32_t timeHour(__seed_timestamp* t) { return (absSeconds(t)%secondsPerDay)/secondsPerHour; }
int32_t timeMinute(__seed_timestamp* t) { return (absSeconds(t)%secondsPerHour)/secondsPerMinute; }
int32_t timeSecond(__seed_timestamp* t) { return absSeconds(t)%secondsPerMinute; }

char* timeFormat(__seed_timestamp* t) {
	char* stamp;
	if ((stamp = malloc(23+1)) == NULL) return NULL;
	ymd* ymd = absDate(t, 1);
	sprintf(stamp, "%04d-%02d-%02d %02d:%02d:%02d.%03d", ymd->year, ymd->month, ymd->day, timeHour(t), timeMinute(t), timeSecond(t), t->nsec/1000000);
	free(ymd);
	return stamp;
}
