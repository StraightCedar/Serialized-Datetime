#define _CRT_SECURE_NO_WARNINGS

// Referenced URL
// http://lse6.u.e.kaiyodai.ac.jp/lecture/lbp/m/tb09b2.htm

#include "serial_sec.h"
#ifdef DEBUG_PRINT
	#include <stdio.h>
#endif

#define BASE_YEAR 2000
#define MAX_YEAR  2099

// Days of Month
static const char g_monthday_arr[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Get serialized datetime in seconds.
static void getSerializedDayFromCalendar(const Datetime_t * const p_datetime, long * const p_serialized_day);

// Get calendar datetime.
static void getSerializedDayFromSerializedSec(long long ser_sec, long * const p_ser_day, long * const p_remaining_sec);
static void getYear(const long ymd_ser_day, short * const p_yyyy_year, short * const p_md_ser_day, int * const p_leap);
static void getMonthAndDay(const short md_ser_day, const int leap, char * const p_mm_month, char * const p_dd_day);
static void getHourMinSec(const long hms_ser_sec, char * const p_hh_hour, char * const p_mm_min, char * const p_ss_sec);


// =======================================================
// Get serialized datetime in seconds.
// =======================================================

void SerSec_GetSerializedDatetimeSec(const Datetime_t * const p_datetime, unsigned long * const p_total_serialized_sec) {
	long          serialized_day;		// Serialized day of Year-Month-Day
	unsigned long ymd_serialized_sec;	// Serialized sec of Year-Month-Day    : MAX = around 315,360,000
	long          hms_serialized_sec;	// Serialized sec of Hour-Minute-Second: MAX =             86,400 (long:-2,147,483,648`2,147,483,647)

	// Get serialized day from year, month and day.
	getSerializedDayFromCalendar(p_datetime, &serialized_day);
	#ifdef DEBUG_PRINT
		printf("    Serialized day of Year-Month-Day = %ld\n", serialized_day);
	#endif

	// Get serialized second of serialized day of Year-Month-Dat.
	ymd_serialized_sec = (unsigned long)serialized_day * 86400UL;
	#ifdef DEBUG_PRINT
		printf("    Serialized second of Year-Month-Day = %lu\n", ymd_serialized_sec);
	#endif

	// Get serialized second of Hour-Minute-Second.
	hms_serialized_sec = (long)p_datetime->hour * 3600L + (long)p_datetime->min * 60L + (long)p_datetime->sec;
	#ifdef DEBUG_PRINT
		printf("    Serialized second of Hour-Minute-Second = %ld\n", hms_serialized_sec);
	#endif

	*p_total_serialized_sec = ymd_serialized_sec + (unsigned long)hms_serialized_sec;
	#ifdef DEBUG_PRINT
		printf("    Total serialized second = %lu\n", *p_total_serialized_sec);
	#endif
}

static void getSerializedDayFromCalendar(const Datetime_t * const p_datetime, long * const p_serialized_day) {
	long  day2, day3;
	short loop_counter;
	int   leap;

	day2 = 0;
	for (loop_counter = BASE_YEAR; loop_counter <= p_datetime->year - 1; loop_counter++) {
		day2 += 365;
		if ((!(loop_counter % 4) && (loop_counter % 100)) || !(loop_counter % 400)) {
			leap = 1;
		}
		else {
			leap = 0;
		}
		day2 += (long)leap;
	}

	day3 = 0;
	if ((!(p_datetime->year % 4) && (p_datetime->year % 100)) || !(p_datetime->year % 400)) {
		leap = 1;
	}
	else {
		leap = 0;
	}
	for (loop_counter = 1; loop_counter <= (short)p_datetime->mon - 1; loop_counter++) {
		day3 += (long)g_monthday_arr[loop_counter];
		if (loop_counter == 2) day3 += (long)leap;
	}
	day3 += (long)p_datetime->day;

	*p_serialized_day = day2 + day3 - 1L;
}


// =======================================================
// Get calendar datetime.
// =======================================================

void SerSec_GetCalendarDatetime(const unsigned long total_serialized_sec, Datetime_t * const p_datetime) {
	long	ymd_ser_day;	// Serialized day of Year-Month-Day
	long	hms_ser_sec;	// Serialized sec of Hour-Minute-Second
	short	md_ser_day;		// Serialized day of Month-Day (without year)
	int     leap;			// Leap year flag: 0 = normal year, 1 = leap year
	
	// Get serialized day of Year-Month-Day.
	getSerializedDayFromSerializedSec(total_serialized_sec, &ymd_ser_day, &hms_ser_sec);
	#ifdef DEBUG_PRINT
		printf("    Serialized day of Year-Month-Day     = %ld\n", ymd_ser_day);
		printf("    Serialized sec of Hour-Minute-Second = %ld\n", hms_ser_sec);
	#endif

	// Get A.D. year.
	getYear(ymd_ser_day, &p_datetime->year, &md_ser_day, &leap);
	#ifdef DEBUG_PRINT
		printf("    A.D. year                                  = %d\n", p_datetime->year);
		printf("    Serialized day of Month-Day (without year) = %d\n", md_ser_day);
		printf("    Leap year flag                             = %d\n", leap);
	#endif

	// Get month and day.
	getMonthAndDay(md_ser_day, leap, &p_datetime->mon, &p_datetime->day);
	#ifdef DEBUG_PRINT
		printf("    Month = %02d\n", p_datetime->mon);
		printf("    Day   = %02d\n", p_datetime->day);
	#endif

	// Get hour, minute and second.
	getHourMinSec(hms_ser_sec, &p_datetime->hour, &p_datetime->min, &p_datetime->sec);
	#ifdef DEBUG_PRINT
		printf("    Hour   = %02d\n", p_datetime->hour);
		printf("    Minute = %02d\n", p_datetime->min);
		printf("    Second = %02d\n", p_datetime->sec);
	#endif

	#ifdef DEBUG_PRINT
		// Show final output
		printf("    dd/mm/yyyy HH:MM:SS = %02d/%02d/%04d %02d:%02d:%02d\n",
			p_datetime->day,  p_datetime->mon, p_datetime->year,
			p_datetime->hour, p_datetime->min, p_datetime->sec);
	#endif
}

static void getSerializedDayFromSerializedSec(long long ser_sec, long * const p_ser_day, long * const p_remaining_sec)
{
	*p_ser_day       = (long)(ser_sec / 86400LL);
	*p_remaining_sec = (long)(ser_sec % 86400LL);
}

static void getYear(const long ymd_ser_day, short * const p_yyyy_year, short * const p_md_ser_day, int * const p_leap)
{
	long  year_day     = 0L;
	long  pre_year_day = 0L;
	short year_counter;
	int   leap;

	for (year_counter = BASE_YEAR; year_counter <= MAX_YEAR; year_counter++) {
		year_day += 365;
		if ((!(year_counter % 4) && (year_counter % 100)) || !(year_counter % 400)) {
			leap = 1;
		}
		else {
			leap = 0;
		}
		year_day += (long)leap;
		if (ymd_ser_day <= year_day) break;
		pre_year_day = year_day;
	}
	*p_yyyy_year  = year_counter;
	*p_md_ser_day = (short)(ymd_ser_day - pre_year_day) + 1;
	*p_leap       = leap;
}

static void getMonthAndDay(const short md_ser_day, const int leap, char * const p_mm_month, char * const p_dd_day)
{
	char  month_counter;
	short month_day = 0;
	short pre_month_day = 0;

	for (month_counter = 1; 13 > month_counter; month_counter++) {
		month_day += (short)g_monthday_arr[month_counter];
		if (2 == month_day) month_day += (short)leap;
		if (md_ser_day <= month_day) break;
		pre_month_day = month_day;
	}
	*p_mm_month = month_counter;
	*p_dd_day   = (char)(md_ser_day - pre_month_day);
}

static void getHourMinSec(const long hms_ser_sec, char * const p_hh_hour, char * const p_mm_min, char * const p_ss_sec)
{
	short min_ser_sec = (short)(hms_ser_sec % 3600L);

	*p_hh_hour = (char)(hms_ser_sec / 3600L);
	*p_mm_min  = (char)(min_ser_sec / 60);
	*p_ss_sec  = (char)(min_ser_sec % 60);
}
