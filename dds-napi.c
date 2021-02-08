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


// libCLI.c
napi_value get_performance(napi_env env, napi_callback_info info);
napi_value get_performance_sync(napi_env env, napi_callback_info info);

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


	napi_property_descriptor getHddFlag = DECLARE_NAPI_METHOD("getHddFlag", get_hdd_flag);
    napi_property_descriptor getHddFlagSync = DECLARE_NAPI_METHOD("getHddFlagSync", get_hdd_flag_sync);
	napi_property_descriptor setHddFlag = DECLARE_NAPI_METHOD("setHddFlag", set_hdd_flag);
    napi_property_descriptor setHddFlagSync = DECLARE_NAPI_METHOD("setHddFlagSync", set_hdd_flag_sync);

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

	napi_define_properties(env, exports, 1, &getHddFlag);
    napi_define_properties(env, exports, 1, &getHddFlagSync);
	napi_define_properties(env, exports, 1, &setHddFlag);
    napi_define_properties(env, exports, 1, &setHddFlagSync);

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init);