// libCLI
#include <string.h>
#include <stdlib.h>
#include "dds-napi-common.h"
#ifdef CACHE_SO_LIB
#else
    #include "libCLI.h"
#endif

//---------char* getSystemInfo();-------------
napi_value getSystemInfoSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    struct AddonDataSo *addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 执行so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getSystemInfo)
    {
        valuePtr = addonDataSo->getSystemInfo();
    }
    #else
        valuePtr = getSystemInfo();
    #endif
    // so函数返回值
    napi_value methodReult; 
    napi_get_null(env, &methodReult);   
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }   
    // 判断是否有回调函数
    if (argc >= 1) {
        napi_valuetype valueTypeLast;
        napi_typeof(env, argv[0], &valueTypeLast);
        if (valueTypeLast == napi_function) {
            // 构造返回信息
            napi_value errorInfo;
            napi_get_null(env, &errorInfo);
            errorInfo = getErrorMsg(env);
            napi_value callbackParams[2];
            callbackParams[0] = errorInfo;
            callbackParams[1] = methodReult;
            napi_value global;
            napi_get_global(env, &global);
            napi_value returnVal;
            napi_call_function(env, global, argv[0], 2, callbackParams, &returnVal);
        }
    }
    return methodReult;
}

struct AddonDataCBInParams *callSoGetSystemInfo(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams *addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    // 执行so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getSystemInfo)
    {
        valuePtr = addonDataSo->getSystemInfo();
    }
    #else
        valuePtr = getSystemInfo();
    #endif
    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getSystemInfo(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    // 检查参数个数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc < 1)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 判断是否有回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, "EINVAL", "There is not callback function.");
        return NULL;
    }
    // 获取参数   
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = NULL;
    addon_data->freeOthersFPtr = NULL;
    // 设置调用so方法指针
    addon_data->callSoMehtodFPtr = &callSoGetSystemInfo;
    // 初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

//-------------int setAdminPassword(char* password);
napi_value setAdminPasswordSync(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 检查参数个数
    if (argc < 1)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 获取参数
    size_t strLength;
    // password    
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* password = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], password, strLength, &strLength));
    // 执行so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setAdminPassword)
        {
            valuePtr = addonDataSo->setAdminPassword(password);
        }
    #else
        valuePtr = setAdminPassword(password);
    #endif
    free(password);

    // so函数返回值
    napi_value methodReult;
    napi_get_null(env, &methodReult);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    // 判断是否有回调函数
    if (argc >= 2) {
        napi_valuetype valueTypeLast;
        napi_typeof(env, argv[1], &valueTypeLast);
        if (valueTypeLast == napi_function) {
            // 构造返回信息
            napi_value errorInfo;
            napi_get_null(env, &errorInfo);
            errorInfo = getErrorMsg(env);
            napi_value callbackParams[2];
            callbackParams[0] = errorInfo;
            callbackParams[1] = methodReult;
            napi_value global;
            napi_get_global(env, &global);
            napi_value returnVal;
            napi_call_function(env, global, argv[1], 2, callbackParams, &returnVal);
        }
    }
    return methodReult;
}

struct AddonDataSetAdminPassword
{
    char* password;
};

void freeParamsSetAdminPassword(void* params)
{
    if (params != NULL)
    {
        struct AddonDataSetAdminPassword* data = (struct AddonDataSetAdminPassword*)params;
        if (data != NULL) {
            if (data->password != NULL)
            {
                free(data->password);
            }            
            free(data);
        }
    }
}

struct AddonDataCBInParams* callSoSetAdminPassword(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    struct AddonDataSetAdminPassword* data = (struct AddonDataSetAdminPassword*)params;
    // 执行so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setAdminPassword)
        {
            valuePtr = addonDataSo->setAdminPassword(data->password);
        }
    #else
        valuePtr = setAdminPassword(data->password);
    #endif

	addonDataCBInParams->value = malloc(sizeof(int));
	*((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value setAdminPassword(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];    
    // 检查参数个数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc < 2)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 判断是否有回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[1], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, "EINVAL", "There is not callback function.");
        return NULL;
    }
    // 获取参数
    size_t strLength;
    struct AddonDataSetAdminPassword* params = (struct AddonDataSetAdminPassword*)malloc(sizeof(struct AddonDataSetAdminPassword));
    // password    
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    params->password = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], params->password, strLength, &strLength));

    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = params;
    addon_data->freeOthersFPtr = &freeParamsSetAdminPassword;
    // 设置调用so方法指针
    addon_data->callSoMehtodFPtr = &callSoSetAdminPassword;

    // 初始化
    initWorkTsfn(env, argv[1], addon_data);
    return NULL;
}

//------------char* getDeviceName();
napi_value getDeviceNameSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 执行so函数
    char* valuePtr = NULL;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getDeviceName)
    {
        valuePtr = addonDataSo->getDeviceName();
    }
#else
    valuePtr = getDeviceName();
#endif
    // so函数返回值
    napi_value methodReult;
    napi_get_null(env, &methodReult);
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }
    // 判断是否有回调函数
    if (argc >= 1) {
        napi_valuetype valueTypeLast;
        napi_typeof(env, argv[0], &valueTypeLast);
        if (valueTypeLast == napi_function) {
            // 构造返回信息
            napi_value errorInfo;
            napi_get_null(env, &errorInfo);
            errorInfo = getErrorMsg(env);
            napi_value callbackParams[2];
            callbackParams[0] = errorInfo;
            callbackParams[1] = methodReult;
            napi_value global;
            napi_get_global(env, &global);
            napi_value returnVal;
            napi_call_function(env, global, argv[0], 2, callbackParams, &returnVal);
        }
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoGetDeviceName(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    // 执行so函数
    char* valuePtr = NULL;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getDeviceName)
    {
        valuePtr = addonDataSo->getDeviceName();
    }
#else
    valuePtr = getDeviceName();
#endif
    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getDeviceName(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    // 检查参数个数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc < 1)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 判断是否有回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, "EINVAL", "There is not callback function.");
        return NULL;
    }
    // 获取参数   
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = NULL;
    addon_data->freeOthersFPtr = NULL;
    // 设置调用so方法指针
    addon_data->callSoMehtodFPtr = &callSoGetDeviceName;

    // 初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

//------------int setDeviceName(char* strDeviceName);
napi_value setDeviceNameSync(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 检查参数个数
    if (argc < 1)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 获取参数
    size_t strLength;
    // strDeviceName    
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* strDeviceName = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], strDeviceName, strLength, &strLength));
    
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->setDeviceName)
    {
        valuePtr = addonDataSo->setDeviceName(strDeviceName);
    }
#else
    valuePtr = setDeviceName(strDeviceName);
#endif
    free(strDeviceName);

    // so函数返回值
    napi_value methodReult;
    napi_get_null(env, &methodReult);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    // 判断是否有回调函数
    if (argc >= 2) {
        napi_valuetype valueTypeLast;
        napi_typeof(env, argv[1], &valueTypeLast);
        if (valueTypeLast == napi_function) {
            // 构造返回信息
            napi_value errorInfo;
            napi_get_null(env, &errorInfo);
            errorInfo = getErrorMsg(env);
            napi_value callbackParams[2];
            callbackParams[0] = errorInfo;
            callbackParams[1] = methodReult;
            napi_value global;
            napi_get_global(env, &global);
            napi_value returnVal;
            napi_call_function(env, global, argv[1], 2, callbackParams, &returnVal);
        }
    }
    return methodReult;
}

struct AddonDataSetDeviceName
{
    char* strDeviceName;
};

void freeParamsSetDeviceName(void* params)
{
	if (params != NULL)
	{
		struct AddonDataSetDeviceName* data = (struct AddonDataSetDeviceName*)params;
		if (data != NULL) {
            if (data->strDeviceName != NULL)
            {
                free(data->strDeviceName);
            }
			free(data);
		}
	}
}

struct AddonDataCBInParams* callSoSetDeviceName(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    struct AddonDataSetDeviceName* data = (struct AddonDataSetDeviceName*)params;
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->setDeviceName)
    {
        valuePtr = addonDataSo->setDeviceName(data->strDeviceName);
    }
#else
    valuePtr = setDeviceName(data->strDeviceName);
#endif

    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value setDeviceName(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    // 检查参数个数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc < 2)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 判断是否有回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[1], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, "EINVAL", "There is not callback function.");
        return NULL;
    }
    // 获取参数
    size_t strLength;
    struct AddonDataSetDeviceName* params = (struct AddonDataSetDeviceName*)malloc(sizeof(struct AddonDataSetDeviceName));
    // strDeviceName    
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    params->strDeviceName = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], params->strDeviceName, strLength, &strLength));

    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = params;
    addon_data->freeOthersFPtr = &freeParamsSetDeviceName;
    // 调用so方法
    addon_data->callSoMehtodFPtr = &callSoSetDeviceName;

    // 初始化
    initWorkTsfn(env, argv[1], addon_data);
    return NULL;
}

//------------char* getPerformance();
napi_value getPerformanceSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 执行so函数
    char* valuePtr = NULL;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getPerformance)
    {
        valuePtr = addonDataSo->getPerformance();
    }
#else
    valuePtr = getPerformance();
#endif
    // so函数返回值
    napi_value methodReult;
    napi_get_null(env, &methodReult);
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }
    // 判断是否有回调函数
    if (argc >= 1) {
        napi_valuetype valueTypeLast;
        napi_typeof(env, argv[0], &valueTypeLast);
        if (valueTypeLast == napi_function) {
            // 构造返回信息
            napi_value errorInfo;
            napi_get_null(env, &errorInfo);
            errorInfo = getErrorMsg(env);
            napi_value callbackParams[2];
            callbackParams[0] = errorInfo;
            callbackParams[1] = methodReult;
            napi_value global;
            napi_get_global(env, &global);
            napi_value returnVal;
            napi_call_function(env, global, argv[0], 2, callbackParams, &returnVal);
        }
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoGetPerformance(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    // 执行so函数
    char* valuePtr = NULL;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getPerformance)
    {
        valuePtr = addonDataSo->getPerformance();
    }
#else
    valuePtr = getPerformance();
#endif
    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getPerformance(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    // 检查参数个数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc < 1)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 判断是否有回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, "EINVAL", "There is not callback function.");
        return NULL;
    }
    // 获取参数    
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = NULL;
    addon_data->freeOthersFPtr = NULL;
    // 调用so方法
    addon_data->callSoMehtodFPtr = &callSoGetPerformance;

    // 初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

//------------int getWebAccessPort(void);
napi_value getWebAccessPortSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getWebAccessPort)
    {
        valuePtr = addonDataSo->getWebAccessPort();
    }
#else
    valuePtr = getWebAccessPort();
#endif
    // so函数返回值
    napi_value methodReult;
    napi_get_null(env, &methodReult);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    // 判断是否有回调函数
    if (argc >= 1) {
        napi_valuetype valueTypeLast;
        napi_typeof(env, argv[0], &valueTypeLast);
        if (valueTypeLast == napi_function) {
            // 构造返回信息
            napi_value errorInfo;
            napi_get_null(env, &errorInfo);
            errorInfo = getErrorMsg(env);
            napi_value callbackParams[2];
            callbackParams[0] = errorInfo;
            callbackParams[1] = methodReult;
            napi_value global;
            napi_get_global(env, &global);
            napi_value returnVal;
            napi_call_function(env, global, argv[0], 2, callbackParams, &returnVal);
        }
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoGetWebAccessPort(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getWebAccessPort)
    {
        valuePtr = addonDataSo->getWebAccessPort();
    }
#else
    valuePtr = getWebAccessPort();
#endif

    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value getWebAccessPort(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    // 检查参数个数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc < 1)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 判断是否有回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, "EINVAL", "There is not callback function.");
        return NULL;
    }
    // 获取参数   
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = NULL;
    addon_data->freeOthersFPtr = NULL;
    // 调用so方法
    addon_data->callSoMehtodFPtr = &callSoGetWebAccessPort;

    // 初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

//------------int setWebAccessPort(int nPortNumber);
napi_value setWebAccessPortSync(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 检查参数个数
    if (argc < 1)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 获取参数
    // nPortNumber    
    int nPortNumber;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &nPortNumber));
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->setWebAccessPort)
    {
        valuePtr = addonDataSo->setWebAccessPort(nPortNumber);
    }
#else
    valuePtr = setWebAccessPort(nPortNumber);
#endif

    // so函数返回值
    napi_value methodReult;
    napi_get_null(env, &methodReult);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    // 判断是否有回调函数
    if (argc >= 2) {
        napi_valuetype valueTypeLast;
        napi_typeof(env, argv[1], &valueTypeLast);
        if (valueTypeLast == napi_function) {
            // 构造返回信息
            napi_value errorInfo;
            napi_get_null(env, &errorInfo);
            errorInfo = getErrorMsg(env);
            napi_value callbackParams[2];
            callbackParams[0] = errorInfo;
            callbackParams[1] = methodReult;
            napi_value global;
            napi_get_global(env, &global);
            napi_value returnVal;
            napi_call_function(env, global, argv[1], 2, callbackParams, &returnVal);
        }
    }
    return methodReult;
}

struct AddonDataSetWebAccessPort
{
    int nPortNumber;
};

void freeParamsSetWebAccessPort(void* params)
{
    if (params != NULL)
    {
        struct AddonDataSetWebAccessPort* data = (struct AddonDataSetWebAccessPort*)params;
        if (data != NULL) {
            free(data);
        }
    }
}

struct AddonDataCBInParams* callSoSetWebAccessPort(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    struct AddonDataSetWebAccessPort* data = (struct AddonDataSetWebAccessPort*)params;
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->setWebAccessPort)
    {
        valuePtr = addonDataSo->setWebAccessPort(data->nPortNumber);
    }
#else
    valuePtr = setWebAccessPort(data->nPortNumber);
#endif

    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value setWebAccessPort(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    // 检查参数个数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc < 2)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 判断是否有回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[1], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, "EINVAL", "There is not callback function.");
        return NULL;
    }
    // 获取参数
    struct AddonDataSetWebAccessPort* params = (struct AddonDataSetWebAccessPort*)malloc(sizeof(struct AddonDataSetWebAccessPort));
    // nPortNumber
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &params->nPortNumber));

    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = params;
    addon_data->freeOthersFPtr = &freeParamsSetWebAccessPort;
    // 设置调用so函数指针
    addon_data->callSoMehtodFPtr = &callSoSetWebAccessPort;

    // 初始化
    initWorkTsfn(env, argv[1], addon_data);
    return NULL;
}

//------------int getNginxAccessPort();
napi_value getNginxAccessPortSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getNginxAccessPort)
    {
        valuePtr = addonDataSo->getNginxAccessPort();
    }
#else
    valuePtr = getNginxAccessPort();
#endif
    // so函数返回值
    napi_value methodReult;
    napi_get_null(env, &methodReult);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    // 判断是否有回调函数
    if (argc >= 1) {
        napi_valuetype valueTypeLast;
        napi_typeof(env, argv[0], &valueTypeLast);
        if (valueTypeLast == napi_function) {
            // 构造返回信息
            napi_value errorInfo;
            napi_get_null(env, &errorInfo);
            errorInfo = getErrorMsg(env);
            napi_value callbackParams[2];
            callbackParams[0] = errorInfo;
            callbackParams[1] = methodReult;
            napi_value global;
            napi_get_global(env, &global);
            napi_value returnVal;
            napi_call_function(env, global, argv[0], 2, callbackParams, &returnVal);
        }
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoGetNginxAccessPort(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getNginxAccessPort)
    {
        valuePtr = addonDataSo->getNginxAccessPort();
    }
#else
    valuePtr = getNginxAccessPort();
#endif

    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value getNginxAccessPort(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    // 检查参数个数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc < 1)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 判断是否有回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, "EINVAL", "There is not callback function.");
        return NULL;
    }
    // 获取参数   
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = NULL;
    addon_data->freeOthersFPtr = NULL;
    // 设置调用so函数指针
    addon_data->callSoMehtodFPtr = &callSoGetNginxAccessPort;

    // 初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

//------------int setNginxAccessPort(int nPortNumber);
napi_value setNginxAccessPortSync(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 检查参数个数
    if (argc < 1)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 获取参数
    // nPortNumber    
    int nPortNumber;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &nPortNumber));
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->setNginxAccessPort)
    {
        valuePtr = addonDataSo->setNginxAccessPort(nPortNumber);
    }
#else
    valuePtr = setNginxAccessPort(nPortNumber);
#endif

    // so函数返回值
    napi_value methodReult;
    napi_get_null(env, &methodReult);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    // 判断是否有回调函数
    if (argc >= 2) {
        napi_valuetype valueTypeLast;
        napi_typeof(env, argv[1], &valueTypeLast);
        if (valueTypeLast == napi_function) {
            // 构造返回信息
            napi_value errorInfo;
            napi_get_null(env, &errorInfo);
            errorInfo = getErrorMsg(env);
            napi_value callbackParams[2];
            callbackParams[0] = errorInfo;
            callbackParams[1] = methodReult;
            napi_value global;
            napi_get_global(env, &global);
            napi_value returnVal;
            napi_call_function(env, global, argv[1], 2, callbackParams, &returnVal);
        }
    }
    return methodReult;
}

struct AddonDataSetNginxAccessPort
{
    int nPortNumber;
};

void freeParamsSetNginxAccessPort(void* params)
{
    if (params != NULL)
    {
        struct AddonDataSetNginxAccessPort* data = (struct AddonDataSetNginxAccessPort*)params;
        if (data != NULL) {
            free(data);
        }
    }
}

struct AddonDataCBInParams* callSoSetNginxAccessPort(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    struct AddonDataSetNginxAccessPort* data = (struct AddonDataSetNginxAccessPort*)params;
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->setNginxAccessPort)
    {
        valuePtr = addonDataSo->setNginxAccessPort(data->nPortNumber);
    }
#else
    valuePtr = setNginxAccessPort(data->nPortNumber);
#endif

    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value setNginxAccessPort(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    // 检查参数个数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc < 2)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 判断是否有回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[1], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, "EINVAL", "There is not callback function.");
        return NULL;
    }
    // 获取参数
    struct AddonDataSetNginxAccessPort* params = (struct AddonDataSetNginxAccessPort*)malloc(sizeof(struct AddonDataSetNginxAccessPort));
    // nPortNumber
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &params->nPortNumber));

    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = params;
    addon_data->freeOthersFPtr = &freeParamsSetNginxAccessPort;
    // 设置调用so函数指针
    addon_data->callSoMehtodFPtr = &callSoSetNginxAccessPort;

    // 初始化
    initWorkTsfn(env, argv[1], addon_data);
    return NULL;
}

//-------------char* getNvrVersion();
napi_value getNvrVersionSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 执行so函数
    char* valuePtr = NULL;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getNvrVersion)
    {
        valuePtr = addonDataSo->getNvrVersion();
    }
#else
    valuePtr = getNvrVersion();
#endif
    // so函数返回值
    napi_value methodReult;
    napi_get_null(env, &methodReult);
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }
    // 判断是否有回调函数
    if (argc >= 1) {
        napi_valuetype valueTypeLast;
        napi_typeof(env, argv[0], &valueTypeLast);
        if (valueTypeLast == napi_function) {
            // 构造返回信息
            napi_value errorInfo;
            napi_get_null(env, &errorInfo);
            errorInfo = getErrorMsg(env);
            napi_value callbackParams[2];
            callbackParams[0] = errorInfo;
            callbackParams[1] = methodReult;
            napi_value global;
            napi_get_global(env, &global);
            napi_value returnVal;
            napi_call_function(env, global, argv[0], 2, callbackParams, &returnVal);
        }
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoGetNvrVersion(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    // 执行so函数
    char* valuePtr = NULL;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getNvrVersion)
    {
        valuePtr = addonDataSo->getNvrVersion();
    }
#else
    valuePtr = getNvrVersion();
#endif
    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getNvrVersion(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    // 检查参数个数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc < 1)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 判断是否有回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, "EINVAL", "There is not callback function.");
        return NULL;
    }
    // 获取参数   
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = NULL;
    addon_data->freeOthersFPtr = NULL;
    // 调用so方法
    addon_data->callSoMehtodFPtr = &callSoGetNvrVersion;

    // 初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

//-------------int setMACAddress(char* macAddress); deprecated