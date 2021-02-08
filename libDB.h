#ifndef LIBDB_H
#define LIBDB_H

char* getSSOStatus();
int getNCStatus();
char* getDDPv5ClientVersion();
int setNCStatus(int nStatus);
int setSSOStatus(char* strStatus);
int setDDPv5ClientVersion(char* strVersion);
int startFOTAScript();
int setFirmwareReleaseNote(char* strReleaseNote);
int setFirmwareDateTime(char* strDateTime);

int setFOTASetting(int nEnable, int nWeekday, int nHour, int nMinute, int nUpdateBetaFw);
int setFOTAUpdateStatus(int nStatus);
int setFirmwareUpgradeStatus(int nStatus);
char* getHWVersion();
char* getNCVersion();
char* getFOTASetting();
int getFOTAUpdateStatus();
char* getFirmwareVersion();
char* getFirmwareVersionFull();
int getFirmwareUpgradeStatus();
char* getFirmwareInfo();
char* getDeviceUUID();
char* analyzeFirmwareInfo(int nMajor, int nMinor, int nRev, char* strFirmwareInfoFromFOTA, int nUpdateBetaFw);
#endif
