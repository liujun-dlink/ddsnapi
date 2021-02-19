// libLANSetting
#ifndef LIB_LAN_SETTING_H
#define LIB_LAN_SETTING_H

#include <stdio.h>

// 该方法有可能有重构（传参不同）
int setLANSetting(
	int isPermanent,
	int isDHCP,
	char* argIpAddress,
	char* argNetmask,
	char* argGateway,
	int nDnsType,
	char* argDNS1,
	char* argDNS2,
	int flagUpdateDaa
);

char* getLANSetting();

int setLANSettingIpv6(
	int isPermanent,
	char* argProvision,
	char* argIpv6Address,
	int nPrefixLength,
	char* argGateway,
	int nDnsType,
	char* argDns1,
	char* argDns2,
	int flagUpdateDaa
);

char* getLANSettingIpv6();

// char* getIpv6Provision();

#endif
