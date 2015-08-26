/* __TIME_H__ */
#ifndef __TIME_H__
#define __TIME_H__

#include <stdint.h>

typedef struct {
	int64_t sec;    /* sec gives the number of seconds eclapsed since 1-1-1 00:00:00 */
	int32_t nsec;   /* nanoseconds [0, 999999999] */
	int16_t offset; /* offset from UTC in minutes [-1439, 1439] */
} timestamp;

int After(timestamp *t, timestamp *u) {
	return t->sec > u->sec || t->sec == u->sec && t->nsec > u->nsec;
}

int Before(timestamp *t, timestamp *u) {
	return t->sec < u->sec || t->sec == u->sec && t->nsec < u->nsec;
}

int Equal(timestamp *t, timestamp *u) {
	return t->sec == u->sec && t->nsec == u->nsec;
}

typedef int Month;

const Month January = 1,
      February  = 2,
      March     = 3,
      April     = 4,
      May       = 5,
      June      = 6,
      July      = 7,
      August    = 8,
      September = 9,
      October   = 10,
      November  = 11,
      December  = 12;

const char* months[] = {
	"January",
	"February",
	"March",
	"April",
	"March",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
};

const char* MonthToString(Month m);

typedef int Weekday;

const Weekday Sunday = 0,
      Monday    = 1,
      Tuesday   = 2,
      Wednesday = 3,
      Thursday  = 4,
      Friday    = 5,
      Saturday  = 6;

const char* days[] = {
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday"
};

const int secondsPerMinute = 60,
      secondsPerHour = 3600,
      secondsPerDay = 86400,
      secondsPerWeek = 604800,
      daysPer400Years = 365*400 + 97,
      daysPer100Years = 365*100 + 24,
      daysPer4Years = 365*4 + 1;


const char* WeekdayToString(Weekday w);


const int64_t absoluteZeroYear = -292277022399;
const int internalYear = 1;
const unixYear = 1970;

//offsets to convert between internal and absolute or unix times
const int64_t absoluteToInternal = -9223371966579724288,
      internalToAbsolute = 9223371966579724288,
      unixToInternal = 62135596800,
      internalToUnix = -62135596800;

#endif/* __TIME_H__ */
