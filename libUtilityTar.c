// libUtilityTar
#include <node_api.h>
#include <stdlib.h>
#include "dds-napi-common.h"
#ifdef CACHE_SO_LIB
#else
    #include "libUtilityTar.h"
#endif

// ------------int unTar(char* strFilePath, char* strFileName, char* strDesFilePath);
napi_value unTarSync(napi_env env, napi_callback_info info) {
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
    // strFilePath
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* strFilePath = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], strFilePath, strLength, &strLength));
    // strFileName
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    char* strFileName = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], strFileName, strLength, &strLength));
    // strDesFilePath
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    char* strDesFilePath = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], strDesFilePath, strLength, &strLength));
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->unTar)
    {
        valuePtr = addonDataSo->unTar(strFilePath, strFileName, strDesFilePath);
    }
#else
    valuePtr = unTar(strFilePath, strFileName, strDesFilePath);
#endif
    free(strFilePath);
    free(strFileName);
    free(strDesFilePath);

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

struct AddonDataUnTar
{
    char* strFilePath;
    char* strFileName;
    char* strDesFilePath;
};

void freeParamsUnTar(void* params)
{
    if (params != NULL)
    {
        struct AddonDataUnTar* data = (struct AddonDataUnTar*)params;
        if (data != NULL) {
            if (data->strFilePath != NULL) {
                free(data->strFilePath);
            }
            if (data->strFileName != NULL) {
                free(data->strFileName);
            }
            if (data->strDesFilePath != NULL) {
                free(data->strDesFilePath);
            }
            free(data);
        }
    }
}

struct AddonDataCBInParams* callSoUnTar(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    struct AddonDataUnTar* data = (struct AddonDataUnTar*)params;
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->unTar)
    {
        valuePtr = addonDataSo->unTar(data->strFilePath, data->strFileName, data->strDesFilePath);
    }
#else
    valuePtr = unTar(data->strFilePath, data->strFileName, data->strDesFilePath);
#endif

    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value unTar(napi_env env, napi_callback_info info) {
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
    struct AddonDataUnTar* params = (struct AddonDataUnTar*)malloc(sizeof(struct AddonDataUnTar));
    // strFilePath
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    params->strFilePath = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], params->strFilePath, strLength, &strLength));
    // strFileName
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    params->strFileName = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], params->strFileName, strLength, &strLength));
    // strDesFilePath
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    params->strDesFilePath = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], params->strDesFilePath, strLength, &strLength));

    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = params;
    addon_data->freeOthersFPtr = &freeParamsUnTar;
    // 设置调用so方法指针
    addon_data->callSoMehtodFPtr = &callSoUnTar;

    // 初始化
    initWorkTsfn(env, argv[3], addon_data);
    return NULL;
}