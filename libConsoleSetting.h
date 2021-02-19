// libConsoleSetting
#ifndef LIB_CONSOLE_SETTING_H
#define LIB_CONSOLE_SETTING_H

#include <stdio.h>

int setConsoleSetting(int enableConsole, char* strProtocol,	int nTimeoutSec);

char* getConsoleSetting(void);

#endif
