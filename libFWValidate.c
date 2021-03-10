// libFWValidate
#include <stdlib.h>
#include "dds-napi-common.h"
#ifdef CACHE_SO_LIB
#else
    #include "libFWValidate.h"
#endif

//-------int checkHeaderAndPayload(char* strFilePath, char* strFileName);--------
napi_value checkHeaderAndPayloadSync(napi_env env, napi_callback_info info) {
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
    size_t strLength;
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* strFilePath = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], strFilePath, strLength, &strLength));
    // strFileName
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    char* strFileName = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], strFileName, strLength, &strLength));
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->checkHeaderAndPayload)
    {
        valuePtr = addonDataSo->checkHeaderAndPayload(strFilePath, strFileName);
    }
#else
    valuePtr = checkHeaderAndPayload(strFilePath, strFileName);
#endif
    free(strFilePath);
    free(strFileName);

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

struct AddonDataCheckHeaderAndPayload
{
    char* strFilePath;
    char* strFileName;
};

void freeParamsCheckHeaderAndPayload(void* params)
{
    if (params != NULL)
    {
        struct AddonDataCheckHeaderAndPayload* data = (struct AddonDataCheckHeaderAndPayload*)params;
        if (data != NULL) {
            if (data->strFilePath != NULL) {
                free(data->strFilePath);
            }
            if (data->strFileName != NULL) {
                free(data->strFileName);
            }
            free(data);
        }
    }
}

struct AddonDataCBInParams* callSoCheckHeaderAndPayload(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    struct AddonDataCheckHeaderAndPayload* data = (struct AddonDataCheckHeaderAndPayload*)params;
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->checkHeaderAndPayload)
    {
        valuePtr = addonDataSo->checkHeaderAndPayload(data->strFilePath, data->strFileName);
    }
#else
    valuePtr = checkHeaderAndPayload(data->strFilePath, data->strFileName);
#endif

    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value checkHeaderAndPayload(napi_env env, napi_callback_info info) {
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
    size_t strLength;
    struct AddonDataCheckHeaderAndPayload* params = (struct AddonDataCheckHeaderAndPayload*)malloc(sizeof(struct AddonDataCheckHeaderAndPayload));
    // strFilePath
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    params->strFilePath = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], params->strFilePath, strLength, &strLength));
    // strFileName
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    params->strFileName = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], params->strFileName, strLength, &strLength));

    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = params;
    addon_data->freeOthersFPtr = &freeParamsCheckHeaderAndPayload;
    // 设置调用so方法指针
    addon_data->callSoMehtodFPtr = &callSoCheckHeaderAndPayload;

    // 初始化
    initWorkTsfn(env, argv[2], addon_data);
    return NULL;
}