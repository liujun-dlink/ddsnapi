// libFTPClient
#ifndef LIB_FTP_CLient_H
#define LIB_FTP_CLient_H

#include <stdio.h>

int download(char* strIpAddr, int nPort, char* strlogin, char* strPassword, char* strSrcFilePath, char* strFileName, char* strDesFilePath);

int upload(
	char* strIpAddr,
	int nPort,
	char* strlogin,
	char* strPassword,
	char* strFileName,
	char* strSrcFilePath,
	int nBuildPath
);

#endif
