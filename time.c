/* time.c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "time.h"

Timestamp
Now() {
	Timestamp t;
	t = malloc(sizeof(*t));
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	t->sec = tv.tv_sec;
	t->nsec = tv.tv_usec * 1000;
	t->offset = tz.tz_minuteswest;
	return t;
}

int After(Timestamp t, Timestamp u) { return t->sec > u->sec || t->sec == u->sec && t->nsec > u->nsec; }
int Before(Timestamp t, Timestamp u) { return t->sec < u->sec || t->sec == u->sec && t->nsec < u->nsec; }
int Equal(Timestamp t, Timestamp u) { return t->sec == u->sec && t->nsec == u->nsec; }

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
const unixYear = 1970;

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

static int64_t absSeconds(Timestamp t) { return t->sec + unixToInternal + absoluteZeroYear - t->offset*60; }

static int isLeap(int32_t year) { return year%4 == 0 && (year%100 !=0 || year&400 == 0); }

static ymd* absDate(Timestamp t, int full) {
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

int32_t Hour(Timestamp t) { return (absSeconds(t)%secondsPerDay)/secondsPerHour; }
int32_t Minute(Timestamp t) { return (absSeconds(t)%secondsPerHour)/secondsPerMinute; }
int32_t Second(Timestamp t) { return absSeconds(t)%secondsPerMinute; }

char* Format(Timestamp t) {
	char* stamp;
	if ((stamp = malloc(23+1)) == NULL) return NULL;
	ymd* ymd = absDate(t, 1);
	sprintf(stamp, "%04d-%02d-%02d %02d:%02d:%02d.%03d", ymd->year, ymd->month, ymd->day, Hour(t), Minute(t), Second(t), 0);
	free(ymd);
	return stamp;
}

/* Example
int main() {
	Timestamp t = Now();
	char* stamp;
	if ((stamp = Format(t)) == NULL) 
		return 0;
	else
		free(t);
	printf("%s\n", stamp);
	free(stamp);
	return 0;
} */
