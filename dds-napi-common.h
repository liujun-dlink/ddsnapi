#ifndef DDS_NAPI_COMMON_H
#define DDS_NAPI_COMMON_H

#include <stdlib.h>
#include <time.h>
#define NAPI_EXPERIMENTAL
#include <node_api.h>


//定义�?
//定义方法名称
//libCLI
#define GET_SYSTEM_INFO_STR				"getSystemInfo"
#define SET_ADMIN_PASSWORD_STR          "setAdminPassword"
#define GET_DEVICE_NAME_STR             "getDeviceName"
#define SET_DEVICE_NAME_STR             "setDeviceName"
#define GET_PERFORMANCE_STR				"getPerformance"
#define GET_WEB_ACCESS_PORT_STR         "getWebAccessPort"
#define SET_WEB_ACCESS_PORT_STR         "setWebAccessPort"
#define GET_NGINX_ACCESS_PORT_STR       "getNginxAccessPort"
#define SET_NGINX_ACCESS_PORT_STR       "setNginxAccessPort"
#define GET_NVR_VERSION_STR             "getNvrVersion"
//libDatetimeSetting
#define GET_CITY_DATETIME_STR			"getCityDatetime"
#define SET_CITY_DATETIME_STR			"setCityDatetime"
#define GET_CITY_DST_STR				"getCityDst"
#define GET_DATETIME_STR				"getDatetime"
#define GET_TIMEZONE_STR				"getTimezone"
#define GET_DAYLIGHT_SAVING_STR			"getDaylightSaving"
#define SET_DATETIME_STR				"setDatetime"
#define SET_TIMEZONE_STR				"setTimezone"
#define SET_DAYLIGHT_SAVING_STR			"setDaylightSaving"
//libDB
#define GET_HW_VERSION_STR				"getHWVersion"
#define GET_NC_VERSION_STR				"getNCVersion"
#define GET_FOAT_SETTING_STR			"getFOTASetting"
#define GET_FIRMWARE_VERSION_STR		"getFirmwareVersion"
#define GET_FIRMWARE_VERSION_FULL_STR	"getFirmwareVersionFull"
#define GET_FIRMWARE_INFO_STR			"getFirmwareInfo"
#define GET_DEVICE_UUID_STR				"getDeviceUUID"
#define GET_FOTA_UPDATE_STATUS_STR		"getFOTAUpdateStatus"
#define GET_FIRMWARE_UPGRADE_STATUS_STR	"getFirmwareUpgradeStatus"
#define SET_FOTA_SETTING_STR			"setFOTASetting"
#define SET_FOTA_UPDATE_STATUS_STR		"setFOTAUpdateStatus"
#define SET_FIRMWARE_UPGRADE_STATUS_STR	"setFirmwareUpgradeStatus"
#define ANALYZE_FIRMWARE_INFO_STR		"analyzeFirmwareInfo"

//libUptime
#define GET_UPTIME_STR					"getUptime"
//libKiller
#define KILL_CONSOLES_STR				"killConsoles"
//libFtpSetting
#define GET_FTP_DATA_STR				"getFtpData"
#define SET_FTP_DATA_STR				"setFtpData"
	
// libUtilityTar
#define UN_TAR_STR "unTar"

// libSystem
#define GET_FOTA_STATUS_STR "getFotaStatus"

// libLed
#define SET_POWER_LED_STR "setPowerLed"
#define SET_SSO_LED_STR  "setSsoLed"

// libHdd
#define GET_HDD_FLAG_STR "getHddFlag"
#define SET_HDD_FLAG_STR "setHddFlag"

// libConsoleSetting
#define SET_CONSOLE_SETTING_STR "setConsoleSetting"
#define GET_CONSOLE_SETTING_STR "getConsoleSetting"

// libFTPClient
#define DOWNLOAD_STR "download"
#define UPLOAD_STR "upload"

// libFWValidate
#define CHECK_HEADER_AND_PAYLOAD_STR "checkHeaderAndPayload"

// libLANSetting
#define SET_LAN_SETTING_STR "setLANSetting"
#define GET_LAN_SETTING_STR "getLANSetting"
#define SET_LAN_SETTING_IPV6_STR "setLANSettingIpv6"
#define GET_LAN_SETTING_IPV6_STR "getLANSettingIpv6"

//加载动态链接库文件的方�?
#define RTLD_LAZY 		1
#define RTLD_NOW        2
#define RTLD_GLOBAL     3
#define RTLD_LOCAL      4


//定义函数指针类型的别�?
//libCLI
typedef char* (*GET_SYSTEM_INFO_F_PTR)();          //getSystemInfo
typedef int (*SET_ADMIN_PASSWORD_F_PTR)(char*);          //setAdminPassword
typedef char* (*GET_DEVICE_NAME_F_PTR)();           //getDeviceName
typedef int (*SET_DEVICE_NAME_F_PTR)(char*);             //setDeviceName
typedef char* (*GET_PERFORMANCE_F_PTR)();          //getPerformance
typedef int (*GET_WEB_ACCESS_PORT_F_PTR)();         //getWebAccessPort
typedef int (*SET_WEB_ACCESS_PORT_F_PTR)(int);         //setWebAccessPort
typedef int (*GET_NGINX_ACCESS_PORT_F_PTR)();       //getNginxAccessPort
typedef int (*SET_NGINX_ACCESS_PORT_F_PTR)(int);       //setNginxAccessPort
typedef char* (*GET_NVR_VERSION_F_PTR)();           //getNvrVersion


//libDatetimeSetting
typedef char *(* GET_CITY_DATETIME_F_PTR)();	//getCityDatetime
typedef int (* SET_CITY_DATETIME_F_PTR)(char*, int, int, int, int, int, char*, int, int, int, char*);	//setCityDatetime
typedef char* (* GET_CITY_DST_F_PTR)(char*);			//getCityDst
typedef char* (*GET_DATETIME_F_PTR)();			//getDatetime
typedef char* (*GET_TIMEZONE_F_PTR)();			//getTimezone
typedef char* (*GET_DAYLIGHT_SAVING_F_PTR)();	//getDaylightSaving
typedef int (*SET_DATETIME_F_PTR)(int, int, char*, char*);			//setDatetime
typedef int (*SET_TIMEZONE_F_PTR)(int, char*);			//setTimezone
typedef int (*SET_DAYLIGHT_SAVING_F_PTR)(int, int, int, int, int, char*, int, int, int, char*);	//setDaylightSaving


//libUptime
typedef char *(* GET_UPTIME_F_PTR)();   		//getUptime

//libkiller
typedef int (* KILL_CONSOLES_F_PTR)();   		//killConsoles

//libFtpSetting
typedef char* (*GET_FTP_DATA_F_PTR)();   		//getFtpData
typedef int (*SET_FTP_DATA_F_PTR)(char*, int, char*, char*);   		//setFtpData

//libDB
typedef char* (*GET_HW_VERSION_F_PTR)();   		//getHWVersion
typedef char* (*GET_NC_VERSION_F_PTR)();   		//getNCVersion
typedef char* (*GET_FOAT_SETTING_F_PTR)();   		//getFOTASetting
typedef char* (*GET_FIRMWARE_VERSION_F_PTR)();   	//getFirmwareVersion
typedef char* (*GET_FIRMWARE_VERSION_FULL_F_PTR)(); //getFirmwareVersionFull
typedef char* (*GET_FIRMWARE_INFO_F_PTR)();   		//getFirmwareInfo
typedef char* (*GET_DEVICE_UUID_F_PTR)();   		//getDeviceUUID
typedef int (*GET_FOTA_UPDATE_STATUS_F_PTR)();   		//getFOTAUpdateStatus
typedef int (*GET_FIRMWARE_UPGRADE_STATUS_F_PTR)();   //getFirmwareUpgradeStatus
typedef int (*SET_FOTA_SETTING_F_PTR)(int, int, int, int, int);				//setFOTASetting
typedef int (*SET_FOTA_UPDATE_STATUS_F_PTR)(int);		//setFOTAUpdateStatus
typedef int (*SET_FIRMWARE_UPGRADE_STATUS_F_PTR)(int);   //setFirmwareUpgradeStatus
typedef char* (*ANALYZE_FIRMWARE_INFO_F_PTR)(int, int, int, char*, int);		//analyzeFirmwareInfo

// libUtilityTar
typedef	int (*UN_TAR_F_PTR)(char*, char*, char*); //unTar

// libSystem
typedef	int (*GET_FOTA_STATUS_F_PTR)(); //getFotaStatus

// libLed
typedef	int (*SET_POWER_LED_F_PTR)(int, int);   //setPowerLed
typedef	int (*SET_SSO_LED_F_PTR)(int);  //setSsoLed

// libHdd
typedef	int (*GET_HDD_FLAG_F_PTR)(); //getHddFlag
typedef	int (*SET_HDD_FLAG_F_PTR)(int); //setHddFlag

// libConsoleSetting
typedef	int (*SET_CONSOLE_SETTING_F_PTR)(int, char*, int); //setConsoleSetting
typedef	char* (*GET_CONSOLE_SETTING_F_PTR)(); //getConsoleSetting

// libFTPClient
typedef	int (*DOWNLOAD_F_PTR)(char*, int, char*, char*, char*, char*, char*); //download
typedef	int (*UPLOAD_F_PTR)(char*, int, char*, char*, char*, char*, int); //upload

// libFWValidate
typedef	int (*CHECK_HEADER_AND_PAYLOAD_F_PTR)(char*, char*); //checkHeaderAndPayload

// libLANSetting
typedef	int (*SET_LAN_SETTING_F_PTR)(int, int, char*, char*, char*, int, char*, char*, int); //setLANSetting
typedef	char* (*GET_LAN_SETTING_F_PTR)(); //getLANSetting
typedef	int (*SET_LAN_SETTING_IPV6_F_PTR)(int, char*, char*, int, char*, int, char*, char*, int); //setLANSettingIpv6
typedef	char* (*GET_LAN_SETTING_IPV6_F_PTR)(); //getLANSettingIpv6

//动态链接库句柄
struct AddonDataSoHandle
{
	//动态链接库句柄
	//libCLI
	void *libCLIHandle;
	//libUptime
	void *libUptimeHandle;
	//libDatetimeSetting
	void *libDatetimeSettingHandle;
	//libKiller
	void *libKillerHandle;
	//libFtpSetting
	void* libFtpSettingHandle;
	//libDB
	void* libDBHandle;
	// libUtilityTar
	void* libUtilityTarHandle;
	// libSystem
	void* libSystemHandle;
	// libLed
	void* libLedHandle;
	// libHdd
	void* libHddHandle;
	// libConsoleSetting
	void* libConsoleSettingHandle;
	// libFTPClient
	void* libFTPClientHandle;
	// libFWValidate
	void* libFWValidateHandle;
	// libLANSetting
	void* libLANSettingHandle;
};

//动态链接库句柄和函数指�?
struct AddonDataSo
{
	//动态链接库句柄
	struct AddonDataSoHandle *addonDataSoHandle;

	//函数指针
	//libCLI
	char* (*getSystemInfo)();
	int (*setAdminPassword)(char*);
	char* (*getDeviceName)();
	int (*setDeviceName)(char*);
	char* (*getPerformance)();
	int (*getWebAccessPort)();
	int (*setWebAccessPort)(int);
	int (*getNginxAccessPort)();
	int (*setNginxAccessPort)(int);
	char* (*getNvrVersion)();
	//libDatetimeSetting
	char *(*getCityDatetime)();
	int (*setCityDatetime)(char*, int, int, int, int, int, char*, int, int, int, char*);
	char* (*getCityDst)(char*);
	char* (*getDatetime)();
	char* (*getTimezone)();
	char* (*getDaylightSaving)();
	int (*setDatetime)(int, int, char*, char*);
	int (*setTimezone)(int, char*);
	int (*setDaylightSaving)(int, int, int, int, int, char*, int, int, int, char*);
	//libUptime
	char *(*getUptime)();
	//libKiller
	int (*killConsoles)();
	//libFtpSetting
	char* (*getFtpData)();
	int (*setFtpData)(char*, int, char*, char*);
	//libDB
	char* (*getHWVersion)();
	char* (*getNCVersion)();
	char* (*getFOTASetting)();
	char* (*getFirmwareVersion)();
	char* (*getFirmwareVersionFull)();
	char* (*getFirmwareInfo)();
	char* (*getDeviceUUID)();
	int (*getFOTAUpdateStatus)();
	int (*getFirmwareUpgradeStatus)();
	int (*setFOTASetting)(int, int, int, int, int);
	int (*setFOTAUpdateStatus)(int);
	int (*setFirmwareUpgradeStatus)(int);
	char* (*analyzeFirmwareInfo)(int, int, int, char*, int);
	// libUtilityTar
	int (*unTar)(char*, char*, char*);
	// libSystem
	int (*getFotaStatus)();
	// libLed
	int (*setPowerLed)(int, int);
	int (*setSsoLed)(int);
	// libHdd
	int (*getHddFlag)();
	int (*setHddFlag)(int);
	// libConsoleSetting
	int (*setConsoleSetting)(int, char*, int);
	char* (*getConsoleSetting)();
	// libFTPClient
	int (*download)(char*, int, char*, char*, char*, char*, char*);
	int (*upload)(char*, int, char*, char*, char*, char*, int);
	// libFWValidate
	int (*checkHeaderAndPayload)(char*, char*);
	// libLANSetting
	int (*setLANSetting)(int, int, char*, char*, char*, int, char*, char*, int);
	char* (*getLANSetting)();
	int (*setLANSettingIpv6)(int, char*, char*, int, char*, int, char*, char*, int);
	char* (*getLANSettingIpv6)();
};

//异步函数和线程安全函数的句柄
struct AddonDataWorkTsfn
{
    napi_async_work work;
    napi_threadsafe_function tsfn;
};

//回调函数的入�?
struct AddonDataCBInParams
{
	napi_value error;
	void *value;
	napi_value (*transferNapiValueFPtr)(napi_env, void *);
};

//通用的插件数�?
struct AddonData
{
	struct AddonDataSo *addonDataSo;
	struct AddonDataWorkTsfn *addonDataWorkTsfn;
	//扩展参数
	void *others;
	//扩展参数的释放函�?
	void (*freeOthersFPtr)(void *);
	//调用so中方法的函数
	struct AddonDataCBInParams *(*callSoMehtodFPtr)(struct AddonDataSo *, void *);
};



//定义方法
void *dlopen( const char *pathname, int mode);
void *dlsym(void* handle, const char *symbol);
int dlclose(void *handle);
char *dlerror(void);

//捕获异常信息
void catchErrorMsg(napi_env env, napi_status status);
//获取异常信息
napi_value getErrorMsg(napi_env env);
//��ʼ��Work��Tsfn
void initWorkTsfn(napi_env env, napi_value callbackFun, struct AddonData *addonData);
//执行异步函数
void executeWork(napi_env env, void* data);
//回调JS函数
void callbackJSFunction(napi_env env, napi_value js_cb, void* context, void* data);
//完成整个异步后的操作
void workComplete(napi_env env, napi_status status, void* data);

//将value转换成NAPI对应的String、int�?
napi_value transferNapiString(napi_env env, void *value);
napi_value transferNapiInt32(napi_env env, void *value);

//生成随机字符�?
void genRandomString(int length, char *ouput);


#endif