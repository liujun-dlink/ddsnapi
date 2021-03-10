// libCLI
#ifndef LIB_CLI_H
#define LIB_CLI_H

#include <stdio.h>

char* getSystemInfo();

int setAdminPassword(char* password);

//int save();

char* getDeviceName();

int setDeviceName(char* strDeviceName);

char* getPerformance();

int getWebAccessPort(void);

int setWebAccessPort(int nPortNumber);

int getNginxAccessPort();

int setNginxAccessPort(int nPortNumber);

char* getNvrVersion();

//int setMACAddress(char* macAddress);

#endif
