// libSystem
#include <node_api.h>
#include <string.h>
#include <stdlib.h>
#include "dds-napi-common.h"
#ifdef CACHE_SO_LIB
#else
    #include "libSystem.h"
#endif

//int runMdns();
//int runDnssd();
//-------------int getFotaStatus(void);----------------
napi_value getFotaStatusSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getFotaStatus)
    {
        valuePtr = addonDataSo->getFotaStatus();
    }
#else
    valuePtr = getFotaStatus();
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

struct AddonDataCBInParams* callSoGetFotaStatus(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getFotaStatus)
    {
        valuePtr = addonDataSo->getFotaStatus();
    }
#else
    valuePtr = getFotaStatus();
#endif

    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value getFotaStatus(napi_env env, napi_callback_info info) {
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
    addon_data->callSoMehtodFPtr = &callSoGetFotaStatus;

    // 初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}