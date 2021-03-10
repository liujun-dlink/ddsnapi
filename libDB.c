#include <string.h>
#include "dds-napi-common.h"
#ifdef CACHE_SO_LIB
#else
	#include "libDB.h"
#endif

// ----------------char* getHWVersion()
napi_value getHWVersionSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getHWVersion)
        {
            valuePtr = addonDataSo->getHWVersion();
        }
    #else
        valuePtr = getHWVersion();
    #endif
    // 转类型
    napi_value methodReult;
    napi_value errorInfo;
    // 为其赋默认值null
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }
    errorInfo = getErrorMsg(env);
    // 回调函数参数
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    // 判断是否存在回调方法
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[0], 2, callbackParams, &returnVal);
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoGetHWVersion(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getHWVersion)
        {
            valuePtr = addonDataSo->getHWVersion();
        }
    #else
        valuePtr = getHWVersion();
    #endif

    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getHWVersion(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];

    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // 分配内存空间，在work_complete中会释放
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //需要进行手动的malloc，否则访问成员变量work和tsfn会出现错误
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //没有其他参数即设置为null即可
    addon_data->others = NULL;
    //非分配地址空间参数，即无需释放内存方法
    addon_data->freeOthersFPtr = NULL;
    addon_data->callSoMehtodFPtr = &callSoGetHWVersion;
    //初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}


// ----------------char* getNCVersion()
napi_value getNCVersionSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getNCVersion)
        {
            valuePtr = addonDataSo->getNCVersion();
        }
    #else
        valuePtr = getNCVersion();
    #endif
    // 转类型
    napi_value methodReult;
    napi_value errorInfo;
    // 为其赋默认值null
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }
    errorInfo = getErrorMsg(env);
    // 回调函数参数
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    // 判断是否存在回调方法
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[0], 2, callbackParams, &returnVal);
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoGetNCVersion(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getNCVersion)
        {
            valuePtr = addonDataSo->getNCVersion();
        }
    #else
        valuePtr = getNCVersion();
    #endif

    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getNCVersion(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];

    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // 分配内存空间，在work_complete中会释放
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //需要进行手动的malloc，否则访问成员变量work和tsfn会出现错误
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //没有其他参数即设置为null即可
    addon_data->others = NULL;
    //非分配地址空间参数，即无需释放内存方法
    addon_data->freeOthersFPtr = NULL;
    addon_data->callSoMehtodFPtr = &callSoGetNCVersion;
    //初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}


// ----------------char* getFOTASetting()
napi_value getFOTASettingSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFOTASetting)
        {
            valuePtr = addonDataSo->getFOTASetting();
        }
    #else
        valuePtr = getFOTASetting();
    #endif
    // 转类型
    napi_value methodReult;
    napi_value errorInfo;
    // 为其赋默认值null
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }
    errorInfo = getErrorMsg(env);
    // 回调函数参数
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    // 判断是否存在回调方法
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[0], 2, callbackParams, &returnVal);
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoGetFOTASetting(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFOTASetting)
        {
            valuePtr = addonDataSo->getFOTASetting();
        }
    #else
        valuePtr = getFOTASetting();
    #endif

    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getFOTASetting(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];

    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // 分配内存空间，在work_complete中会释放
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //需要进行手动的malloc，否则访问成员变量work和tsfn会出现错误
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //没有其他参数即设置为null即可
    addon_data->others = NULL;
    //非分配地址空间参数，即无需释放内存方法
    addon_data->freeOthersFPtr = NULL;
    addon_data->callSoMehtodFPtr = &callSoGetFOTASetting;
    //初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}


// ----------------char* getFirmwareVersion()
napi_value getFirmwareVersionSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFirmwareVersion)
        {
            valuePtr = addonDataSo->getFirmwareVersion();
        }
    #else
        valuePtr = getFirmwareVersion();
    #endif
    // 转类型
    napi_value methodReult;
    napi_value errorInfo;
    // 为其赋默认值null
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }
    errorInfo = getErrorMsg(env);
    // 回调函数参数
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    // 判断是否存在回调方法
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[0], 2, callbackParams, &returnVal);
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoGetFirmwareVersion(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFirmwareVersion)
        {
            valuePtr = addonDataSo->getFirmwareVersion();
        }
    #else
        valuePtr = getFirmwareVersion();
    #endif

    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getFirmwareVersion(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];

    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // 分配内存空间，在work_complete中会释放
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //需要进行手动的malloc，否则访问成员变量work和tsfn会出现错误
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //没有其他参数即设置为null即可
    addon_data->others = NULL;
    //非分配地址空间参数，即无需释放内存方法
    addon_data->freeOthersFPtr = NULL;
    addon_data->callSoMehtodFPtr = &callSoGetFirmwareVersion;
    //初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}


// ----------------char* getFirmwareVersionFull()
napi_value getFirmwareVersionFullSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFirmwareVersionFull)
        {
            valuePtr = addonDataSo->getFirmwareVersionFull();
        }
    #else
        valuePtr = getFirmwareVersionFull();
    #endif
    // 转类型
    napi_value methodReult;
    napi_value errorInfo;
    // 为其赋默认值null
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }
    errorInfo = getErrorMsg(env);
    // 回调函数参数
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    // 判断是否存在回调方法
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[0], 2, callbackParams, &returnVal);
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoGetFirmwareVersionFull(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFirmwareVersionFull)
        {
            valuePtr = addonDataSo->getFirmwareVersionFull();
        }
    #else
        valuePtr = getFirmwareVersionFull();
    #endif

    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getFirmwareVersionFull(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];

    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // 分配内存空间，在work_complete中会释放
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //需要进行手动的malloc，否则访问成员变量work和tsfn会出现错误
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //没有其他参数即设置为null即可
    addon_data->others = NULL;
    //非分配地址空间参数，即无需释放内存方法
    addon_data->freeOthersFPtr = NULL;
    addon_data->callSoMehtodFPtr = &callSoGetFirmwareVersionFull;
    //初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}


// ----------------char* getFirmwareInfo()
napi_value getFirmwareInfoSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFirmwareInfo)
        {
            valuePtr = addonDataSo->getFirmwareInfo();
        }
    #else
        valuePtr = getFirmwareInfo();
    #endif
    // 转类型
    napi_value methodReult;
    napi_value errorInfo;
    // 为其赋默认值null
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }
    errorInfo = getErrorMsg(env);
    // 回调函数参数
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    // 判断是否存在回调方法
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[0], 2, callbackParams, &returnVal);
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoGetFirmwareInfo(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFirmwareInfo)
        {
            valuePtr = addonDataSo->getFirmwareInfo();
        }
    #else
        valuePtr = getFirmwareInfo();
    #endif

    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getFirmwareInfo(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];

    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // 分配内存空间，在work_complete中会释放
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //需要进行手动的malloc，否则访问成员变量work和tsfn会出现错误
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //没有其他参数即设置为null即可
    addon_data->others = NULL;
    //非分配地址空间参数，即无需释放内存方法
    addon_data->freeOthersFPtr = NULL;
    addon_data->callSoMehtodFPtr = &callSoGetFirmwareInfo;
    //初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

// ----------------char* getDeviceUUID()
napi_value getDeviceUUIDSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getDeviceUUID)
        {
            valuePtr = addonDataSo->getDeviceUUID();
        }
    #else
        valuePtr = getDeviceUUID();
    #endif
    // 转类型
    napi_value methodReult;
    napi_value errorInfo;
    // 为其赋默认值null
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }
    errorInfo = getErrorMsg(env);
    // 回调函数参数
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    // 判断是否存在回调方法
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[0], 2, callbackParams, &returnVal);
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoGetDeviceUUID(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getDeviceUUID)
        {
            valuePtr = addonDataSo->getDeviceUUID();
        }
    #else
        valuePtr = getDeviceUUID();
    #endif

    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getDeviceUUID(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];

    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // 分配内存空间，在work_complete中会释放
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //需要进行手动的malloc，否则访问成员变量work和tsfn会出现错误
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //没有其他参数即设置为null即可
    addon_data->others = NULL;
    //非分配地址空间参数，即无需释放内存方法
    addon_data->freeOthersFPtr = NULL;
    addon_data->callSoMehtodFPtr = &callSoGetDeviceUUID;
    //初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

//------------int getFOTAUpdateStatus();
napi_value getFOTAUpdateStatusSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFOTAUpdateStatus)
        {
            valuePtr = addonDataSo->getFOTAUpdateStatus();
        }
    #else
        valuePtr = getFOTAUpdateStatus();
    #endif
    // 转类型
    napi_value methodReult;
    napi_value errorInfo;
    // 为其赋默认值null
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    errorInfo = getErrorMsg(env);
    // 回调函数参数
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    //判断是否存在回调方法
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[0], 2, callbackParams, &returnVal);
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoGetFOTAUpdateStatus(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->value = NULL;
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFOTAUpdateStatus)
        {
            valuePtr = addonDataSo->getFOTAUpdateStatus();
        }
    #else
        valuePtr = getFOTAUpdateStatus();
    #endif
    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;

    return addonDataCBInParams;
}

napi_value getFOTAUpdateStatus(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 1)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // 分配内存空间，在work_complete中会释放
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //需要进行手动的malloc，否则访问成员变量work和tsfn会出现错误
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //没有其他参数即设置为null即可
    addon_data->others = NULL;
    //非分配地址空间参数，即无需释放内存方法
    addon_data->freeOthersFPtr = NULL;
    //设置调用方法
    addon_data->callSoMehtodFPtr = &callSoGetFOTAUpdateStatus;
    //初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

//------------int getFirmwareUpgradeStatus();
napi_value getFirmwareUpgradeStatusSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFirmwareUpgradeStatus)
        {
            valuePtr = addonDataSo->getFirmwareUpgradeStatus();
        }
    #else
        valuePtr = getFirmwareUpgradeStatus();
    #endif
    // 转类型
    napi_value methodReult;
    napi_value errorInfo;
    // 为其赋默认值null
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    errorInfo = getErrorMsg(env);
    // 回调函数参数
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    //判断是否存在回调方法
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[0], 2, callbackParams, &returnVal);
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoGetFirmwareUpgradeStatus(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->value = NULL;
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFirmwareUpgradeStatus)
        {
            valuePtr = addonDataSo->getFirmwareUpgradeStatus();
        }
    #else
        valuePtr = getFirmwareUpgradeStatus();
    #endif
    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;

    return addonDataCBInParams;
}

napi_value getFirmwareUpgradeStatus(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 1)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // 分配内存空间，在work_complete中会释放
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //需要进行手动的malloc，否则访问成员变量work和tsfn会出现错误
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //没有其他参数即设置为null即可
    addon_data->others = NULL;
    //非分配地址空间参数，即无需释放内存方法
    addon_data->freeOthersFPtr = NULL;
    //设置调用方法
    addon_data->callSoMehtodFPtr = &callSoGetFirmwareUpgradeStatus;
    //初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}


// ----int setFOTAUpdateStatus

napi_value setFOTAUpdateStatusSync(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 2)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    //nStatus
    int nStatus = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &nStatus));
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setFOTAUpdateStatus)
        {
            valuePtr = addonDataSo->setFOTAUpdateStatus(nStatus);
        }
    #else
        valuePtr = setFOTAUpdateStatus(nStatus);
    #endif
    // 转类型
    napi_value methodReult;
    napi_value errorInfo;
    // 为其赋默认值null
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    errorInfo = getErrorMsg(env);
    // 回调函数参数
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    // 判断是否存在回调方法
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[1], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[1], 2, callbackParams, &returnVal);
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoSetFOTAUpdateStatus(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->value = NULL;
    int nStatus = (int)params;
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setFOTAUpdateStatus)
        {
            valuePtr = addonDataSo->setFOTAUpdateStatus(nStatus);
        }
    #else
        valuePtr = setFOTAUpdateStatus(nStatus);
    #endif
    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value setFOTAUpdateStatus(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 2)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[1], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    //nStatus
    int nStatus = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &nStatus));
    // 分配内存空间，在work_complete中会释放
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //需要进行手动的malloc，否则访问成员变量work和tsfn会出现错误
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //没有其他参数即设置为null即可
    addon_data->others = nStatus;
    //非分配地址空间参数，即无需释放内存方法
    addon_data->freeOthersFPtr = NULL;
    addon_data->callSoMehtodFPtr = &callSoSetFOTAUpdateStatus;
    //初始化
    initWorkTsfn(env, argv[1], addon_data);
    return NULL;
}


// ----int setFirmwareUpgradeStatus

napi_value setFirmwareUpgradeStatusSync(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 2)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    //nStatus
    int nStatus = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &nStatus));
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setFirmwareUpgradeStatus)
        {
            valuePtr = addonDataSo->setFirmwareUpgradeStatus(nStatus);
        }
    #else
        valuePtr = setFirmwareUpgradeStatus(nStatus);
    #endif
    // 转类型
    napi_value methodReult;
    napi_value errorInfo;
    // 为其赋默认值null
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    errorInfo = getErrorMsg(env);
    // 回调函数参数
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    // 判断是否存在回调方法
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[1], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[1], 2, callbackParams, &returnVal);
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoSetFirmwareUpgradeStatus(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->value = NULL;
    int nStatus = (int)params;
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setFirmwareUpgradeStatus)
        {
            valuePtr = addonDataSo->setFirmwareUpgradeStatus(nStatus);
        }
    #else
        valuePtr = setFirmwareUpgradeStatus(nStatus);
    #endif
    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value setFirmwareUpgradeStatus(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 2)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[1], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    //nStatus
    int nStatus = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &nStatus));
    // 分配内存空间，在work_complete中会释放
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //需要进行手动的malloc，否则访问成员变量work和tsfn会出现错误
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //没有其他参数即设置为null即可
    addon_data->others = nStatus;
    //非分配地址空间参数，即无需释放内存方法
    addon_data->freeOthersFPtr = NULL;
    addon_data->callSoMehtodFPtr = &callSoSetFirmwareUpgradeStatus;
    //初始化
    initWorkTsfn(env, argv[1], addon_data);
    return NULL;
}


//------------int setFOTASetting();

struct AddonDataSetFOTASetting
{
    int nEnable;
    int nWeekday;
    int nHour;
    int nMinute;
    int nUpdateBetaFw;
};

//释放others参数的内存
void freeSetFOTASettingParams(void* params)
{
    struct AddonDataSetFOTASetting* data = (struct AddonDataSetFOTASetting*)params;
    free(data);
}

napi_value setFOTASettingSync(napi_env env, napi_callback_info info) {
    size_t argc = 6;
    napi_value argv[6];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 6)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    //nEnable
    int nEnable = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &nEnable));
    //nWeekday
    int nWeekday = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &nWeekday));
    //nHour
    int nHour = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[2], &nHour));
    //nMinute
    int nMinute = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[3], &nMinute));
    //nUpdateBetaFw
    int nUpdateBetaFw = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[4], &nUpdateBetaFw));
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setFOTASetting)
        {
            valuePtr = addonDataSo->setFOTASetting(nEnable, nWeekday, nHour, nMinute, nUpdateBetaFw);
        }
    #else
        valuePtr = setFOTASetting(nEnable, nWeekday, nHour, nMinute, nUpdateBetaFw);
    #endif
    // 转类型
    napi_value methodReult;
    napi_value errorInfo;
    // 为其赋默认值null
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    errorInfo = getErrorMsg(env);
    // 回调函数参数
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    //判断是否存在回调方法
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[5], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[5], 2, callbackParams, &returnVal);
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoSetFOTASetting(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    //获取传参
    addonDataCBInParams->value = NULL;
    struct AddonDataSetFOTASetting* data = (struct AddonDataSetFOTASetting*)params;
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setFOTASetting)
        {
            valuePtr = addonDataSo->setFOTASetting(data->nEnable, data->nWeekday, data->nHour, data->nMinute, data->nUpdateBetaFw);
        }
    #else
        valuePtr = setFOTASetting(data->nEnable, data->nWeekday, data->nHour, data->nMinute, data->nUpdateBetaFw);
    #endif
    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;

    return addonDataCBInParams;
}

napi_value setFOTASetting(napi_env env, napi_callback_info info) {
    size_t argc = 6;
    napi_value argv[6];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 6)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[5], &valueTypeLast);

    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    struct AddonDataSetFOTASetting* data = (struct AddonDataSetFOTASetting*)malloc(sizeof(struct AddonDataSetFOTASetting));
    //获取参数
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &data->nEnable));
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &data->nWeekday));
    catchErrorMsg(env, napi_get_value_int32(env, argv[2], &data->nHour));
    catchErrorMsg(env, napi_get_value_int32(env, argv[3], &data->nMinute));
    catchErrorMsg(env, napi_get_value_int32(env, argv[4], &data->nUpdateBetaFw));
    // 分配内存空间，在work_complete中会释放
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //需要进行手动的malloc，否则访问成员变量work和tsfn会出现错误
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //没有其他参数即设置为null即可
    addon_data->others = data;
    //非分配地址空间参数，即无需释放内存方法
    addon_data->freeOthersFPtr = &freeSetFOTASettingParams;
    //设置调用方法
    addon_data->callSoMehtodFPtr = &callSoSetFOTASetting;
    //初始化
    initWorkTsfn(env, argv[5], addon_data);
    return NULL;
}

//------------char* analyzeFirmwareInfo();

struct AddonDataAnalyzeFirmwareInfo
{
    int nMajor;
    int nMinor;
    int nRev;
    char* strFirmwareInfoFromFOTA;
    int nUpdateBetaFw;
};

//释放others参数的内存
void freeAnalyzeFirmwareInfoParams(void* params)
{
    struct AddonDataAnalyzeFirmwareInfo* data = (struct AddonDataAnalyzeFirmwareInfo*)params;
    free(data->strFirmwareInfoFromFOTA);
    free(data);
}

napi_value analyzeFirmwareInfoSync(napi_env env, napi_callback_info info) {
    size_t argc = 6;
    napi_value argv[6];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 6)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    // 第一个参数：动态链接库的全路径(int nMajor, int nMinor, int nRev, char* strFirmwareInfoFromFOTA, int nUpdateBetaFw)
    size_t strLength;
    //nMajor
    int nMajor = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &nMajor));
    //nMinor
    int nMinor = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &nMinor));
    //nRev
    int nRev = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[2], &nRev));
    //strFirmwareInfoFromFOTA
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    char* strFirmwareInfoFromFOTA = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], strFirmwareInfoFromFOTA, strLength, &strLength));
    //nUpdateBetaFw
    int nUpdateBetaFw = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[4], &nUpdateBetaFw));
    //调用so函数
    char *valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->analyzeFirmwareInfo)
        {
            valuePtr = addonDataSo->analyzeFirmwareInfo(nMajor, nMinor, nRev, strFirmwareInfoFromFOTA, nUpdateBetaFw);
        }
    #else
        valuePtr = analyzeFirmwareInfo(nMajor, nMinor, nRev, strFirmwareInfoFromFOTA, nUpdateBetaFw);
    #endif
    // 转类型
    napi_value methodReult;
    napi_value errorInfo;
    // 为其赋默认值null
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }
    errorInfo = getErrorMsg(env);
    // 回调函数参数
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    //判断是否存在回调方法
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[5], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[5], 2, callbackParams, &returnVal);
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoAnalyzeFirmwareInfo(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    //获取传参
    addonDataCBInParams->value = NULL;
    struct AddonDataAnalyzeFirmwareInfo* data = (struct AddonDataAnalyzeFirmwareInfo*)params;
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->analyzeFirmwareInfo)
        {
            valuePtr = addonDataSo->analyzeFirmwareInfo(data->nMajor, data->nMinor, data->nRev, data->strFirmwareInfoFromFOTA, data->nUpdateBetaFw);
        }
    #else
        valuePtr = analyzeFirmwareInfo(data->nMajor, data->nMinor, data->nRev, data->strFirmwareInfoFromFOTA, data->nUpdateBetaFw);
    #endif
    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }

    return addonDataCBInParams;
}

napi_value analyzeFirmwareInfo(napi_env env, napi_callback_info info) {
    size_t argc = 6;
    napi_value argv[6];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 6)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[5], &valueTypeLast);

    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    struct AddonDataAnalyzeFirmwareInfo* data = (struct AddonDataAnalyzeFirmwareInfo*)malloc(sizeof(struct AddonDataAnalyzeFirmwareInfo));
    //获取参数
    size_t strLength;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &data->nMajor));
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &data->nMinor));
    catchErrorMsg(env, napi_get_value_int32(env, argv[2], &data->nRev));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    data->strFirmwareInfoFromFOTA = (char*)malloc(sizeof(char) * (++strLength));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], data->strFirmwareInfoFromFOTA, strLength, &strLength));
    catchErrorMsg(env, napi_get_value_int32(env, argv[4], &data->nUpdateBetaFw));
    // 分配内存空间，在work_complete中会释放
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //需要进行手动的malloc，否则访问成员变量work和tsfn会出现错误
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //没有其他参数即设置为null即可
    addon_data->others = data;
    //非分配地址空间参数，即无需释放内存方法
    addon_data->freeOthersFPtr = &freeAnalyzeFirmwareInfoParams;
    //设置调用方法
    addon_data->callSoMehtodFPtr = &callSoAnalyzeFirmwareInfo;
    //初始化
    initWorkTsfn(env, argv[5], addon_data);
    return NULL;
}