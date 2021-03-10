// libConsoleSetting
#include <string.h>
#include <stdlib.h>
#include "dds-napi-common.h"
#ifdef CACHE_SO_LIB
#else
    #include "libConsoleSetting.h"
#endif

//------int setConsoleSetting(int enableConsole, char* strProtocol,	int nTimeoutSec);
napi_value setConsoleSettingSync(napi_env env, napi_callback_info info) {
    size_t argc = 4;
    napi_value argv[4];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 检查参数个数
    if (argc < 3)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 获取参数
    size_t strLength;
    // enableConsole    
    int enableConsole;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &enableConsole));
    // strProtocol
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    char* strProtocol = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], strProtocol, strLength, &strLength));
    // nTimeoutSec    
    int nTimeoutSec;
    catchErrorMsg(env, napi_get_value_int32(env, argv[2], &nTimeoutSec));
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->setConsoleSetting)
    {
        valuePtr = addonDataSo->setConsoleSetting(enableConsole, strProtocol, nTimeoutSec);
    }
#else
    valuePtr = setConsoleSetting(enableConsole, strProtocol, nTimeoutSec);
#endif
    free(strProtocol);

    // so函数返回值
    napi_value methodReult;
    napi_get_null(env, &methodReult);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    // 判断是否有回调函数
    if (argc >= 4) {
        napi_valuetype valueTypeLast;
        napi_typeof(env, argv[3], &valueTypeLast);
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
            napi_call_function(env, global, argv[3], 2, callbackParams, &returnVal);
        }
    }
    return methodReult;
}

struct AddonDataSetConsoleSetting
{
    int enableConsole;
    char* strProtocol;
    int nTimeoutSec;
};

void freeParamsSetConsoleSetting(void* params)
{
    if (params != NULL)
    {
        struct AddonDataSetConsoleSetting* data = (struct AddonDataSetConsoleSetting*)params;
        if (data != NULL) {
            if (data->strProtocol != NULL) {
                free(data->strProtocol);
            }            
            free(data);
        }
    }
}

struct AddonDataCBInParams* callSoSetConsoleSetting(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    struct AddonDataSetConsoleSetting* data = (struct AddonDataSetConsoleSetting*)params;
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->setConsoleSetting)
    {
        valuePtr = addonDataSo->setConsoleSetting(data->enableConsole, data->strProtocol, data->nTimeoutSec);
    }
#else
    valuePtr = setConsoleSetting(data->enableConsole, data->strProtocol, data->nTimeoutSec);
#endif

    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value setConsoleSetting(napi_env env, napi_callback_info info) {
    size_t argc = 4;
    napi_value argv[4];
    // 检查参数个数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc < 4)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 判断是否有回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[3], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, "EINVAL", "There is not callback function.");
        return NULL;
    }
    // 获取参数
    size_t strLength;
    struct AddonDataSetConsoleSetting* params = (struct AddonDataSetConsoleSetting*)malloc(sizeof(struct AddonDataSetConsoleSetting));
    // enableConsole
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &params->enableConsole));
    // strProtocol
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    params->strProtocol = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], params->strProtocol, strLength, &strLength));
    // nTimeoutSec
    catchErrorMsg(env, napi_get_value_int32(env, argv[2], &params->nTimeoutSec));

    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = params;
    addon_data->freeOthersFPtr = &freeParamsSetConsoleSetting;
    // 设置调用so方法指针
    addon_data->callSoMehtodFPtr = &callSoSetConsoleSetting;

    // 初始化
    initWorkTsfn(env, argv[3], addon_data);
    return NULL;
}

//----------char* getConsoleSetting(void);
napi_value getConsoleSettingSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 执行so函数
    char* valuePtr = NULL;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getConsoleSetting)
    {
        valuePtr = addonDataSo->getConsoleSetting();
    }
#else
    valuePtr = getConsoleSetting();
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

struct AddonDataCBInParams* callSoGetConsoleSetting(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    // 执行so函数
    char* valuePtr = NULL;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getConsoleSetting)
    {
        valuePtr = addonDataSo->getConsoleSetting();
    }
#else
    valuePtr = getConsoleSetting();
#endif
    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getConsoleSetting(napi_env env, napi_callback_info info) {
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
    addon_data->callSoMehtodFPtr = &callSoGetConsoleSetting;

    // 初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}