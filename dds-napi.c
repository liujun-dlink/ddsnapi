// DDS NAPI
#include <node_api.h>

// libUtilityTar.c
napi_value un_tar(napi_env env, napi_callback_info info);
napi_value un_tar_sync(napi_env env, napi_callback_info info);

// libUptime.c
napi_value get_uptime(napi_env env, napi_callback_info info);
napi_value get_uptime_sync(napi_env env, napi_callback_info info);

// libKiller.c
napi_value kill_consoles_sync(napi_env env, napi_callback_info info);
napi_value kill_consoles(napi_env env, napi_callback_info info);

// libSystem.c
napi_value get_fotaStatus(napi_env env, napi_callback_info info);
napi_value get_fotaStatus_sync(napi_env env, napi_callback_info info);

// libLed.c
napi_value set_sso_led(napi_env env, napi_callback_info info);
napi_value set_sso_led_sync(napi_env env, napi_callback_info info);
napi_value set_power_led(napi_env env, napi_callback_info info);
napi_value set_power_led_sync(napi_env env, napi_callback_info info);

// libFtpSetting.c
napi_value get_ftp_data(napi_env env, napi_callback_info info);
napi_value get_ftp_data_sync(napi_env env, napi_callback_info info);
napi_value set_ftp_data(napi_env env, napi_callback_info info);
napi_value set_ftp_data_sync(napi_env env, napi_callback_info info);

// libHdd.c
napi_value get_hdd_flag(napi_env env, napi_callback_info info);
napi_value get_hdd_flag_sync(napi_env env, napi_callback_info info);
napi_value set_hdd_flag(napi_env env, napi_callback_info info);
napi_value set_hdd_flag_sync(napi_env env, napi_callback_info info);

// libDB.c
napi_value get_hw_version(napi_env env, napi_callback_info info);
napi_value get_hw_version_sync(napi_env env, napi_callback_info info);
napi_value get_nc_version(napi_env env, napi_callback_info info);
napi_value get_nc_version_sync(napi_env env, napi_callback_info info);
napi_value get_fota_setting(napi_env env, napi_callback_info info);
napi_value get_fota_setting_sync(napi_env env, napi_callback_info info);
napi_value get_firmware_version(napi_env env, napi_callback_info info);
napi_value get_firmware_version_sync(napi_env env, napi_callback_info info);
napi_value get_firmware_version_full(napi_env env, napi_callback_info info);
napi_value get_firmware_version_full_sync(napi_env env, napi_callback_info info);
napi_value get_firmware_info(napi_env env, napi_callback_info info);
napi_value get_firmware_info_sync(napi_env env, napi_callback_info info);
napi_value get_device_uuid(napi_env env, napi_callback_info info);
napi_value get_device_uuid_sync(napi_env env, napi_callback_info info);
napi_value get_firmware_upgrade_status(napi_env env, napi_callback_info info);
napi_value get_firmware_upgrade_status_sync(napi_env env, napi_callback_info info);
napi_value get_fota_update_status(napi_env env, napi_callback_info info);
napi_value get_fota_update_status_sync(napi_env env, napi_callback_info info);
napi_value set_fota_setting(napi_env env, napi_callback_info info);
napi_value set_fota_setting_sync(napi_env env, napi_callback_info info);
napi_value set_fota_update_status(napi_env env, napi_callback_info info);
napi_value set_fota_update_status_sync(napi_env env, napi_callback_info info);
napi_value set_firmware_upgrade_status(napi_env env, napi_callback_info info);
napi_value set_firmware_upgrade_status_sync(napi_env env, napi_callback_info info);
napi_value analyze_firmware_info(napi_env env, napi_callback_info info);
napi_value analyze_firmware_info_sync(napi_env env, napi_callback_info info);

//libDatetimeSetting
napi_value set_datetime(napi_env env, napi_callback_info info);
napi_value set_datetime_sync(napi_env env, napi_callback_info info);
napi_value get_datetime(napi_env env, napi_callback_info info);
napi_value get_datetime_sync(napi_env env, napi_callback_info info);
napi_value set_timezone(napi_env env, napi_callback_info info);
napi_value set_timezone_sync(napi_env env, napi_callback_info info);
napi_value get_timezone(napi_env env, napi_callback_info info);
napi_value get_timezone_sync(napi_env env, napi_callback_info info);
napi_value set_daylight_saving(napi_env env, napi_callback_info info);
napi_value set_daylight_saving_sync(napi_env env, napi_callback_info info);
napi_value get_daylight_saving(napi_env env, napi_callback_info info);
napi_value get_daylight_saving_sync(napi_env env, napi_callback_info info);
napi_value set_city_datetime(napi_env env, napi_callback_info info);
napi_value set_city_datetime_sync(napi_env env, napi_callback_info info);
napi_value get_city_datetime(napi_env env, napi_callback_info info);
napi_value get_city_datetime_sync(napi_env env, napi_callback_info info);
napi_value get_city_dst(napi_env env, napi_callback_info info);
napi_value get_city_dst_sync(napi_env env, napi_callback_info info);

// libCLI.c
napi_value get_performance(napi_env env, napi_callback_info info);
napi_value get_performance_sync(napi_env env, napi_callback_info info);
napi_value get_system_info(napi_env env, napi_callback_info info);
napi_value get_system_info_sync(napi_env env, napi_callback_info info);
napi_value set_admin_password(napi_env env, napi_callback_info info);
napi_value set_admin_password_sync(napi_env env, napi_callback_info info);
napi_value cli_save(napi_env env, napi_callback_info info);
napi_value cli_save_sync(napi_env env, napi_callback_info info);
napi_value get_device_name(napi_env env, napi_callback_info info);
napi_value get_device_name_sync(napi_env env, napi_callback_info info);
napi_value set_device_name(napi_env env, napi_callback_info info);
napi_value set_device_name_sync(napi_env env, napi_callback_info info);
napi_value get_web_access_port(napi_env env, napi_callback_info info);
napi_value get_web_access_port_sync(napi_env env, napi_callback_info info);
napi_value set_web_access_port(napi_env env, napi_callback_info info);
napi_value set_web_access_port_sync(napi_env env, napi_callback_info info);
napi_value get_nginx_access_port(napi_env env, napi_callback_info info);
napi_value get_nginx_access_port_sync(napi_env env, napi_callback_info info);
napi_value set_nginx_access_port(napi_env env, napi_callback_info info);
napi_value set_nginx_access_port_sync(napi_env env, napi_callback_info info);
napi_value get_nvr_version(napi_env env, napi_callback_info info);
napi_value get_nvr_version_sync(napi_env env, napi_callback_info info);
napi_value set_MAC_address(napi_env env, napi_callback_info info);
napi_value set_MAC_address_sync(napi_env env, napi_callback_info info);

// libConsoleSetting.c
napi_value set_console_setting(napi_env env, napi_callback_info info);
napi_value set_console_setting_sync(napi_env env, napi_callback_info info);
napi_value get_console_setting(napi_env env, napi_callback_info info);
napi_value get_console_setting_sync(napi_env env, napi_callback_info info);

// libFTPClient.c
napi_value ftp_upload(napi_env env, napi_callback_info info);
napi_value ftp_upload_sync(napi_env env, napi_callback_info info);
napi_value ftp_download(napi_env env, napi_callback_info info);
napi_value ftp_download_sync(napi_env env, napi_callback_info info);

// libFWValidate.c
napi_value check_header_and_payload(napi_env env, napi_callback_info info);
napi_value check_header_and_payload_sync(napi_env env, napi_callback_info info);

// libLANSetting.c
napi_value set_LAN_setting(napi_env env, napi_callback_info info);
napi_value set_LAN_setting_sync(napi_env env, napi_callback_info info);
napi_value get_LAN_setting(napi_env env, napi_callback_info info);
napi_value get_LAN_setting_sync(napi_env env, napi_callback_info info);
napi_value set_LAN_setting_ipv6(napi_env env, napi_callback_info info);
napi_value set_LAN_setting_ipv6_sync(napi_env env, napi_callback_info info);
napi_value get_LAN_setting_ipv6(napi_env env, napi_callback_info info);
napi_value get_LAN_setting_ipv6_sync(napi_env env, napi_callback_info info);

/*
 * 公开napi方法
 */
#define DECLARE_NAPI_METHOD(name, func) { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value init(napi_env env, napi_value exports)
{
    napi_property_descriptor unTar = DECLARE_NAPI_METHOD("unTar", un_tar);
    napi_property_descriptor unTarSync = DECLARE_NAPI_METHOD("unTarSync", un_tar_sync);
	napi_property_descriptor killConsoles = DECLARE_NAPI_METHOD("killConsoles", kill_consoles);
	napi_property_descriptor killConsolesSync = DECLARE_NAPI_METHOD("killConsolesSync", kill_consoles_sync);
	napi_property_descriptor getUptime = DECLARE_NAPI_METHOD("getUptime", get_uptime);
	napi_property_descriptor getUptimeSync = DECLARE_NAPI_METHOD("getUptimeSync", get_uptime_sync);
	napi_property_descriptor getFotaStatus = DECLARE_NAPI_METHOD("getFotaStatus", get_fotaStatus);
	napi_property_descriptor getFotaStatusSync = DECLARE_NAPI_METHOD("getFotaStatusSync", get_fotaStatus_sync);
	napi_property_descriptor setPowerLed = DECLARE_NAPI_METHOD("setPowerLed", set_power_led);
    napi_property_descriptor setPowerLedSync = DECLARE_NAPI_METHOD("setPowerLedSync", set_power_led_sync);
	napi_property_descriptor setSsoLed = DECLARE_NAPI_METHOD("setSsoLed", set_sso_led);
    napi_property_descriptor setSsoLedSync = DECLARE_NAPI_METHOD("setSsoLedSync", set_sso_led_sync);
	napi_property_descriptor getFtpData = DECLARE_NAPI_METHOD("getFtpData", get_ftp_data);
    napi_property_descriptor getFtpDataSync = DECLARE_NAPI_METHOD("getFtpDataSync", get_ftp_data_sync);
	napi_property_descriptor setFtpData = DECLARE_NAPI_METHOD("setFtpData", set_ftp_data);
    napi_property_descriptor setFtpDataSync = DECLARE_NAPI_METHOD("setFtpDataSync", set_ftp_data_sync);

	napi_property_descriptor getPerformance = DECLARE_NAPI_METHOD("getPerformance", get_performance);
    napi_property_descriptor getPerformanceSync = DECLARE_NAPI_METHOD("getPerformanceSync", get_performance_sync);
	napi_property_descriptor getSystemInfo = DECLARE_NAPI_METHOD("getSystemInfo", get_system_info);
    napi_property_descriptor getSystemInfoSync = DECLARE_NAPI_METHOD("getSystemInfoSync", get_system_info_sync);
    napi_property_descriptor setAdminPassword = DECLARE_NAPI_METHOD("setAdminPassword", set_admin_password);
    napi_property_descriptor setAdminPasswordSync = DECLARE_NAPI_METHOD("setAdminPasswordSync", set_admin_password_sync);
	napi_property_descriptor cliSave = DECLARE_NAPI_METHOD("save", cli_save);
	napi_property_descriptor cliSaveSync = DECLARE_NAPI_METHOD("saveSync", cli_save_sync);
	napi_property_descriptor getDeviceName = DECLARE_NAPI_METHOD("getDeviceName", get_device_name);
	napi_property_descriptor getDeviceNameSync = DECLARE_NAPI_METHOD("getDeviceNameSync", get_device_name_sync);
	napi_property_descriptor setDeviceName = DECLARE_NAPI_METHOD("setDeviceName", set_device_name);
	napi_property_descriptor setDeviceNameSync = DECLARE_NAPI_METHOD("setDeviceNameSync", set_device_name_sync);
	napi_property_descriptor getWebAccessPort = DECLARE_NAPI_METHOD("getWebAccessPort", get_web_access_port);
	napi_property_descriptor getWebAccessPortSync = DECLARE_NAPI_METHOD("getWebAccessPortSync", get_web_access_port_sync);
	napi_property_descriptor setWebAccessPort = DECLARE_NAPI_METHOD("setWebAccessPort", set_web_access_port);
	napi_property_descriptor setWebAccessPortSync = DECLARE_NAPI_METHOD("setWebAccessPortSync", set_web_access_port_sync);
	napi_property_descriptor getNginxAccessPort = DECLARE_NAPI_METHOD("getNginxAccessPort", get_nginx_access_port);
	napi_property_descriptor getNginxAccessPortSync = DECLARE_NAPI_METHOD("getNginxAccessPortSync", get_nginx_access_port_sync);
	napi_property_descriptor setNginxAccessPort = DECLARE_NAPI_METHOD("setNginxAccessPort", set_nginx_access_port);
	napi_property_descriptor setNginxAccessPortSync = DECLARE_NAPI_METHOD("setNginxAccessPortSync", set_nginx_access_port_sync);
	napi_property_descriptor getNvrVersion = DECLARE_NAPI_METHOD("getNvrVersion", get_nvr_version);
	napi_property_descriptor getNvrVersionSync = DECLARE_NAPI_METHOD("getNvrVersionSync", get_nvr_version_sync);
	napi_property_descriptor setMACAddress = DECLARE_NAPI_METHOD("setMACAddress", set_MAC_address);
	napi_property_descriptor setMACAddressSync = DECLARE_NAPI_METHOD("setMACAddressSync", set_MAC_address_sync);

	napi_property_descriptor getHddFlag = DECLARE_NAPI_METHOD("getHddFlag", get_hdd_flag);
    napi_property_descriptor getHddFlagSync = DECLARE_NAPI_METHOD("getHddFlagSync", get_hdd_flag_sync);
	napi_property_descriptor setHddFlag = DECLARE_NAPI_METHOD("setHddFlag", set_hdd_flag);
    napi_property_descriptor setHddFlagSync = DECLARE_NAPI_METHOD("setHddFlagSync", set_hdd_flag_sync);
	
	napi_property_descriptor setDatetime = DECLARE_NAPI_METHOD("setDatetime", set_datetime);
	napi_property_descriptor setDatetimeSync = DECLARE_NAPI_METHOD("setDatetimeSync", set_datetime_sync);
	napi_property_descriptor getDatetime = DECLARE_NAPI_METHOD("getDatetime", get_datetime);
	napi_property_descriptor getDatetimeSync = DECLARE_NAPI_METHOD("getDatetimeSync", get_datetime_sync);
	napi_property_descriptor setTimezone = DECLARE_NAPI_METHOD("setTimezone", set_timezone);
	napi_property_descriptor setTimezoneSync = DECLARE_NAPI_METHOD("setTimezoneSync", set_timezone_sync);
	napi_property_descriptor getTimezone = DECLARE_NAPI_METHOD("getTimezone", get_timezone);
	napi_property_descriptor getTimezoneSync = DECLARE_NAPI_METHOD("getTimezoneSync", get_timezone_sync);
	napi_property_descriptor setDaylightSaving = DECLARE_NAPI_METHOD("setDaylightSaving", set_daylight_saving);
	napi_property_descriptor setDaylightSavingSync = DECLARE_NAPI_METHOD("setDaylightSavingSync", set_daylight_saving_sync);
	napi_property_descriptor getDaylightSaving = DECLARE_NAPI_METHOD("getDaylightSaving", get_daylight_saving);
	napi_property_descriptor getDaylightSavingSync = DECLARE_NAPI_METHOD("getDaylightSavingSync", get_daylight_saving_sync);
	napi_property_descriptor setCityDatetime = DECLARE_NAPI_METHOD("setCityDatetime", set_city_datetime);
	napi_property_descriptor setCityDatetimeSync = DECLARE_NAPI_METHOD("setCityDatetimeSync", set_city_datetime_sync);
	napi_property_descriptor getCityDatetime = DECLARE_NAPI_METHOD("getCityDatetime", get_city_datetime);
	napi_property_descriptor getCityDatetimeSync = DECLARE_NAPI_METHOD("getCityDatetimeSync", get_city_datetime_sync);
	napi_property_descriptor getCityDst = DECLARE_NAPI_METHOD("getCityDst", get_city_dst);
	napi_property_descriptor getCityDstSync = DECLARE_NAPI_METHOD("getCityDstSync", get_city_dst_sync);
	
	napi_property_descriptor getHWVersion = DECLARE_NAPI_METHOD("getHWVersion", get_hw_version);
	napi_property_descriptor getHWVersionSync = DECLARE_NAPI_METHOD("getHWVersionSync", get_hw_version_sync);
	napi_property_descriptor getNCVersion = DECLARE_NAPI_METHOD("getNCVersion", get_nc_version);
	napi_property_descriptor getNCVersionSync = DECLARE_NAPI_METHOD("getNCVersionSync", get_nc_version_sync);
	napi_property_descriptor setFOTASetting = DECLARE_NAPI_METHOD("setFOTASetting", set_fota_setting);
	napi_property_descriptor setFOTASettingSync = DECLARE_NAPI_METHOD("setFOTASettingSync", set_fota_setting_sync);
	napi_property_descriptor getFirmwareVersion = DECLARE_NAPI_METHOD("getFirmwareVersion", get_firmware_version);
	napi_property_descriptor getFirmwareVersionSync = DECLARE_NAPI_METHOD("getFirmwareVersionSync", get_firmware_version_sync);
	napi_property_descriptor getFirmwareVersionFull = DECLARE_NAPI_METHOD("getFirmwareVersionFull", get_firmware_version_full);
	napi_property_descriptor getFirmwareVersionFullSync = DECLARE_NAPI_METHOD("getFirmwareVersionFullSync", get_firmware_version_full_sync);
	napi_property_descriptor getFirmwareInfo = DECLARE_NAPI_METHOD("getFirmwareInfo", get_firmware_info);
	napi_property_descriptor getFirmwareInfoSync = DECLARE_NAPI_METHOD("getFirmwareInfoSync", get_firmware_info_sync);
	napi_property_descriptor getDeviceUUID = DECLARE_NAPI_METHOD("getDeviceUUID", get_device_uuid);
	napi_property_descriptor getDeviceUUIDSync = DECLARE_NAPI_METHOD("getDeviceUUIDSync", get_device_uuid_sync);
	napi_property_descriptor getFirmwareUpgradeStatus = DECLARE_NAPI_METHOD("getFirmwareUpgradeStatus", get_firmware_upgrade_status);
	napi_property_descriptor getFirmwareUpgradeStatusSync = DECLARE_NAPI_METHOD("getFirmwareUpgradeStatusSync", get_firmware_upgrade_status_sync);
	napi_property_descriptor setFOTAUpdateStatus = DECLARE_NAPI_METHOD("setFOTAUpdateStatus", set_fota_update_status);
	napi_property_descriptor setFOTAUpdateStatusSync = DECLARE_NAPI_METHOD("setFOTAUpdateStatusSync", set_fota_update_status_sync);
	napi_property_descriptor getFOTASetting = DECLARE_NAPI_METHOD("getFOTASetting", get_fota_setting);
	napi_property_descriptor getFOTASettingSync = DECLARE_NAPI_METHOD("getFOTASettingSync", get_fota_setting_sync);
	napi_property_descriptor getFOTAUpdateStatus = DECLARE_NAPI_METHOD("getFOTAUpdateStatus", get_fota_update_status);
	napi_property_descriptor getFOTAUpdateStatusSync = DECLARE_NAPI_METHOD("getFOTAUpdateStatusSync", get_fota_update_status_sync);
	napi_property_descriptor setFirmwareUpgradeStatus = DECLARE_NAPI_METHOD("setFirmwareUpgradeStatus", set_firmware_upgrade_status);
	napi_property_descriptor setFirmwareUpgradeStatusSync = DECLARE_NAPI_METHOD("setFirmwareUpgradeStatusSync", set_firmware_upgrade_status_sync);
	napi_property_descriptor analyzeFirmwareInfo = DECLARE_NAPI_METHOD("analyzeFirmwareInfo", analyze_firmware_info);
	napi_property_descriptor analyzeFirmwareInfoSync = DECLARE_NAPI_METHOD("analyzeFirmwareInfoSync", analyze_firmware_info_sync);

	napi_property_descriptor setConsoleSetting = DECLARE_NAPI_METHOD("setConsoleSetting", set_console_setting);
	napi_property_descriptor setConsoleSettingSync = DECLARE_NAPI_METHOD("setConsoleSettingSync", set_console_setting_sync);
	napi_property_descriptor getConsoleSetting = DECLARE_NAPI_METHOD("getConsoleSetting", get_console_setting);
	napi_property_descriptor getConsoleSettingSync = DECLARE_NAPI_METHOD("getConsoleSettingSync", get_console_setting_sync);

	napi_property_descriptor ftpUpload = DECLARE_NAPI_METHOD("upload", ftp_upload);
	napi_property_descriptor ftpUploadSync = DECLARE_NAPI_METHOD("uploadSync", ftp_upload_sync);
	napi_property_descriptor ftpDownload = DECLARE_NAPI_METHOD("download", ftp_download);
	napi_property_descriptor ftpDownloadSync = DECLARE_NAPI_METHOD("downloadSync", ftp_download_sync);

	napi_property_descriptor checkHeaderAndPayload = DECLARE_NAPI_METHOD("checkHeaderAndPayload", check_header_and_payload);
	napi_property_descriptor checkHeaderAndPayloadSync = DECLARE_NAPI_METHOD("checkHeaderAndPayloadSync", check_header_and_payload_sync);

	napi_property_descriptor setLANSetting = DECLARE_NAPI_METHOD("setLANSetting", set_LAN_setting);
	napi_property_descriptor setLANSettingSync = DECLARE_NAPI_METHOD("setLANSettingSync", set_LAN_setting_sync);
	napi_property_descriptor getLANSetting = DECLARE_NAPI_METHOD("getLANSetting", get_LAN_setting);
	napi_property_descriptor getLANSettingSync = DECLARE_NAPI_METHOD("getLANSettingSync", get_LAN_setting_sync);
	napi_property_descriptor setLANSettingIpv6 = DECLARE_NAPI_METHOD("setLANSettingIpv6", set_LAN_setting);
	napi_property_descriptor setLANSettingIpv6Sync = DECLARE_NAPI_METHOD("setLANSettingIpv6Sync", set_LAN_setting_sync);
	napi_property_descriptor getLANSettingIpv6 = DECLARE_NAPI_METHOD("getLANSettingIpv6", get_LAN_setting);
	napi_property_descriptor getLANSettingIpv6Sync = DECLARE_NAPI_METHOD("getLANSettingIpv6Sync", get_LAN_setting_sync);

    napi_define_properties(env, exports, 1, &unTar);
    napi_define_properties(env, exports, 1, &unTarSync);
	napi_define_properties(env, exports, 1, &killConsoles);
	napi_define_properties(env, exports, 1, &killConsolesSync);
	napi_define_properties(env, exports, 1, &getUptime);
	napi_define_properties(env, exports, 1, &getUptimeSync);
	napi_define_properties(env, exports, 1, &getFotaStatus);
	napi_define_properties(env, exports, 1, &getFotaStatusSync);
	napi_define_properties(env, exports, 1, &setPowerLed);
    napi_define_properties(env, exports, 1, &setPowerLedSync);
	napi_define_properties(env, exports, 1, &setSsoLed);
    napi_define_properties(env, exports, 1, &setSsoLedSync);
	napi_define_properties(env, exports, 1, &getFtpData);
    napi_define_properties(env, exports, 1, &getFtpDataSync);
	napi_define_properties(env, exports, 1, &setFtpData);
    napi_define_properties(env, exports, 1, &setFtpDataSync);

	napi_define_properties(env, exports, 1, &getPerformance);
    napi_define_properties(env, exports, 1, &getPerformanceSync);
	napi_define_properties(env, exports, 1, &getSystemInfo);
    napi_define_properties(env, exports, 1, &getSystemInfoSync);
    napi_define_properties(env, exports, 1, &setAdminPassword);
    napi_define_properties(env, exports, 1, &setAdminPasswordSync);
	napi_define_properties(env, exports, 1, &cliSave);
	napi_define_properties(env, exports, 1, &cliSaveSync);
	napi_define_properties(env, exports, 1, &getDeviceName);
	napi_define_properties(env, exports, 1, &getDeviceNameSync);
	napi_define_properties(env, exports, 1, &setDeviceName);
	napi_define_properties(env, exports, 1, &setDeviceNameSync);
	napi_define_properties(env, exports, 1, &getWebAccessPort);
	napi_define_properties(env, exports, 1, &getWebAccessPortSync);
	napi_define_properties(env, exports, 1, &setWebAccessPort);
	napi_define_properties(env, exports, 1, &setWebAccessPortSync);
	napi_define_properties(env, exports, 1, &getNginxAccessPort);
	napi_define_properties(env, exports, 1, &getNginxAccessPortSync);
	napi_define_properties(env, exports, 1, &setNginxAccessPort);
	napi_define_properties(env, exports, 1, &setNginxAccessPortSync);
	napi_define_properties(env, exports, 1, &getNvrVersion);
	napi_define_properties(env, exports, 1, &getNvrVersionSync);
	napi_define_properties(env, exports, 1, &setMACAddress);
	napi_define_properties(env, exports, 1, &setMACAddressSync);

	napi_define_properties(env, exports, 1, &getHddFlag);
    napi_define_properties(env, exports, 1, &getHddFlagSync);
	napi_define_properties(env, exports, 1, &setHddFlag);
    napi_define_properties(env, exports, 1, &setHddFlagSync);
	
	napi_define_properties(env, exports, 1, &setDatetime);
	napi_define_properties(env, exports, 1, &setDatetimeSync);
	napi_define_properties(env, exports, 1, &getDatetime);
	napi_define_properties(env, exports, 1, &getDatetimeSync);
	napi_define_properties(env, exports, 1, &setTimezone);
	napi_define_properties(env, exports, 1, &setTimezoneSync);
	napi_define_properties(env, exports, 1, &getTimezone);
	napi_define_properties(env, exports, 1, &getTimezoneSync);
	napi_define_properties(env, exports, 1, &setDaylightSaving);
	napi_define_properties(env, exports, 1, &setDaylightSavingSync);
	napi_define_properties(env, exports, 1, &getDaylightSaving);
	napi_define_properties(env, exports, 1, &getDaylightSavingSync);
	napi_define_properties(env, exports, 1, &setCityDatetime);
	napi_define_properties(env, exports, 1, &setCityDatetimeSync);
	napi_define_properties(env, exports, 1, &getCityDatetime);
	napi_define_properties(env, exports, 1, &getCityDatetimeSync);
	napi_define_properties(env, exports, 1, &getCityDst);
	napi_define_properties(env, exports, 1, &getCityDstSync);
	
	napi_define_properties(env, exports, 1, &setFOTASetting);
	napi_define_properties(env, exports, 1, &setFOTASettingSync);
	napi_define_properties(env, exports, 1, &setFOTAUpdateStatus);
	napi_define_properties(env, exports, 1, &setFOTAUpdateStatusSync);
	napi_define_properties(env, exports, 1, &setFirmwareUpgradeStatus);
	napi_define_properties(env, exports, 1, &setFirmwareUpgradeStatusSync);
	napi_define_properties(env, exports, 1, &getHWVersion);
	napi_define_properties(env, exports, 1, &getHWVersionSync);
	napi_define_properties(env, exports, 1, &getNCVersion);
	napi_define_properties(env, exports, 1, &getNCVersionSync);
	napi_define_properties(env, exports, 1, &getFOTASetting);
	napi_define_properties(env, exports, 1, &getFOTASettingSync);
	napi_define_properties(env, exports, 1, &getFOTAUpdateStatus);
	napi_define_properties(env, exports, 1, &getFOTAUpdateStatusSync);
	napi_define_properties(env, exports, 1, &getFirmwareVersion);
	napi_define_properties(env, exports, 1, &getFirmwareVersionSync);
	napi_define_properties(env, exports, 1, &getFirmwareVersionFull);
	napi_define_properties(env, exports, 1, &getFirmwareVersionFullSync);
	napi_define_properties(env, exports, 1, &getFirmwareUpgradeStatus);
	napi_define_properties(env, exports, 1, &getFirmwareUpgradeStatusSync);
	napi_define_properties(env, exports, 1, &getFirmwareInfo);
	napi_define_properties(env, exports, 1, &getFirmwareInfoSync);
	napi_define_properties(env, exports, 1, &getDeviceUUID);
	napi_define_properties(env, exports, 1, &getDeviceUUIDSync);
	napi_define_properties(env, exports, 1, &analyzeFirmwareInfo);
	napi_define_properties(env, exports, 1, &analyzeFirmwareInfoSync);

	napi_define_properties(env, exports, 1, &setConsoleSetting);
	napi_define_properties(env, exports, 1, &setConsoleSettingSync);
	napi_define_properties(env, exports, 1, &getConsoleSetting);
	napi_define_properties(env, exports, 1, &getConsoleSettingSync);

	napi_define_properties(env, exports, 1, &ftpUpload);
	napi_define_properties(env, exports, 1, &ftpUploadSync);
	napi_define_properties(env, exports, 1, &ftpDownload);
	napi_define_properties(env, exports, 1, &ftpDownloadSync);

	napi_define_properties(env, exports, 1, &checkHeaderAndPayload);
	napi_define_properties(env, exports, 1, &checkHeaderAndPayloadSync);

	napi_define_properties(env, exports, 1, &setLANSetting);
	napi_define_properties(env, exports, 1, &setLANSettingSync);
	napi_define_properties(env, exports, 1, &getLANSetting);
	napi_define_properties(env, exports, 1, &getLANSettingSync);
	napi_define_properties(env, exports, 1, &setLANSettingIpv6);
	napi_define_properties(env, exports, 1, &setLANSettingIpv6Sync);
	napi_define_properties(env, exports, 1, &getLANSettingIpv6);
	napi_define_properties(env, exports, 1, &getLANSettingIpv6Sync);
	
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init);