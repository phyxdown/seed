/* time.c */

#include <stdio.h>

#include "time.h"

const char*
MonthToString(Month m) {
	return months[m-1];
}

const char*
WeekdayToString(Weekday w) {
	return days[w];
}
