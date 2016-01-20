#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "time.h"

static void time_now_init(seed_time* t, int16_t offset) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	t->sec = tv.tv_sec;
	t->nsec = tv.tv_usec * 1000;
	t->offset = offset;
}

seed_time* seed_time_now(int tz) {
	seed_time* t;
	if ((t = malloc(sizeof(*t))) == NULL) return NULL;
	time_now_init(t, -tz * 60);
	return t;
}

void seed_time_release(seed_time* t) {
	if (t != NULL) free(t);
}

int seed_time_after(seed_time* t, seed_time* u) { return t->sec > u->sec || ((t->sec == u->sec) && t->nsec > u->nsec); }
int seed_time_before(seed_time* t, seed_time* u) { return t->sec < u->sec || ((t->sec == u->sec) && t->nsec < u->nsec); }
int seed_time_equal(seed_time* t, seed_time* u) { return t->sec == u->sec && t->nsec == u->nsec; }

typedef struct ymd {
	int32_t year;
	int32_t month;
	int32_t day;
} ymd;

static const int secondsPerMinute = 60,
	     secondsPerHour = 3600, 
	     secondsPerDay = 86400, 
//	     secondsPerWeek = 604800, 
	     daysPer400Years = 365*400 + 97, 
	     daysPer100Years = 365*100 + 24, 
	     daysPer4Years = 365*4 + 1;

static const int64_t absoluteZeroYear = 1;
//static const int internalYear = 1;
//static const int unixYear = 1970;

static const int64_t unixToInternal = 62135596800;

//static const int64_t absoluteToInternal = 0,
//	     internalToAbsolute = 0, 
//	     internalToUnix = -62135596800;

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

static int64_t absolute_seconds(seed_time* t) { return t->sec + unixToInternal + absoluteZeroYear - t->offset*60; }

static int is_leap(int32_t year) { return year%4 == 0 && (year%100 !=0 || year%400 == 0); }

static ymd* absDate(seed_time* t, int full) {
	int64_t abs = absolute_seconds(t);
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

	if (is_leap(year)) {
		if (day == 31 + 29 -1) {
			ymd->month = 2;
			ymd->day = 29;
			return ymd;
		} else if (day > 31 + 29 -1) {
			day --;
		}
	}

	month = (int)(day/31);
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

int32_t seed_time_hour(seed_time* t) { return (absolute_seconds(t)%secondsPerDay)/secondsPerHour; }
int32_t seed_time_minute(seed_time* t) { return (absolute_seconds(t)%secondsPerHour)/secondsPerMinute; }
int32_t seed_time_second(seed_time* t) { return absolute_seconds(t)%secondsPerMinute; }

int64_t seed_time_since(seed_time* t1, seed_time* t2) { 
	return (absolute_seconds(t1)*1000000 + t1->nsec/1000) - (absolute_seconds(t2)*1000000 + t2->nsec/1000);
}

char* seed_time_format(seed_time* t) {
	char* stamp;
	if ((stamp = malloc(23+1)) == NULL) return NULL;
	ymd* ymd;
	if ((ymd = absDate(t, 1)) == NULL) { 
		free(stamp);
		return NULL;
	}
	sprintf(stamp, "%04d-%02d-%02d %02d:%02d:%02d.%03d", ymd->year, ymd->month, ymd->day,
			seed_time_hour(t), seed_time_minute(t), seed_time_second(t), t->nsec/1000000);
	free(ymd);
	*(stamp+23) = '\0';
	return stamp;
}

int32_t seed_time_hourage(seed_time* t) {
	return t->sec/secondsPerHour;
}

int seed_time_try_update_by_minimal_interval(seed_time* t, int64_t minimal_interval) {
	seed_time n;
	time_now_init(&n, t->offset);
	int64_t interval = seed_time_since(&n, t);
	if (interval >= minimal_interval) {
		*t = n;
		return 1;
	} else return 0;
}

#ifdef TEST_SEED_TIME
static void test_seed_time_now() {
  	int32_t h;
  	seed_time* t = seed_time_now(8);
  	if (!!t) h = seed_time_hourage(t);
  	free(t);
  	printf("%d\n", h);
}

static void test_seed_time_hourage() {
  	seed_time* t = seed_time_now(8);
  	if (!!t) {
  		char* ts = seed_time_format(t);
  		if (!!ts) printf("%s\n", ts);
  		free(ts);
  	}
  	free(t);
}

int main() {
	test_seed_time_now();
	test_seed_time_hourage();
	return 0;
}
#endif
