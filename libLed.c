// libLed
#include <node_api.h>
#include <stdlib.h>
#include "dds-napi-common.h"
#ifdef CACHE_SO_LIB
#else
    #include "libLed.h"
#endif

//------------int setPowerLed(int ledColor1, int ledColor2);--------------
napi_value setPowerLedSync(napi_env env, napi_callback_info info) {
    size_t argc = 3;
    napi_value argv[3];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 检查参数个数
    if (argc < 2)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 获取参数
    // ledColor1    
    int ledColor1;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &ledColor1));
    // ledColor2
    int ledColor2;
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &ledColor2));
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->setPowerLed)
    {
        valuePtr = addonDataSo->setPowerLed(ledColor1, ledColor2);
    }
#else
    valuePtr = setPowerLed(ledColor1, ledColor2);
#endif

    // so函数返回值
    napi_value methodReult;
    napi_get_null(env, &methodReult);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    // 判断是否有回调函数
    if (argc >= 3) {
        napi_valuetype valueTypeLast;
        napi_typeof(env, argv[2], &valueTypeLast);
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
            napi_call_function(env, global, argv[2], 2, callbackParams, &returnVal);
        }
    }
    return methodReult;
}

struct AddonDataSetPowerLed
{
    int ledColor1;
    int ledColor2;
};

void freeParamsSetPowerLed(void* params)
{
    if (params != NULL)
    {
        struct AddonDataSetPowerLed* data = (struct AddonDataSetPowerLed*)params;
        if (data != NULL) {
            free(data);
        }
    }
}

struct AddonDataCBInParams* callSoSetPowerLed(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    struct AddonDataSetPowerLed* data = (struct AddonDataSetPowerLed*)params;
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->setPowerLed)
    {
        valuePtr = addonDataSo->setPowerLed(data->ledColor1, data->ledColor2);
    }
#else
    valuePtr = setPowerLed(data->ledColor1, data->ledColor2);
#endif

    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value setPowerLed(napi_env env, napi_callback_info info) {
    size_t argc = 3;
    napi_value argv[3];
    // 检查参数个数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc < 3)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 判断是否有回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[2], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, "EINVAL", "There is not callback function.");
        return NULL;
    }
    // 获取参数
    struct AddonDataSetPowerLed* params = (struct AddonDataSetPowerLed*)malloc(sizeof(struct AddonDataSetPowerLed));
    // ledColor1
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &params->ledColor1));
    // ledColor2
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &params->ledColor2));

    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = params;
    addon_data->freeOthersFPtr = &freeParamsSetPowerLed;
    // 设置调用so方法指针
    addon_data->callSoMehtodFPtr = &callSoSetPowerLed;

    // 初始化
    initWorkTsfn(env, argv[2], addon_data);
    return NULL;
}

//-------------int setSsoLed(int nSsoMode);---------------------
napi_value setSsoLedSync(napi_env env, napi_callback_info info) {
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
    // nSsoMode    
    int nSsoMode;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &nSsoMode));
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->setSsoLed)
    {
        valuePtr = addonDataSo->setSsoLed(nSsoMode);
    }
#else
    valuePtr = setSsoLed(nSsoMode);
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

struct AddonDataSetSsoLed
{
    int nSsoMode;
};

void freeParamsSetSsoLed(void* params)
{
    if (params != NULL)
    {
        struct AddonDataSetSsoLed* data = (struct AddonDataSetSsoLed*)params;
        if (data != NULL) {
            free(data);
        }
    }
}

struct AddonDataCBInParams* callSoSetSsoLed(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    struct AddonDataSetSsoLed* data = (struct AddonDataSetSsoLed*)params;
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->setSsoLed)
    {
        valuePtr = addonDataSo->setSsoLed(data->nSsoMode);
    }
#else
    valuePtr = setSsoLed(data->nSsoMode);
#endif

    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value setSsoLed(napi_env env, napi_callback_info info) {
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
    struct AddonDataSetSsoLed* params = (struct AddonDataSetSsoLed*)malloc(sizeof(struct AddonDataSetSsoLed));
    // nSsoMode    
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &params->nSsoMode));

    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = params;
    addon_data->freeOthersFPtr = &freeParamsSetSsoLed;
    // 设置调用so方法指针
    addon_data->callSoMehtodFPtr = &callSoSetSsoLed;

    // 初始化
    initWorkTsfn(env, argv[1], addon_data);
    return NULL;
}