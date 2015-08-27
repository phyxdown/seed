/* time.c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "time.h"

timestamp*
Now() {
	timestamp* t;
	t = malloc(sizeof(*t));
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	t->sec = tv.tv_sec;
	t->nsec = tv.tv_usec * 1000;
	t->offset = tz.tz_minuteswest;
	return t;
}

int After(timestamp *t, timestamp *u) { return t->sec > u->sec || t->sec == u->sec && t->nsec > u->nsec; }
int Before(timestamp *t, timestamp *u) { return t->sec < u->sec || t->sec == u->sec && t->nsec < u->nsec; }
int Equal(timestamp *t, timestamp *u) { return t->sec == u->sec && t->nsec == u->nsec; }

const int secondsPerMinute = 60,
      secondsPerHour = 3600,
      secondsPerDay = 86400,
      secondsPerWeek = 604800,
      daysPer400Years = 365*400 + 97,
      daysPer100Years = 365*100 + 24,
      daysPer4Years = 365*4 + 1;

const int64_t absoluteZeroYear = -292277022399;
const int internalYear = 1;
const unixYear = 1970;

//offsets to convert between internal and absolute or unix times
const int64_t absoluteToInternal = -9223371966579724288,
      internalToAbsolute = 9223371966579724288,
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

static int64_t absSeconds(timestamp* t) { return t->sec + unixToInternal + internalToAbsolute; }

static int isLeap(int32_t year) { return year%4 == 0 && (year%100 !=0 || year&400 == 0); }

static ymd* absDate(timestamp* t, int full) {
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
	ymd->year = year = (int32_t)(y + absoluteZeroYear);
	ymd->month = 0;
	ymd->day = 0;
	if (!full) return ymd;

	yday = (int32_t)d;

	day = yday;

	if (isLeap(year)) {
		if (day == 31 + 29 -1) {
			ymd->month = February;
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

int main() {
	timestamp* t = Now();
	ymd* ymd = absDate(t, 1);
	printf("%ld\n%ld\n%ld\n", ymd->year, ymd->month, ymd->day);
}