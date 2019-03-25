#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "serial_sec.h"

int main(void) {
	int func_num;
	unsigned long total_serialized_sec;
	Datetime_t datetime;
	int day, mon, year, hour, min, sec;

	printf("Base year = %d\n", BASE_YEAR);
	printf("Max  year = %d\n", MAX_YEAR);
	printf("1 = Calendar datetime to serialized sec.\n");
	printf("2 = Serialized sec to calendar datetime.\n");
	printf("Other = Quit.\n");
	printf("> ");
	scanf("%d", &func_num);

	switch (func_num) {
	case 1:	// Calendar datetime to serialized sec
		printf("Input calendar datetime with space, dd mm yyyy HH MM SS: ");
		scanf("%d%d%d%d%d%d", &day, &mon, &year, &hour, &min, &sec);
		datetime.day  = (char)day;
		datetime.mon  = (char)mon;
		datetime.year = (short)year;
		datetime.hour = (char)hour;
		datetime.min  = (char)min;
		datetime.sec  = (char)sec;
		SerSec_GetSerializedDatetimeSec(&datetime, &total_serialized_sec);
		printf("Result = %lu [sec]\n", total_serialized_sec);

		SerSec_GetCalendarDatetime(total_serialized_sec, &datetime);
		printf("Reconstructed datetime dd/mm/yyyy HH:MM:SS = %02d/%02d/%04d %02d:%02d:%02d\n",
			datetime.day, datetime.mon, datetime.year,
			datetime.hour, datetime.min, datetime.sec);
		break;
	case 2: // Serialized sec to calendar datetime
		printf("Input serialized second:");
		scanf("%lu", &total_serialized_sec);
		SerSec_GetCalendarDatetime(total_serialized_sec, &datetime);
		printf("Result = dd/mm/yyyy HH:MM:SS = %02d/%02d/%04d %02d:%02d:%02d\n",
			datetime.day,  datetime.mon, datetime.year,
			datetime.hour, datetime.min, datetime.sec);

		SerSec_GetSerializedDatetimeSec(&datetime, &total_serialized_sec);
		printf("Reconstructed serialized second = %lu [sec]\n", total_serialized_sec);
		break;
	default:	// No operation
		break;
	}

	return 0;
}