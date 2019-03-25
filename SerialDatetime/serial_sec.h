#pragma once

#ifndef SERIAL_SEC_H
#define SERIAL_SEC_H

#define DEBUG_PRINT	// Comment out when "debug print" is not needed.

#define BASE_YEAR 2000
#define MAX_YEAR  2099

// Calendar Info
typedef struct tag_datetime {
	short year;
	char  mon;
	char  day;
	char  hour;
	char  min;
	char  sec;
} Datetime_t;

void SerSec_GetSerializedDatetimeSec(const Datetime_t * const p_datetime, unsigned long * const p_total_serialized_sec);
void SerSec_GetCalendarDatetime(const unsigned long total_serialized_sec, Datetime_t * const p_datetime);

#endif

