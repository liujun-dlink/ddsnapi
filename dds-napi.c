// DDS NAPI
#include <assert.h>
#include "dds-napi-common.h"
#define NAPI_EXPERIMENTAL
#include <node_api.h>


/*
 * 公开napi方法
 */
 //napi_writable | napi_enumerable | napi_configurable
#define DECLARE_NAPI_METHOD(name, func, data) { name, 0, func, 0, 0, 0, napi_writable | napi_enumerable | napi_configurable, data }


// libUtilityTar.c
napi_value unTar(napi_env env, napi_callback_info info);
napi_value unTarSync(napi_env env, napi_callback_info info);

// libUptime.c
napi_value getUptime(napi_env env, napi_callback_info info);
napi_value getUptimeSync(napi_env env, napi_callback_info info);

// libKiller.c
napi_value killConsolesSync(napi_env env, napi_callback_info info);
napi_value killConsoles(napi_env env, napi_callback_info info);

// libSystem.c
napi_value getFotaStatus(napi_env env, napi_callback_info info);
napi_value getFotaStatusSync(napi_env env, napi_callback_info info);

// libLed.c
napi_value setSsoLed(napi_env env, napi_callback_info info);
napi_value setSsoLedSync(napi_env env, napi_callback_info info);
napi_value setPowerLed(napi_env env, napi_callback_info info);
napi_value setPowerLedSync(napi_env env, napi_callback_info info);

// libFtpSetting.c
napi_value getFtpData(napi_env env, napi_callback_info info);
napi_value getFtpDataSync(napi_env env, napi_callback_info info);
napi_value setFtpData(napi_env env, napi_callback_info info);
napi_value setFtpDataSync(napi_env env, napi_callback_info info);

// libHdd.c
napi_value getHddFlag(napi_env env, napi_callback_info info);
napi_value getHddFlagSync(napi_env env, napi_callback_info info);
napi_value setHddFlag(napi_env env, napi_callback_info info);
napi_value setHddFlagSync(napi_env env, napi_callback_info info);

// libDB.c
napi_value getHWVersion(napi_env env, napi_callback_info info);
napi_value getHWVersionSync(napi_env env, napi_callback_info info);
napi_value getNCVersion(napi_env env, napi_callback_info info);
napi_value getNCVersionSync(napi_env env, napi_callback_info info);
napi_value getFOTASetting(napi_env env, napi_callback_info info);
napi_value getFOTASettingSync(napi_env env, napi_callback_info info);
napi_value getFirmwareVersion(napi_env env, napi_callback_info info);
napi_value getFirmwareVersionSync(napi_env env, napi_callback_info info);
napi_value getFirmwareVersionFull(napi_env env, napi_callback_info info);
napi_value getFirmwareVersionFullSync(napi_env env, napi_callback_info info);
napi_value getFirmwareInfo(napi_env env, napi_callback_info info);
napi_value getFirmwareInfoSync(napi_env env, napi_callback_info info);
napi_value getDeviceUUID(napi_env env, napi_callback_info info);
napi_value getDeviceUUIDSync(napi_env env, napi_callback_info info);
napi_value getFirmwareUpgradeStatus(napi_env env, napi_callback_info info);
napi_value getFirmwareUpgradeStatusSync(napi_env env, napi_callback_info info);
napi_value getFOTAUpdateStatus(napi_env env, napi_callback_info info);
napi_value getFOTAUpdateStatusSync(napi_env env, napi_callback_info info);
napi_value setFOTASetting(napi_env env, napi_callback_info info);
napi_value setFOTASettingSync(napi_env env, napi_callback_info info);
napi_value setFOTAUpdateStatus(napi_env env, napi_callback_info info);
napi_value setFOTAUpdateStatusSync(napi_env env, napi_callback_info info);
napi_value setFirmwareUpgradeStatus(napi_env env, napi_callback_info info);
napi_value setFirmwareUpgradeStatusSync(napi_env env, napi_callback_info info);
napi_value analyzeFirmwareInfo(napi_env env, napi_callback_info info);
napi_value analyzeFirmwareInfoSync(napi_env env, napi_callback_info info);

//libDatetimeSetting
napi_value setDatetime(napi_env env, napi_callback_info info);
napi_value setDatetimeSync(napi_env env, napi_callback_info info);
napi_value getDatetime(napi_env env, napi_callback_info info);
napi_value getDatetimeSync(napi_env env, napi_callback_info info);
napi_value setTimezone(napi_env env, napi_callback_info info);
napi_value setTimezoneSync(napi_env env, napi_callback_info info);
napi_value getTimezone(napi_env env, napi_callback_info info);
napi_value getTimezoneSync(napi_env env, napi_callback_info info);
napi_value setDaylightSaving(napi_env env, napi_callback_info info);
napi_value setDaylightSavingSync(napi_env env, napi_callback_info info);
napi_value getDaylightSaving(napi_env env, napi_callback_info info);
napi_value getDaylightSavingSync(napi_env env, napi_callback_info info);
napi_value setCityDatetime(napi_env env, napi_callback_info info);
napi_value setCityDatetimeSync(napi_env env, napi_callback_info info);
napi_value getCityDatetime(napi_env env, napi_callback_info info);
napi_value getCityDatetimeSync(napi_env env, napi_callback_info info);
napi_value getCityDst(napi_env env, napi_callback_info info);
napi_value getCityDstSync(napi_env env, napi_callback_info info);

// libCLI.c
napi_value getPerformance(napi_env env, napi_callback_info info);
napi_value getPerformanceSync(napi_env env, napi_callback_info info);
napi_value getSystemInfo(napi_env env, napi_callback_info info);
napi_value getSystemInfoSync(napi_env env, napi_callback_info info);
napi_value setAdminPassword(napi_env env, napi_callback_info info);
napi_value setAdminPasswordSync(napi_env env, napi_callback_info info);
napi_value getDeviceName(napi_env env, napi_callback_info info);
napi_value getDeviceNameSync(napi_env env, napi_callback_info info);
napi_value setDeviceName(napi_env env, napi_callback_info info);
napi_value setDeviceNameSync(napi_env env, napi_callback_info info);
napi_value getWebAccessPort(napi_env env, napi_callback_info info);
napi_value getWebAccessPortSync(napi_env env, napi_callback_info info);
napi_value setWebAccessPort(napi_env env, napi_callback_info info);
napi_value setWebAccessPortSync(napi_env env, napi_callback_info info);
napi_value getNginxAccessPort(napi_env env, napi_callback_info info);
napi_value getNginxAccessPortSync(napi_env env, napi_callback_info info);
napi_value setNginxAccessPort(napi_env env, napi_callback_info info);
napi_value setNginxAccessPortSync(napi_env env, napi_callback_info info);
napi_value getNvrVersion(napi_env env, napi_callback_info info);
napi_value getNvrVersionSync(napi_env env, napi_callback_info info);

// libConsoleSetting.c
napi_value setConsoleSetting(napi_env env, napi_callback_info info);
napi_value setConsoleSettingSync(napi_env env, napi_callback_info info);
napi_value getConsoleSetting(napi_env env, napi_callback_info info);
napi_value getConsoleSettingSync(napi_env env, napi_callback_info info);

// libFTPClient.c
napi_value ftpUpload(napi_env env, napi_callback_info info);
napi_value ftpUploadSync(napi_env env, napi_callback_info info);
napi_value ftpDownload(napi_env env, napi_callback_info info);
napi_value ftpDownloadSync(napi_env env, napi_callback_info info);

// libFWValidate.c
napi_value checkHeaderAndPayload(napi_env env, napi_callback_info info);
napi_value checkHeaderAndPayloadSync(napi_env env, napi_callback_info info);

// libLANSetting.c
napi_value setLANSetting(napi_env env, napi_callback_info info);
napi_value setLANSettingSync(napi_env env, napi_callback_info info);
napi_value getLANSetting(napi_env env, napi_callback_info info);
napi_value getLANSettingSync(napi_env env, napi_callback_info info);
napi_value setLANSettingIpv6(napi_env env, napi_callback_info info);
napi_value setLANSettingIpv6Sync(napi_env env, napi_callback_info info);
napi_value getLANSettingIpv6(napi_env env, napi_callback_info info);
napi_value getLANSettingIpv6Sync(napi_env env, napi_callback_info info);

void addon_getting_unloaded(napi_env env, void* data, void* hint) {

	if (data)
	{
		struct AddonDataSo* addonData = (struct AddonDataSo*)data;
		//关闭所有的动态链接库的文件句柄
		if (addonData->addonDataSoHandle)
		{
			//置空函数指针
			//libCLI
			addonData->getSystemInfo = NULL;
			addonData->setAdminPassword = NULL;
			addonData->getDeviceName = NULL;
			addonData->setDeviceName = NULL;
			addonData->getPerformance = NULL;
			addonData->getWebAccessPort = NULL;
			addonData->setWebAccessPort = NULL;
			addonData->getNginxAccessPort = NULL;
			addonData->setNginxAccessPort = NULL;
			addonData->getNvrVersion = NULL;
			//libUptime
			addonData->getUptime = NULL;
			//libDatetimeSetting
			addonData->getCityDatetime = NULL;
			addonData->setCityDatetime = NULL;
			addonData->getCityDst = NULL;
			addonData->getDatetime = NULL;
			addonData->getTimezone = NULL;
			addonData->getDaylightSaving = NULL;
			addonData->setDatetime = NULL;
			addonData->setTimezone = NULL;
			addonData->setDaylightSaving = NULL;
			//libKiller
			addonData->killConsoles = NULL;
			//libFtpSetting
			addonData->getFtpData = NULL;
			addonData->setFtpData = NULL;
			//libDB
			addonData->getHWVersion = NULL;
			addonData->getNCVersion = NULL;
			addonData->getFOTASetting = NULL;
			addonData->getFirmwareVersion = NULL;
			addonData->getFirmwareVersionFull = NULL;
			addonData->getFirmwareInfo = NULL;
			addonData->getDeviceUUID = NULL;
			addonData->getFOTAUpdateStatus = NULL;
			addonData->getFirmwareUpgradeStatus = NULL;
			addonData->setFOTASetting = NULL;
			addonData->setFOTAUpdateStatus = NULL;
			addonData->setFirmwareUpgradeStatus = NULL;
			addonData->analyzeFirmwareInfo = NULL;
			// libUtilityTar
			addonData->unTar = NULL;
			// libSystem
			addonData->getFotaStatus = NULL;
			// libLed
			addonData->setSsoLed = NULL;
			addonData->setPowerLed = NULL;
			// libHdd
			addonData->getHddFlag = NULL;
			addonData->setHddFlag = NULL;
			// libConsoleSetting
			addonData->getConsoleSetting = NULL;
			addonData->setConsoleSetting = NULL;
			// libFTPClient
			addonData->upload = NULL;
			addonData->download = NULL;
			// libFWValidate
			addonData->checkHeaderAndPayload = NULL;
			// libLANSetting
			addonData->getLANSetting = NULL;
			addonData->setLANSetting = NULL;
			addonData->getLANSettingIpv6 = NULL;
			addonData->setLANSettingIpv6 = NULL;

			//关闭句柄
			//libCLI
			if (addonData->addonDataSoHandle->libCLIHandle)
			{
				dlclose(addonData->addonDataSoHandle->libCLIHandle);
			}
			//libUptime
			if (addonData->addonDataSoHandle->libUptimeHandle)
			{
				dlclose(addonData->addonDataSoHandle->libUptimeHandle);
			}
			//libDatetimeSetting
			if (addonData->addonDataSoHandle->libDatetimeSettingHandle)
			{
				dlclose(addonData->addonDataSoHandle->libDatetimeSettingHandle);
			}
			//libKiller
			if (addonData->addonDataSoHandle->libKillerHandle)
			{
				dlclose(addonData->addonDataSoHandle->libKillerHandle);
			}
			//libFtpSetting
			if (addonData->addonDataSoHandle->libFtpSettingHandle)
			{
				dlclose(addonData->addonDataSoHandle->libFtpSettingHandle);
			}
			//libDB
			if (addonData->addonDataSoHandle->libDBHandle)
			{
				dlclose(addonData->addonDataSoHandle->libDBHandle);
			}
			// libUtilityTar
			if (addonData->addonDataSoHandle->libUtilityTarHandle)
			{
				dlclose(addonData->addonDataSoHandle->libUtilityTarHandle);
			}
			// libSystem
			if (addonData->addonDataSoHandle->libSystemHandle)
			{
				dlclose(addonData->addonDataSoHandle->libSystemHandle);
			}
			// libLed
			if (addonData->addonDataSoHandle->libLedHandle)
			{
				dlclose(addonData->addonDataSoHandle->libLedHandle);
			}
			// libHdd
			if (addonData->addonDataSoHandle->libHddHandle)
			{
				dlclose(addonData->addonDataSoHandle->libHddHandle);
			}
			// libConsoleSetting
			if (addonData->addonDataSoHandle->libConsoleSettingHandle)
			{
				dlclose(addonData->addonDataSoHandle->libConsoleSettingHandle);
			}
			// libFTPClient
			if (addonData->addonDataSoHandle->libFTPClientHandle)
			{
				dlclose(addonData->addonDataSoHandle->libFTPClientHandle);
			}
			// libFWValidate
			if (addonData->addonDataSoHandle->libFWValidateHandle)
			{
				dlclose(addonData->addonDataSoHandle->libFWValidateHandle);
			}
			// libLANSetting
			if (addonData->addonDataSoHandle->libLANSettingHandle)
			{
				dlclose(addonData->addonDataSoHandle->libLANSettingHandle);
			}
			//释放内存
			free(addonData->addonDataSoHandle);
		}
		//释放内存
		free(data);
	}
}


// napi_value init(napi_env env, napi_value exports)
// {
// 	napi_status status;
// 	//全局参数数据
// 	struct AddonDataSoHandle *addonData = 
// 		(struct AddonDataSoHandle *)malloc(sizeof(struct AddonDataSoHandle));

// 	// 初始化所有的动态链接库的文件句柄
// 	addonData->libCLIHandle = dlopen(LIB_CLI_PATH, RTLD_LAZY);
// 	addonData->libUptimeHandle = dlopen(LIB_UPTIME_PATH, RTLD_LAZY);

// 	printf("===dds-napi !addonData->libCLIHandle=== %d\n", !addonData->libCLIHandle);

// 	if(addonData->libCLIHandle)
// 	{
// 		addonData->getPerformance = (GET_PERFORMANCE_F_PTR)dlsym(addonData->libCLIHandle, "getPerformance");
// 	}

// 	printf("===dds-napi call NAPI_MODULE_INIT===\n");

//     napi_property_descriptor unTar = DECLARE_NAPI_METHOD("unTar", un_tar, addonData);
//     napi_property_descriptor unTarSync = DECLARE_NAPI_METHOD("unTarSync", un_tar_sync, addonData);
// 	napi_property_descriptor killConsoles = DECLARE_NAPI_METHOD("killConsoles", kill_consoles, addonData);
// 	napi_property_descriptor killConsolesSync = DECLARE_NAPI_METHOD("killConsolesSync", kill_consoles_sync, addonData);
// 	napi_property_descriptor getUptime = DECLARE_NAPI_METHOD("getUptime", get_uptime, addonData);
// 	napi_property_descriptor getUptimeSync = DECLARE_NAPI_METHOD("getUptimeSync", get_uptime_sync, addonData);
// 	napi_property_descriptor getFotaStatus = DECLARE_NAPI_METHOD("getFotaStatus", get_fotaStatus, addonData);
// 	napi_property_descriptor getFotaStatusSync = DECLARE_NAPI_METHOD("getFotaStatusSync", get_fotaStatus_sync, addonData);
// 	napi_property_descriptor setPowerLed = DECLARE_NAPI_METHOD("setPowerLed", set_power_led, addonData);
//     napi_property_descriptor setPowerLedSync = DECLARE_NAPI_METHOD("setPowerLedSync", set_power_led_sync, addonData);
// 	napi_property_descriptor setSsoLed = DECLARE_NAPI_METHOD("setSsoLed", set_sso_led, addonData);
//     napi_property_descriptor setSsoLedSync = DECLARE_NAPI_METHOD("setSsoLedSync", set_sso_led_sync, addonData);
// 	napi_property_descriptor getFtpData = DECLARE_NAPI_METHOD("getFtpData", get_ftp_data, addonData);
//     napi_property_descriptor getFtpDataSync = DECLARE_NAPI_METHOD("getFtpDataSync", get_ftp_data_sync, addonData);
// 	napi_property_descriptor setFtpData = DECLARE_NAPI_METHOD("setFtpData", set_ftp_data, addonData);
//     napi_property_descriptor setFtpDataSync = DECLARE_NAPI_METHOD("setFtpDataSync", set_ftp_data_sync, addonData);
// 	napi_property_descriptor getPerformance = DECLARE_NAPI_METHOD("getPerformance", get_performance, addonData);
//     napi_property_descriptor getPerformanceSync = DECLARE_NAPI_METHOD("getPerformanceSync", get_performance_sync, addonData);
// 	napi_property_descriptor getSystemInfo = DECLARE_NAPI_METHOD("getSystemInfo", get_system_info, addonData);
//     napi_property_descriptor getSystemInfoSync = DECLARE_NAPI_METHOD("getSystemInfoSync", get_system_info_sync, addonData);
// 	napi_property_descriptor getHddFlag = DECLARE_NAPI_METHOD("getHddFlag", get_hdd_flag, addonData);
//     napi_property_descriptor getHddFlagSync = DECLARE_NAPI_METHOD("getHddFlagSync", get_hdd_flag_sync, addonData);
// 	napi_property_descriptor setHddFlag = DECLARE_NAPI_METHOD("setHddFlag", set_hdd_flag, addonData);
//     napi_property_descriptor setHddFlagSync = DECLARE_NAPI_METHOD("setHddFlagSync", set_hdd_flag_sync, addonData);
// 	napi_property_descriptor setDatetime = DECLARE_NAPI_METHOD("setDatetime", set_datetime, addonData);
// 	napi_property_descriptor setDatetimeSync = DECLARE_NAPI_METHOD("setDatetimeSync", set_datetime_sync, addonData);
// 	napi_property_descriptor getDatetime = DECLARE_NAPI_METHOD("getDatetime", get_datetime, addonData);
// 	napi_property_descriptor getDatetimeSync = DECLARE_NAPI_METHOD("getDatetimeSync", get_datetime_sync, addonData);
// 	napi_property_descriptor setTimezone = DECLARE_NAPI_METHOD("setTimezone", set_timezone, addonData);
// 	napi_property_descriptor setTimezoneSync = DECLARE_NAPI_METHOD("setTimezoneSync", set_timezone_sync, addonData);
// 	napi_property_descriptor getTimezone = DECLARE_NAPI_METHOD("getTimezone", get_timezone, addonData);
// 	napi_property_descriptor getTimezoneSync = DECLARE_NAPI_METHOD("getTimezoneSync", get_timezone_sync, addonData);
// 	napi_property_descriptor setDaylightSaving = DECLARE_NAPI_METHOD("setDaylightSaving", set_daylight_saving, addonData);
// 	napi_property_descriptor setDaylightSavingSync = DECLARE_NAPI_METHOD("setDaylightSavingSync", set_daylight_saving_sync, addonData);
// 	napi_property_descriptor getDaylightSaving = DECLARE_NAPI_METHOD("getDaylightSaving", get_daylight_saving, addonData);
// 	napi_property_descriptor getDaylightSavingSync = DECLARE_NAPI_METHOD("getDaylightSavingSync", get_daylight_saving_sync, addonData);
// 	napi_property_descriptor setCityDatetime = DECLARE_NAPI_METHOD("setCityDatetime", set_city_datetime, addonData);
// 	napi_property_descriptor setCityDatetimeSync = DECLARE_NAPI_METHOD("setCityDatetimeSync", set_city_datetime_sync, addonData);
// 	napi_property_descriptor getCityDatetime = DECLARE_NAPI_METHOD("getCityDatetime", get_city_datetime, addonData);
// 	napi_property_descriptor getCityDatetimeSync = DECLARE_NAPI_METHOD("getCityDatetimeSync", get_city_datetime_sync, addonData);
// 	napi_property_descriptor getCityDst = DECLARE_NAPI_METHOD("getCityDst", get_city_dst, addonData);
// 	napi_property_descriptor getCityDstSync = DECLARE_NAPI_METHOD("getCityDstSync", get_city_dst_sync, addonData);
// 	napi_property_descriptor getHWVersion = DECLARE_NAPI_METHOD("getHWVersion", get_hw_version, addonData);
// 	napi_property_descriptor getHWVersionSync = DECLARE_NAPI_METHOD("getHWVersionSync", get_hw_version_sync, addonData);
// 	napi_property_descriptor getNCVersion = DECLARE_NAPI_METHOD("getNCVersion", get_nc_version, addonData);
// 	napi_property_descriptor getNCVersionSync = DECLARE_NAPI_METHOD("getNCVersionSync", get_nc_version_sync, addonData);
// 	napi_property_descriptor setFOTASetting = DECLARE_NAPI_METHOD("setFOTASetting", set_fota_setting, addonData);
// 	napi_property_descriptor setFOTASettingSync = DECLARE_NAPI_METHOD("setFOTASettingSync", set_fota_setting_sync, addonData);
// 	napi_property_descriptor getFirmwareVersion = DECLARE_NAPI_METHOD("getFirmwareVersion", get_firmware_version, addonData);
// 	napi_property_descriptor getFirmwareVersionSync = DECLARE_NAPI_METHOD("getFirmwareVersionSync", get_firmware_version_sync, addonData);
// 	napi_property_descriptor getFirmwareVersionFull = DECLARE_NAPI_METHOD("getFirmwareVersionFull", get_firmware_version_full, addonData);
// 	napi_property_descriptor getFirmwareVersionFullSync = DECLARE_NAPI_METHOD("getFirmwareVersionFullSync", get_firmware_version_full_sync, addonData);
// 	napi_property_descriptor getFirmwareInfo = DECLARE_NAPI_METHOD("getFirmwareInfo", get_firmware_info, addonData);
// 	napi_property_descriptor getFirmwareInfoSync = DECLARE_NAPI_METHOD("getFirmwareInfoSync", get_firmware_info_sync, addonData);
// 	napi_property_descriptor getDeviceUUID = DECLARE_NAPI_METHOD("getDeviceUUID", get_device_uuid, addonData);
// 	napi_property_descriptor getDeviceUUIDSync = DECLARE_NAPI_METHOD("getDeviceUUIDSync", get_device_uuid_sync, addonData);
// 	napi_property_descriptor getFirmwareUpgradeStatus = DECLARE_NAPI_METHOD("getFirmwareUpgradeStatus", get_firmware_upgrade_status, addonData);
// 	napi_property_descriptor getFirmwareUpgradeStatusSync = DECLARE_NAPI_METHOD("getFirmwareUpgradeStatusSync", get_firmware_upgrade_status_sync, addonData);
// 	napi_property_descriptor setFOTAUpdateStatus = DECLARE_NAPI_METHOD("setFOTAUpdateStatus", set_fota_update_status, addonData);
// 	napi_property_descriptor setFOTAUpdateStatusSync = DECLARE_NAPI_METHOD("setFOTAUpdateStatusSync", set_fota_update_status_sync, addonData);
// 	napi_property_descriptor getFOTASetting = DECLARE_NAPI_METHOD("getFOTASetting", get_fota_setting, addonData);
// 	napi_property_descriptor getFOTASettingSync = DECLARE_NAPI_METHOD("getFOTASettingSync", get_fota_setting_sync, addonData);
// 	napi_property_descriptor getFOTAUpdateStatus = DECLARE_NAPI_METHOD("getFOTAUpdateStatus", get_fota_update_status, addonData);
// 	napi_property_descriptor getFOTAUpdateStatusSync = DECLARE_NAPI_METHOD("getFOTAUpdateStatusSync", get_fota_update_status_sync, addonData);
// 	napi_property_descriptor setFirmwareUpgradeStatus = DECLARE_NAPI_METHOD("setFirmwareUpgradeStatus", set_firmware_upgrade_status, addonData);
// 	napi_property_descriptor setFirmwareUpgradeStatusSync = DECLARE_NAPI_METHOD("setFirmwareUpgradeStatusSync", set_firmware_upgrade_status_sync, addonData);
// 	napi_property_descriptor analyzeFirmwareInfo = DECLARE_NAPI_METHOD("analyzeFirmwareInfo", analyze_firmware_info, addonData);
// 	napi_property_descriptor analyzeFirmwareInfoSync = DECLARE_NAPI_METHOD("analyzeFirmwareInfoSync", analyze_firmware_info_sync, addonData);


//     napi_define_properties(env, exports, 1, &unTar);
//     napi_define_properties(env, exports, 1, &unTarSync);
// 	napi_define_properties(env, exports, 1, &killConsoles);
// 	napi_define_properties(env, exports, 1, &killConsolesSync);
// 	napi_define_properties(env, exports, 1, &getUptime);
// 	napi_define_properties(env, exports, 1, &getUptimeSync);
// 	napi_define_properties(env, exports, 1, &getFotaStatus);
// 	napi_define_properties(env, exports, 1, &getFotaStatusSync);
// 	napi_define_properties(env, exports, 1, &setPowerLed);
//     napi_define_properties(env, exports, 1, &setPowerLedSync);
// 	napi_define_properties(env, exports, 1, &setSsoLed);
//     napi_define_properties(env, exports, 1, &setSsoLedSync);
// 	napi_define_properties(env, exports, 1, &getFtpData);
//     napi_define_properties(env, exports, 1, &getFtpDataSync);
// 	napi_define_properties(env, exports, 1, &setFtpData);
//     napi_define_properties(env, exports, 1, &setFtpDataSync);
// 	napi_define_properties(env, exports, 1, &getPerformance);
//     napi_define_properties(env, exports, 1, &getPerformanceSync);
// 	napi_define_properties(env, exports, 1, &getSystemInfo);
//     napi_define_properties(env, exports, 1, &getSystemInfoSync);
// 	napi_define_properties(env, exports, 1, &getHddFlag);
//     napi_define_properties(env, exports, 1, &getHddFlagSync);
// 	napi_define_properties(env, exports, 1, &setHddFlag);
//     napi_define_properties(env, exports, 1, &setHddFlagSync);
// 	napi_define_properties(env, exports, 1, &setDatetime);
// 	napi_define_properties(env, exports, 1, &setDatetimeSync);
// 	napi_define_properties(env, exports, 1, &getDatetime);
// 	napi_define_properties(env, exports, 1, &getDatetimeSync);
// 	napi_define_properties(env, exports, 1, &setTimezone);
// 	napi_define_properties(env, exports, 1, &setTimezoneSync);
// 	napi_define_properties(env, exports, 1, &getTimezone);
// 	napi_define_properties(env, exports, 1, &getTimezoneSync);
// 	napi_define_properties(env, exports, 1, &setDaylightSaving);
// 	napi_define_properties(env, exports, 1, &setDaylightSavingSync);
// 	napi_define_properties(env, exports, 1, &getDaylightSaving);
// 	napi_define_properties(env, exports, 1, &getDaylightSavingSync);
// 	napi_define_properties(env, exports, 1, &setCityDatetime);
// 	napi_define_properties(env, exports, 1, &setCityDatetimeSync);
// 	napi_define_properties(env, exports, 1, &getCityDatetime);
// 	napi_define_properties(env, exports, 1, &getCityDatetimeSync);
// 	napi_define_properties(env, exports, 1, &getCityDst);
// 	napi_define_properties(env, exports, 1, &getCityDstSync);
// 	napi_define_properties(env, exports, 1, &setFOTASetting);
// 	napi_define_properties(env, exports, 1, &setFOTASettingSync);
// 	napi_define_properties(env, exports, 1, &setFOTAUpdateStatus);
// 	napi_define_properties(env, exports, 1, &setFOTAUpdateStatusSync);
// 	napi_define_properties(env, exports, 1, &setFirmwareUpgradeStatus);
// 	napi_define_properties(env, exports, 1, &setFirmwareUpgradeStatusSync);
// 	napi_define_properties(env, exports, 1, &getHWVersion);
// 	napi_define_properties(env, exports, 1, &getHWVersionSync);
// 	napi_define_properties(env, exports, 1, &getNCVersion);
// 	napi_define_properties(env, exports, 1, &getNCVersionSync);
// 	napi_define_properties(env, exports, 1, &getFOTASetting);
// 	napi_define_properties(env, exports, 1, &getFOTASettingSync);
// 	napi_define_properties(env, exports, 1, &getFOTAUpdateStatus);
// 	napi_define_properties(env, exports, 1, &getFOTAUpdateStatusSync);
// 	napi_define_properties(env, exports, 1, &getFirmwareVersion);
// 	napi_define_properties(env, exports, 1, &getFirmwareVersionSync);
// 	napi_define_properties(env, exports, 1, &getFirmwareVersionFull);
// 	napi_define_properties(env, exports, 1, &getFirmwareVersionFullSync);
// 	napi_define_properties(env, exports, 1, &getFirmwareUpgradeStatus);
// 	napi_define_properties(env, exports, 1, &getFirmwareUpgradeStatusSync);
// 	napi_define_properties(env, exports, 1, &getFirmwareInfo);
// 	napi_define_properties(env, exports, 1, &getFirmwareInfoSync);
// 	napi_define_properties(env, exports, 1, &getDeviceUUID);
// 	napi_define_properties(env, exports, 1, &getDeviceUUIDSync);
// 	napi_define_properties(env, exports, 1, &analyzeFirmwareInfo);
// 	napi_define_properties(env, exports, 1, &analyzeFirmwareInfoSync);

// 	status = napi_wrap(env, exports, addonData, addon_getting_unloaded, NULL, NULL);
//   	assert(status == napi_ok);
	
//     return exports;
// }

// NAPI_MODULE(NODE_GYP_MODULE_NAME, init);

//初始化so插件数据
struct AddonDataSo *initSoAddonData()
{
	//全局参数数据
	struct AddonDataSo* addonData = NULL;
	//使用缓存SO的模式，则初始化所有的动态链接库的文件句柄和获取函数指针
	#ifdef CACHE_SO_LIB
		//全局参数数据
		addonData = (struct AddonDataSo*)malloc(sizeof(struct AddonDataSo));
		//赋予默认值NULL
		addonData->addonDataSoHandle = NULL;
		//libCLI
		addonData->getSystemInfo = NULL;
		addonData->setAdminPassword = NULL;
		addonData->getDeviceName = NULL;
		addonData->setDeviceName = NULL;
		addonData->getPerformance = NULL;
		addonData->getWebAccessPort = NULL;
		addonData->setWebAccessPort = NULL;
		addonData->getNginxAccessPort = NULL;
		addonData->setNginxAccessPort = NULL;
		addonData->getNvrVersion = NULL;
		//libUptime
		addonData->getUptime = NULL;
		//libDatetimeSetting
		addonData->getCityDatetime = NULL;
		addonData->setCityDatetime = NULL;
		addonData->getCityDst = NULL;
		addonData->getDatetime = NULL;
		addonData->getTimezone = NULL;
		addonData->getDaylightSaving = NULL;
		addonData->setDatetime = NULL;
		addonData->setTimezone = NULL;
		addonData->setDaylightSaving = NULL;
		//libKiller
		addonData->killConsoles = NULL;
		//libFtpSetting
		addonData->getFtpData = NULL;
		addonData->setFtpData = NULL;
		//libDB
		addonData->getHWVersion = NULL;
		addonData->getNCVersion = NULL;
		addonData->getFOTASetting = NULL;
		addonData->getFirmwareVersion = NULL;
		addonData->getFirmwareVersionFull = NULL;
		addonData->getFirmwareInfo = NULL;
		addonData->getDeviceUUID = NULL;
		addonData->getFOTAUpdateStatus = NULL;
		addonData->getFirmwareUpgradeStatus = NULL;
		addonData->setFOTASetting = NULL;
		addonData->setFOTAUpdateStatus = NULL;
		addonData->setFirmwareUpgradeStatus = NULL;
		addonData->analyzeFirmwareInfo = NULL;
		// libUtilityTar
		addonData->unTar = NULL;
		// libSystem
		addonData->getFotaStatus = NULL;
		// libLed
		addonData->setSsoLed = NULL;
		addonData->setPowerLed = NULL;
		// libHdd
		addonData->getHddFlag = NULL;
		addonData->setHddFlag = NULL;
		// libConsoleSetting
		addonData->getConsoleSetting = NULL;
		addonData->setConsoleSetting = NULL;
		// libFTPClient
		addonData->upload = NULL;
		addonData->download = NULL;
		// libFWValidate
		addonData->checkHeaderAndPayload = NULL;
		// libLANSetting
		addonData->getLANSetting = NULL;
		addonData->setLANSetting = NULL;
		addonData->getLANSettingIpv6 = NULL;
		addonData->setLANSettingIpv6 = NULL;

		struct AddonDataSoHandle *addonDataSoHandle = 
			(struct AddonDataSoHandle *)malloc(sizeof(struct AddonDataSoHandle));
		//赋值
		addonData->addonDataSoHandle = addonDataSoHandle;
		//初始化为NULL
		addonData->addonDataSoHandle->libCLIHandle = NULL;
		addonData->addonDataSoHandle->libDatetimeSettingHandle = NULL;
		addonData->addonDataSoHandle->libUptimeHandle = NULL;
		addonData->addonDataSoHandle->libKillerHandle = NULL;
		addonData->addonDataSoHandle->libFtpSettingHandle = NULL;
		addonData->addonDataSoHandle->libDBHandle = NULL;
		addonData->addonDataSoHandle->libUtilityTarHandle = NULL;
		addonData->addonDataSoHandle->libSystemHandle = NULL;
		addonData->addonDataSoHandle->libLedHandle = NULL;
		addonData->addonDataSoHandle->libHddHandle = NULL;
		addonData->addonDataSoHandle->libConsoleSettingHandle = NULL;
		addonData->addonDataSoHandle->libFTPClientHandle = NULL;
		addonData->addonDataSoHandle->libFWValidateHandle = NULL;
		addonData->addonDataSoHandle->libLANSettingHandle = NULL;

		// 初始化所有的动态链接库的文件句柄
		//libCLI
		#ifdef LIB_CLI_PATH
			addonData->addonDataSoHandle->libCLIHandle = dlopen(LIB_CLI_PATH, RTLD_LAZY);
		#else
		#endif

		//libUptime
		#ifdef LIB_UPTIME_PATH
			addonData->addonDataSoHandle->libUptimeHandle = dlopen(LIB_UPTIME_PATH, RTLD_LAZY);
		#else
		#endif

		//libDatetimeSetting
		#ifdef LIB_DATETIMESETTING_PATH
			addonData->addonDataSoHandle->libDatetimeSettingHandle = dlopen(LIB_DATETIMESETTING_PATH, RTLD_LAZY);
		#else
		#endif

		//libKiller
		#ifdef LIB_KILLER_PATH
			addonData->addonDataSoHandle->libKillerHandle = dlopen(LIB_KILLER_PATH, RTLD_LAZY);
		#else
		#endif

		//libFtpSetting
		#ifdef LIB_FTPSETTING_PATH
			addonData->addonDataSoHandle->libFtpSettingHandle = dlopen(LIB_FTPSETTING_PATH, RTLD_LAZY);
		#else
		#endif

		//libDB
		#ifdef LIB_DB_PATH
			addonData->addonDataSoHandle->libDBHandle = dlopen(LIB_DB_PATH, RTLD_LAZY);
		#else
		#endif

		//libUtilityTar
		#ifdef LIB_UTILITYTAR_PATH
			addonData->addonDataSoHandle->libUtilityTarHandle = dlopen(LIB_UTILITYTAR_PATH, RTLD_LAZY);
		#else
		#endif

		//libSystem
		#ifdef LIB_SYSTEM_PATH
			addonData->addonDataSoHandle->libSystemHandle = dlopen(LIB_SYSTEM_PATH, RTLD_LAZY);
		#else
		#endif

		//libLed
		#ifdef LIB_LED_PATH
			addonData->addonDataSoHandle->libLedHandle = dlopen(LIB_LED_PATH, RTLD_LAZY);
		#else
		#endif

		//libHdd
		#ifdef LIB_HDD_PATH
			addonData->addonDataSoHandle->libHddHandle = dlopen(LIB_HDD_PATH, RTLD_LAZY);
		#else
		#endif

		//libConsoleSetting
		#ifdef LIB_CONSOLESETTING_PATH
			addonData->addonDataSoHandle->libConsoleSettingHandle = dlopen(LIB_CONSOLESETTING_PATH, RTLD_LAZY);
		#else
		#endif

		//libFTPClient
		#ifdef LIB_FTPCLIENT_PATH
			addonData->addonDataSoHandle->libFTPClientHandle = dlopen(LIB_FTPCLIENT_PATH, RTLD_LAZY);
		#else
		#endif

		//libFWValidate
		#ifdef LIB_FWVALIDATE_PATH
			addonData->addonDataSoHandle->libFWValidateHandle = dlopen(LIB_FWVALIDATE_PATH, RTLD_LAZY);
		#else
		#endif

		//libLANSetting
		#ifdef LIB_LANSETTING_PATH
			addonData->addonDataSoHandle->libLANSettingHandle = dlopen(LIB_LANSETTING_PATH, RTLD_LAZY);
		#else
		#endif

		// 从加载的动态链接库中获取对应的函数指针
		if(addonData->addonDataSoHandle->libCLIHandle)
		{
			addonData->getPerformance = (GET_PERFORMANCE_F_PTR)dlsym(addonData->addonDataSoHandle->libCLIHandle, GET_PERFORMANCE_STR);
			addonData->getSystemInfo = (GET_SYSTEM_INFO_F_PTR)dlsym(addonData->addonDataSoHandle->libCLIHandle, GET_SYSTEM_INFO_STR);
			addonData->setAdminPassword = (SET_ADMIN_PASSWORD_F_PTR)dlsym(addonData->addonDataSoHandle->libCLIHandle, SET_ADMIN_PASSWORD_STR);
			addonData->getDeviceName = (GET_DEVICE_NAME_F_PTR)dlsym(addonData->addonDataSoHandle->libCLIHandle, GET_DEVICE_NAME_STR);
			addonData->setDeviceName = (SET_DEVICE_NAME_F_PTR)dlsym(addonData->addonDataSoHandle->libCLIHandle, SET_DEVICE_NAME_STR);
			addonData->getWebAccessPort = (GET_WEB_ACCESS_PORT_F_PTR)dlsym(addonData->addonDataSoHandle->libCLIHandle, GET_WEB_ACCESS_PORT_STR);
			addonData->setWebAccessPort = (SET_WEB_ACCESS_PORT_F_PTR)dlsym(addonData->addonDataSoHandle->libCLIHandle, SET_WEB_ACCESS_PORT_STR);
			addonData->getNginxAccessPort = (GET_NGINX_ACCESS_PORT_F_PTR)dlsym(addonData->addonDataSoHandle->libCLIHandle, GET_NGINX_ACCESS_PORT_STR);
			addonData->setNginxAccessPort = (SET_NGINX_ACCESS_PORT_F_PTR)dlsym(addonData->addonDataSoHandle->libCLIHandle, SET_NGINX_ACCESS_PORT_STR);
			addonData->getNvrVersion = (GET_NVR_VERSION_F_PTR)dlsym(addonData->addonDataSoHandle->libCLIHandle, GET_NVR_VERSION_STR);
		}
		if(addonData->addonDataSoHandle->libDatetimeSettingHandle)
		{
			addonData->getCityDatetime = (GET_CITY_DATETIME_F_PTR)dlsym(addonData->addonDataSoHandle->libDatetimeSettingHandle, GET_CITY_DATETIME_STR);
			addonData->setCityDatetime = (SET_CITY_DATETIME_F_PTR)dlsym(addonData->addonDataSoHandle->libDatetimeSettingHandle, SET_CITY_DATETIME_STR);
			addonData->getCityDst = (GET_CITY_DST_F_PTR)dlsym(addonData->addonDataSoHandle->libDatetimeSettingHandle, GET_CITY_DST_STR);
			addonData->getDatetime = (GET_DATETIME_F_PTR)dlsym(addonData->addonDataSoHandle->libDatetimeSettingHandle, GET_DATETIME_STR);
			addonData->getTimezone = (GET_TIMEZONE_F_PTR)dlsym(addonData->addonDataSoHandle->libDatetimeSettingHandle, GET_TIMEZONE_STR);
			addonData->getDaylightSaving = (GET_DAYLIGHT_SAVING_F_PTR)dlsym(addonData->addonDataSoHandle->libDatetimeSettingHandle, GET_DAYLIGHT_SAVING_STR);
			addonData->setDatetime = (SET_DATETIME_F_PTR)dlsym(addonData->addonDataSoHandle->libDatetimeSettingHandle, SET_DATETIME_STR);
			addonData->setTimezone = (SET_TIMEZONE_F_PTR)dlsym(addonData->addonDataSoHandle->libDatetimeSettingHandle, SET_TIMEZONE_STR);
			addonData->setDaylightSaving = (SET_DAYLIGHT_SAVING_F_PTR)dlsym(addonData->addonDataSoHandle->libDatetimeSettingHandle, SET_DAYLIGHT_SAVING_STR);
		}
		if (addonData->addonDataSoHandle->libUptimeHandle)
		{
			addonData->getUptime = (GET_UPTIME_F_PTR)dlsym(addonData->addonDataSoHandle->libUptimeHandle, GET_UPTIME_STR);
		}
		if (addonData->addonDataSoHandle->libKillerHandle)
		{
			addonData->killConsoles = (KILL_CONSOLES_F_PTR)dlsym(addonData->addonDataSoHandle->libKillerHandle, KILL_CONSOLES_STR);
		}
		if (addonData->addonDataSoHandle->libFtpSettingHandle)
		{
			addonData->getFtpData = (GET_FTP_DATA_F_PTR)dlsym(addonData->addonDataSoHandle->libFtpSettingHandle, GET_FTP_DATA_STR);
			addonData->setFtpData = (SET_FTP_DATA_F_PTR)dlsym(addonData->addonDataSoHandle->libFtpSettingHandle, SET_FTP_DATA_STR);
		}
		if (addonData->addonDataSoHandle->libDBHandle)
		{
			addonData->getHWVersion = (GET_HW_VERSION_F_PTR)dlsym(addonData->addonDataSoHandle->libDBHandle, GET_HW_VERSION_STR);
			addonData->getNCVersion = (GET_NC_VERSION_F_PTR)dlsym(addonData->addonDataSoHandle->libDBHandle, GET_NC_VERSION_STR);
			addonData->getFOTASetting = (GET_FOAT_SETTING_F_PTR)dlsym(addonData->addonDataSoHandle->libDBHandle, GET_FOAT_SETTING_STR);
			addonData->getFirmwareVersion = (GET_FIRMWARE_VERSION_F_PTR)dlsym(addonData->addonDataSoHandle->libDBHandle, GET_FIRMWARE_VERSION_STR);
			addonData->getFirmwareVersionFull = (GET_FIRMWARE_VERSION_FULL_F_PTR)dlsym(addonData->addonDataSoHandle->libDBHandle, GET_FIRMWARE_VERSION_FULL_STR);
			addonData->getFirmwareInfo = (GET_FIRMWARE_INFO_F_PTR)dlsym(addonData->addonDataSoHandle->libDBHandle, GET_FIRMWARE_INFO_STR);
			addonData->getDeviceUUID = (GET_DEVICE_UUID_F_PTR)dlsym(addonData->addonDataSoHandle->libDBHandle, GET_DEVICE_UUID_STR);
			addonData->getFOTAUpdateStatus = (GET_FOTA_UPDATE_STATUS_F_PTR)dlsym(addonData->addonDataSoHandle->libDBHandle, GET_FOTA_UPDATE_STATUS_STR);
			addonData->getFirmwareUpgradeStatus = (GET_FIRMWARE_UPGRADE_STATUS_F_PTR)dlsym(addonData->addonDataSoHandle->libDBHandle, GET_FIRMWARE_UPGRADE_STATUS_STR);
			addonData->setFOTASetting = (SET_FOTA_SETTING_F_PTR)dlsym(addonData->addonDataSoHandle->libDBHandle, SET_FOTA_SETTING_STR);
			addonData->setFOTAUpdateStatus = (SET_FOTA_UPDATE_STATUS_F_PTR)dlsym(addonData->addonDataSoHandle->libDBHandle, SET_FOTA_UPDATE_STATUS_STR);
			addonData->setFirmwareUpgradeStatus = (SET_FIRMWARE_UPGRADE_STATUS_F_PTR)dlsym(addonData->addonDataSoHandle->libDBHandle, SET_FIRMWARE_UPGRADE_STATUS_STR);
			addonData->analyzeFirmwareInfo = (ANALYZE_FIRMWARE_INFO_F_PTR)dlsym(addonData->addonDataSoHandle->libDBHandle, ANALYZE_FIRMWARE_INFO_STR);
		}
		if (addonData->addonDataSoHandle->libUtilityTarHandle)
		{
			addonData->unTar = (UN_TAR_F_PTR)dlsym(addonData->addonDataSoHandle->libUtilityTarHandle, UN_TAR_STR);
		}
		if (addonData->addonDataSoHandle->libSystemHandle)
		{
			addonData->getFotaStatus = (GET_FOTA_STATUS_F_PTR)dlsym(addonData->addonDataSoHandle->libSystemHandle, GET_FOTA_STATUS_STR);
		}
		if (addonData->addonDataSoHandle->libLedHandle)
		{
			addonData->setPowerLed = (SET_POWER_LED_F_PTR)dlsym(addonData->addonDataSoHandle->libLedHandle, SET_POWER_LED_STR);
			addonData->setSsoLed = (SET_SSO_LED_F_PTR)dlsym(addonData->addonDataSoHandle->libLedHandle, SET_SSO_LED_STR);
		}
		if (addonData->addonDataSoHandle->libHddHandle)
		{
			addonData->getHddFlag = (GET_HDD_FLAG_F_PTR)dlsym(addonData->addonDataSoHandle->libHddHandle, GET_HDD_FLAG_STR);
			addonData->setHddFlag = (SET_HDD_FLAG_F_PTR)dlsym(addonData->addonDataSoHandle->libHddHandle, SET_HDD_FLAG_STR);
		}
		if (addonData->addonDataSoHandle->libConsoleSettingHandle)
		{
			addonData->setConsoleSetting = (SET_CONSOLE_SETTING_F_PTR)dlsym(addonData->addonDataSoHandle->libConsoleSettingHandle, SET_CONSOLE_SETTING_STR);
			addonData->getConsoleSetting = (GET_CONSOLE_SETTING_F_PTR)dlsym(addonData->addonDataSoHandle->libConsoleSettingHandle, GET_CONSOLE_SETTING_STR);
		}
		if (addonData->addonDataSoHandle->libFTPClientHandle)
		{
			addonData->download = (DOWNLOAD_F_PTR)dlsym(addonData->addonDataSoHandle->libFTPClientHandle, DOWNLOAD_STR);
			addonData->upload = (UPLOAD_F_PTR)dlsym(addonData->addonDataSoHandle->libFTPClientHandle, UPLOAD_STR);
		}
		if (addonData->addonDataSoHandle->libFWValidateHandle)
		{
			addonData->checkHeaderAndPayload = (CHECK_HEADER_AND_PAYLOAD_F_PTR)dlsym(addonData->addonDataSoHandle->libFWValidateHandle, CHECK_HEADER_AND_PAYLOAD_STR);
		}
		if (addonData->addonDataSoHandle->libLANSettingHandle)
		{
			addonData->setLANSetting = (SET_LAN_SETTING_F_PTR)dlsym(addonData->addonDataSoHandle->libLANSettingHandle, SET_LAN_SETTING_STR);
			addonData->getLANSetting = (GET_LAN_SETTING_F_PTR)dlsym(addonData->addonDataSoHandle->libLANSettingHandle, GET_LAN_SETTING_STR);
			addonData->setLANSettingIpv6 = (SET_LAN_SETTING_IPV6_F_PTR)dlsym(addonData->addonDataSoHandle->libLANSettingHandle, SET_LAN_SETTING_IPV6_STR);
			addonData->getLANSettingIpv6 = (GET_LAN_SETTING_IPV6_F_PTR)dlsym(addonData->addonDataSoHandle->libLANSettingHandle, GET_LAN_SETTING_IPV6_STR);
		}
	//不使用缓存SO的模式，则不用初始化所有的动态链接库的文件句柄和获取函数指针
	#else
	#endif

	return addonData;
}

NAPI_MODULE_INIT(/*env, exports*/) {
	napi_status status;

	struct AddonDataSo *addonData = initSoAddonData();
	
	napi_property_descriptor bindings[] = {
		DECLARE_NAPI_METHOD("unTar", unTar, addonData),
		DECLARE_NAPI_METHOD("unTarSync", unTarSync, addonData),
		DECLARE_NAPI_METHOD("killConsoles", killConsoles, addonData),
		DECLARE_NAPI_METHOD("killConsolesSync", killConsolesSync, addonData),
		DECLARE_NAPI_METHOD("getUptime", getUptime, addonData),
		DECLARE_NAPI_METHOD("getUptimeSync", getUptimeSync, addonData),
		DECLARE_NAPI_METHOD("getFotaStatus", getFotaStatus, addonData),
		DECLARE_NAPI_METHOD("getFotaStatusSync", getFotaStatusSync, addonData),
		DECLARE_NAPI_METHOD("setPowerLed", setPowerLed, addonData),
		DECLARE_NAPI_METHOD("setPowerLedSync", setPowerLedSync, addonData),
		DECLARE_NAPI_METHOD("setSsoLed", setSsoLed, addonData),
		DECLARE_NAPI_METHOD("setSsoLedSync", setSsoLedSync, addonData),
		DECLARE_NAPI_METHOD("getFtpData", getFtpData, addonData),
		DECLARE_NAPI_METHOD("getFtpDataSync", getFtpDataSync, addonData),
		DECLARE_NAPI_METHOD("setFtpData", setFtpData, addonData),
		DECLARE_NAPI_METHOD("setFtpDataSync", setFtpDataSync, addonData),
		DECLARE_NAPI_METHOD("getPerformance", getPerformance, addonData),
		DECLARE_NAPI_METHOD("getPerformanceSync", getPerformanceSync, addonData),
		DECLARE_NAPI_METHOD("getSystemInfo", getSystemInfo, addonData),
		DECLARE_NAPI_METHOD("getSystemInfoSync", getSystemInfoSync, addonData),
		DECLARE_NAPI_METHOD("getHddFlag", getHddFlag, addonData),
		DECLARE_NAPI_METHOD("getHddFlagSync", getHddFlagSync, addonData),
		DECLARE_NAPI_METHOD("setHddFlag", setHddFlag, addonData),
		DECLARE_NAPI_METHOD("setHddFlagSync", setHddFlagSync, addonData),
		DECLARE_NAPI_METHOD("setDatetime", setDatetime, addonData),
		DECLARE_NAPI_METHOD("setDatetimeSync", setDatetimeSync, addonData),
		DECLARE_NAPI_METHOD("getDatetime", getDatetime, addonData),
		DECLARE_NAPI_METHOD("getDatetimeSync", getDatetimeSync, addonData),
		DECLARE_NAPI_METHOD("setTimezone", setTimezone, addonData),
		DECLARE_NAPI_METHOD("setTimezoneSync", setTimezoneSync, addonData),
		DECLARE_NAPI_METHOD("getTimezone", getTimezone, addonData),
		DECLARE_NAPI_METHOD("getTimezoneSync", getTimezoneSync, addonData),
		DECLARE_NAPI_METHOD("setDaylightSaving", setDaylightSaving, addonData),
		DECLARE_NAPI_METHOD("setDaylightSavingSync", setDaylightSavingSync, addonData),
		DECLARE_NAPI_METHOD("getDaylightSaving", getDaylightSaving, addonData),
		DECLARE_NAPI_METHOD("getDaylightSavingSync", getDaylightSavingSync, addonData),
		DECLARE_NAPI_METHOD("setCityDatetime", setCityDatetime, addonData),
		DECLARE_NAPI_METHOD("setCityDatetimeSync", setCityDatetimeSync, addonData),
		DECLARE_NAPI_METHOD("getCityDatetime", getCityDatetime, addonData),
		DECLARE_NAPI_METHOD("getCityDatetimeSync", getCityDatetimeSync, addonData),
		DECLARE_NAPI_METHOD("getCityDst", getCityDst, addonData),
		DECLARE_NAPI_METHOD("getCityDstSync", getCityDstSync, addonData),
		DECLARE_NAPI_METHOD("getHWVersion", getHWVersion, addonData),
		DECLARE_NAPI_METHOD("getHWVersionSync", getHWVersionSync, addonData),
		DECLARE_NAPI_METHOD("getNCVersion", getNCVersion, addonData),
		DECLARE_NAPI_METHOD("getNCVersionSync", getNCVersionSync, addonData),
		DECLARE_NAPI_METHOD("setFOTASetting", setFOTASetting, addonData),
		DECLARE_NAPI_METHOD("setFOTASettingSync", setFOTASettingSync, addonData),
		DECLARE_NAPI_METHOD("getFirmwareVersion", getFirmwareVersion, addonData),
		DECLARE_NAPI_METHOD("getFirmwareVersionSync", getFirmwareVersionSync, addonData),
		DECLARE_NAPI_METHOD("getFirmwareVersionFull", getFirmwareVersionFull, addonData),
		DECLARE_NAPI_METHOD("getFirmwareVersionFullSync", getFirmwareVersionFullSync, addonData),
		DECLARE_NAPI_METHOD("getFirmwareInfo", getFirmwareInfo, addonData),
		DECLARE_NAPI_METHOD("getFirmwareInfoSync", getFirmwareInfoSync, addonData),
		DECLARE_NAPI_METHOD("getDeviceUUID", getDeviceUUID, addonData),
		DECLARE_NAPI_METHOD("getDeviceUUIDSync", getDeviceUUIDSync, addonData),
		DECLARE_NAPI_METHOD("getFirmwareUpgradeStatus", getFirmwareUpgradeStatus, addonData),
		DECLARE_NAPI_METHOD("getFirmwareUpgradeStatusSync", getFirmwareUpgradeStatusSync, addonData),
		DECLARE_NAPI_METHOD("setFOTAUpdateStatus", setFOTAUpdateStatus, addonData),
		DECLARE_NAPI_METHOD("setFOTAUpdateStatusSync", setFOTAUpdateStatusSync, addonData),
		DECLARE_NAPI_METHOD("getFOTASetting", getFOTASetting, addonData),
		DECLARE_NAPI_METHOD("getFOTASettingSync", getFOTASettingSync, addonData),
		DECLARE_NAPI_METHOD("getFOTAUpdateStatus", getFOTAUpdateStatus, addonData),
		DECLARE_NAPI_METHOD("getFOTAUpdateStatusSync", getFOTAUpdateStatusSync, addonData),
		DECLARE_NAPI_METHOD("setFirmwareUpgradeStatus", setFirmwareUpgradeStatus, addonData),
		DECLARE_NAPI_METHOD("setFirmwareUpgradeStatusSync", setFirmwareUpgradeStatusSync, addonData),
		DECLARE_NAPI_METHOD("analyzeFirmwareInfo", analyzeFirmwareInfo, addonData),
		DECLARE_NAPI_METHOD("analyzeFirmwareInfoSync", analyzeFirmwareInfoSync, addonData),
		DECLARE_NAPI_METHOD("setAdminPassword", setAdminPassword, addonData),
		DECLARE_NAPI_METHOD("setAdminPasswordSync", setAdminPasswordSync, addonData),
		DECLARE_NAPI_METHOD("getDeviceName", getDeviceName, addonData),
		DECLARE_NAPI_METHOD("getDeviceNameSync", getDeviceNameSync, addonData),
		DECLARE_NAPI_METHOD("setDeviceName", setDeviceName, addonData),
		DECLARE_NAPI_METHOD("setDeviceNameSync", setDeviceNameSync, addonData),
		DECLARE_NAPI_METHOD("getWebAccessPort", getWebAccessPort, addonData),
		DECLARE_NAPI_METHOD("getWebAccessPortSync", getWebAccessPortSync, addonData),
		DECLARE_NAPI_METHOD("setWebAccessPort", setWebAccessPort, addonData),
		DECLARE_NAPI_METHOD("setWebAccessPortSync", setWebAccessPortSync, addonData),
		DECLARE_NAPI_METHOD("getNginxAccessPort", getNginxAccessPort, addonData),
		DECLARE_NAPI_METHOD("getNginxAccessPortSync", getNginxAccessPortSync, addonData),
		DECLARE_NAPI_METHOD("setNginxAccessPort", setNginxAccessPort, addonData),
		DECLARE_NAPI_METHOD("setNginxAccessPortSync", setNginxAccessPortSync, addonData),
		DECLARE_NAPI_METHOD("getNvrVersion", getNvrVersion, addonData),
		DECLARE_NAPI_METHOD("getNvrVersionSync", getNvrVersionSync, addonData),
		DECLARE_NAPI_METHOD("setConsoleSetting", setConsoleSetting, addonData),
		DECLARE_NAPI_METHOD("setConsoleSettingSync", setConsoleSettingSync, addonData),
		DECLARE_NAPI_METHOD("getConsoleSetting", getConsoleSetting, addonData),
		DECLARE_NAPI_METHOD("getConsoleSettingSync", getConsoleSettingSync, addonData),
		DECLARE_NAPI_METHOD("upload", ftpUpload, addonData),
		DECLARE_NAPI_METHOD("uploadSync", ftpUploadSync, addonData),
		DECLARE_NAPI_METHOD("download", ftpDownload, addonData),
		DECLARE_NAPI_METHOD("downloadSync", ftpDownloadSync, addonData),
		DECLARE_NAPI_METHOD("checkHeaderAndPayload", checkHeaderAndPayload, addonData),
		DECLARE_NAPI_METHOD("checkHeaderAndPayloadSync", checkHeaderAndPayloadSync, addonData),
		DECLARE_NAPI_METHOD("setLANSetting", setLANSetting, addonData),
		DECLARE_NAPI_METHOD("setLANSettingSync", setLANSettingSync, addonData),
		DECLARE_NAPI_METHOD("getLANSetting", getLANSetting, addonData),
		DECLARE_NAPI_METHOD("getLANSettingSync", getLANSettingSync, addonData),
		DECLARE_NAPI_METHOD("setLANSettingIpv6", setLANSettingIpv6, addonData),
		DECLARE_NAPI_METHOD("setLANSettingIpv6Sync", setLANSettingIpv6Sync, addonData),
		DECLARE_NAPI_METHOD("getLANSettingIpv6", getLANSettingIpv6, addonData),
		DECLARE_NAPI_METHOD("getLANSettingIpv6Sync", getLANSettingIpv6Sync, addonData)
	};
	
	napi_define_properties(env, exports, sizeof(bindings) / sizeof(bindings[0]), bindings);
	assert(status == napi_ok);

	status = napi_wrap(env, exports, addonData, addon_getting_unloaded, NULL, NULL);
  	assert(status == napi_ok);
		
    return exports;
}