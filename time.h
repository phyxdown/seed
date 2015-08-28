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
Timestamp Now();

int After(Timestamp t, Timestamp u);
int Before(Timestamp t, Timestamp u);
int Equal(Timestamp t, Timestamp u);

int32_t Hour(Timestamp t);
int32_t Minute(Timestamp t);

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

typedef int Weekday;
const Weekday Sunday = 0,
      Monday    = 1,
      Tuesday   = 2,
      Wednesday = 3,
      Thursday  = 4,
      Friday    = 5,
      Saturday  = 6;
#endif/* __TIME_H__ */
