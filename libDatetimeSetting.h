#ifndef LIBDATETIMESETTING_H
#define LIBDATETIMESETTING_H


int setDatetime(int isPermanent, int enableNTPServer, char* strServerAddress, char* strDatetime);
char* getDatetime();
int setTimezone(int isPermanent, char* strTimezone);
char* getTimezone();
int setDaylightSaving(int isdst, int offsetSeconds, int startMonth, int startWeek, int startDay, char* startTime,
	int endMonth, int endWeek, int endDay, char* endTime);
char* getDaylightSaving();
int setCityDatetime(char* strCityId, int isDstEnabled, int offsetSeconds, int startMonth, int startWeek, int startDay, char* startTime,
	int endMonth, int endWeek, int endDay, char* endTime);
char* getCityDatetime();
char* getCityDst(char* strCityId);


#endif
