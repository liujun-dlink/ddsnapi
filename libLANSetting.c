#include <node_api.h>
#include <string.h>
#include <stdlib.h>
#include "dds-napi-common.h"
#ifdef CACHE_SO_LIB
#else
    #include "libLANSetting.h"
#endif

//--int setLANSetting(int isPermanent,int isDHCP,char* argIpAddress,char* argNetmask,char* argGateway,int nDnsType,char* argDNS1,char* argDNS2,int flagUpdateDaa);
napi_value setLANSettingSync(napi_env env, napi_callback_info info) {
    size_t argc = 10;
    napi_value argv[10];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 检查参数个数
    if (argc < 9)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 获取参数
    size_t strLength;
    //int isPermanent,
    int isPermanent;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &isPermanent));
    //    int isDHCP,
    int isDHCP;
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &isDHCP));
    //    char* argIpAddress
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    char* argIpAddress = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], argIpAddress, strLength, &strLength));
    //    char* argNetmask
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    char* argNetmask = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], argNetmask, strLength, &strLength));
    //    char* argGateway
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[4], NULL, NULL, &strLength));
    char* argGateway = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[4], argGateway, strLength, &strLength));
    //    int nDnsType
    int nDnsType;
    catchErrorMsg(env, napi_get_value_int32(env, argv[5], &nDnsType));
    //    char* argDNS1
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[6], NULL, NULL, &strLength));
    char* argDNS1 = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[6], argDNS1, strLength, &strLength));
    //    char* argDNS2
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[7], NULL, NULL, &strLength));
    char* argDNS2 = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[7], argDNS2, strLength, &strLength));
    //    int flagUpdateDaa
    int flagUpdateDaa;
    catchErrorMsg(env, napi_get_value_int32(env, argv[8], &flagUpdateDaa));
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->setLANSetting)
    {
        valuePtr = addonDataSo->setLANSetting(isPermanent, isDHCP, argIpAddress, argNetmask, argGateway, nDnsType, argDNS1, argDNS2, flagUpdateDaa);
    }
#else
    valuePtr = setLANSetting(isPermanent, isDHCP, argIpAddress, argNetmask, argGateway, nDnsType, argDNS1, argDNS2, flagUpdateDaa);
#endif
    free(argIpAddress);
    free(argNetmask);
    free(argGateway);
    free(argDNS1);
    free(argDNS2);

    // so函数返回值
    napi_value methodReult;
    napi_get_null(env, &methodReult);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    // 判断是否有回调函数
    if (argc >= 10) {
        napi_valuetype valueTypeLast;
        napi_typeof(env, argv[9], &valueTypeLast);
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
            napi_call_function(env, global, argv[9], 2, callbackParams, &returnVal);
        }
    }
    return methodReult;
}

struct AddonDataSetLANSetting
{
    int isPermanent;
    int isDHCP;
    char* argIpAddress;
    char* argNetmask;
    char* argGateway;
    int nDnsType;
    char* argDNS1;
    char* argDNS2;
    int flagUpdateDaa;
};

void freeParamsSetLANSetting(void* params)
{
    if (params != NULL)
    {
        struct AddonDataSetLANSetting* data = (struct AddonDataSetLANSetting*)params;
        if (data != NULL) {
            if (data->argIpAddress != NULL) {
                free(data->argIpAddress);
            }
            if (data->argNetmask != NULL) {
                free(data->argNetmask);
            }
            if (data->argGateway != NULL) {
                free(data->argGateway);
            }
            if (data->argDNS1 != NULL) {
                free(data->argDNS1);
            }
            if (data->argDNS2 != NULL) {
                free(data->argDNS2);
            }
            free(data);
        }
    }
}

struct AddonDataCBInParams* callSoSetLANSetting(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    struct AddonDataSetLANSetting* data = (struct AddonDataSetLANSetting*)params;
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->setLANSetting)
    {
        valuePtr = addonDataSo->setLANSetting(data->isPermanent, data->isDHCP, data->argIpAddress, data->argNetmask,
            data->argGateway, data->nDnsType, data->argDNS1, data->argDNS2, data->flagUpdateDaa);
    }
#else
    valuePtr = setLANSetting(data->isPermanent, data->isDHCP, data->argIpAddress, data->argNetmask,
        data->argGateway, data->nDnsType, data->argDNS1, data->argDNS2, data->flagUpdateDaa);
#endif

    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value setLANSetting(napi_env env, napi_callback_info info) {
    size_t argc = 10;
    napi_value argv[10];
    // 检查参数个数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc < 10)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 判断是否有回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[9], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, "EINVAL", "There is not callback function.");
        return NULL;
    }
    // 获取参数
    size_t strLength;
    struct AddonDataSetLANSetting* params = (struct AddonDataSetLANSetting*)malloc(sizeof(struct AddonDataSetLANSetting));
    //int isPermanent
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &params->isPermanent));
    // int isDHCP
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &params->isDHCP));
    // char* argIpAddress
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    params->argIpAddress = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], params->argIpAddress, strLength, &strLength));
    // char* argNetmask
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    params->argNetmask = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], params->argNetmask, strLength, &strLength));
    // char* argGateway
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[4], NULL, NULL, &strLength));
    params->argGateway = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[4], params->argGateway, strLength, &strLength));
    // int nDnsType
    catchErrorMsg(env, napi_get_value_int32(env, argv[5], &params->nDnsType));
    // char* argDNS1
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[6], NULL, NULL, &strLength));
    params->argDNS1 = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[6], params->argDNS1, strLength, &strLength));
    // char* argDNS2
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[7], NULL, NULL, &strLength));
    params->argDNS2 = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[7], params->argDNS2, strLength, &strLength));
    // int flagUpdateDaa
    catchErrorMsg(env, napi_get_value_int32(env, argv[8], &params->flagUpdateDaa));

    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = params;
    addon_data->freeOthersFPtr = &freeParamsSetLANSetting;
    // 设置调用so方法指针
    addon_data->callSoMehtodFPtr = &callSoSetLANSetting;

    // 初始化
    initWorkTsfn(env, argv[9], addon_data);
    return NULL;
}

//-----char* getLANSetting();
napi_value getLANSettingSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 执行so函数
    char* valuePtr = NULL;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getLANSetting)
    {
        valuePtr = addonDataSo->getLANSetting();
    }
#else
    valuePtr = getLANSetting();
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

struct AddonDataCBInParams* callSoGetLANSetting(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    // 执行so函数
    char* valuePtr = NULL;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getLANSetting)
    {
        valuePtr = addonDataSo->getLANSetting();
    }
#else
    valuePtr = getLANSetting();
#endif
    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getLANSetting(napi_env env, napi_callback_info info) {
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
    addon_data->callSoMehtodFPtr = &callSoGetLANSetting;

    // 初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

/*int setLANSettingIpv6(int isPermanent,
    char* argProvision,
    char* argIpv6Address,
    int nPrefixLength,
    char* argGateway,
    int nDnsType,
    char* argDns1,
    char* argDns2,
    int flagUpdateDaa
);*/
napi_value setLANSettingIpv6Sync(napi_env env, napi_callback_info info) {
    size_t argc = 10;
    napi_value argv[10];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 检查参数个数
    if (argc < 9)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 获取参数
    size_t strLength;
    //int isPermanent,
    int isPermanent;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &isPermanent));
    // char* argProvision
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    char* argProvision = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], argProvision, strLength, &strLength));
    // char* argIpv6Address
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    char* argIpv6Address = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], argIpv6Address, strLength, &strLength));
    // int nPrefixLength,
    int nPrefixLength;
    catchErrorMsg(env, napi_get_value_int32(env, argv[3], &nPrefixLength));
    // char* argGateway
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[4], NULL, NULL, &strLength));
    char* argGateway = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[4], argGateway, strLength, &strLength));
    // int nDnsType
    int nDnsType;
    catchErrorMsg(env, napi_get_value_int32(env, argv[5], &nDnsType));
    // char* argDns1
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[6], NULL, NULL, &strLength));
    char* argDns1 = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[6], argDns1, strLength, &strLength));
    // char* argDns2
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[7], NULL, NULL, &strLength));
    char* argDns2 = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[7], argDns2, strLength, &strLength));
    // int flagUpdateDaa
    int flagUpdateDaa;
    catchErrorMsg(env, napi_get_value_int32(env, argv[8], &flagUpdateDaa));
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->setLANSettingIpv6)
    {
        valuePtr = addonDataSo->setLANSettingIpv6(isPermanent, argProvision, argIpv6Address, nPrefixLength,
            argGateway, nDnsType, argDns1, argDns2, flagUpdateDaa);
    }
#else
    valuePtr = setLANSettingIpv6(isPermanent, argProvision, argIpv6Address, nPrefixLength, argGateway,
        nDnsType, argDns1, argDns2, flagUpdateDaa);
#endif
    free(argProvision);
    free(argIpv6Address);
    free(argGateway);
    free(argDns1);
    free(argDns2);

    // so函数返回值
    napi_value methodReult;
    napi_get_null(env, &methodReult);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    // 判断是否有回调函数
    if (argc >= 10) {
        napi_valuetype valueTypeLast;
        napi_typeof(env, argv[9], &valueTypeLast);
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
            napi_call_function(env, global, argv[9], 2, callbackParams, &returnVal);
        }
    }
    return methodReult;
}

struct AddonDataSetLANSettingIpv6
{
    int isPermanent;
    char* argProvision;
    char* argIpv6Address;
    int nPrefixLength;
    char* argGateway;
    int nDnsType;
    char* argDns1;
    char* argDns2;
    int flagUpdateDaa;
};

void freeParamsSetLANSettingIpv6(void* params)
{
    if (params != NULL)
    {
        struct AddonDataSetLANSettingIpv6* data = (struct AddonDataSetLANSettingIpv6*)params;
        if (data != NULL) {
            if (data->argProvision != NULL) {
                free(data->argProvision);
            }
            if (data->argIpv6Address != NULL) {
                free(data->argIpv6Address);
            }
            if (data->argGateway != NULL) {
                free(data->argGateway);
            }
            if (data->argDns1 != NULL) {
                free(data->argDns1);
            }
            if (data->argDns2 != NULL) {
                free(data->argDns2);
            }
            free(data);
        }
    }
}

struct AddonDataCBInParams* callSoSetLANSettingIpv6(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    struct AddonDataSetLANSettingIpv6* data = (struct AddonDataSetLANSettingIpv6*)params;
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->setLANSettingIpv6)
    {
        valuePtr = addonDataSo->setLANSettingIpv6(data->isPermanent, data->argProvision, data->argIpv6Address, data->nPrefixLength,
            data->argGateway, data->nDnsType, data->argDns1, data->argDns2, data->flagUpdateDaa);
    }
#else
    valuePtr = setLANSettingIpv6(data->isPermanent, data->argProvision, data->argIpv6Address, data->nPrefixLength,
        data->argGateway, data->nDnsType, data->argDns1, data->argDns2, data->flagUpdateDaa);
#endif

    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value setLANSettingIpv6(napi_env env, napi_callback_info info) {
    size_t argc = 10;
    napi_value argv[10];
    // 检查参数个数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc < 10)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 判断是否有回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[9], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, "EINVAL", "There is not callback function.");
        return NULL;
    }
    // 获取参数
    size_t strLength;
    struct AddonDataSetLANSettingIpv6* params = (struct AddonDataSetLANSettingIpv6*)malloc(sizeof(struct AddonDataSetLANSettingIpv6));
    //int isPermanent
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &params->isPermanent));
    // char* argProvision
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    params->argProvision = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], params->argProvision, strLength, &strLength));
    // char* argIpv6Address
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    params->argIpv6Address = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], params->argIpv6Address, strLength, &strLength));
    // int nPrefixLength
    catchErrorMsg(env, napi_get_value_int32(env, argv[3], &params->nPrefixLength));
    // char* argGateway
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[4], NULL, NULL, &strLength));
    params->argGateway = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[4], params->argGateway, strLength, &strLength));
    // int nDnsType
    catchErrorMsg(env, napi_get_value_int32(env, argv[5], &params->nDnsType));
    // char* argDns1
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[6], NULL, NULL, &strLength));
    params->argDns1 = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[6], params->argDns1, strLength, &strLength));
    // char* argDns2
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[7], NULL, NULL, &strLength));
    params->argDns2 = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[7], params->argDns2, strLength, &strLength));
    // int flagUpdateDaa
    catchErrorMsg(env, napi_get_value_int32(env, argv[8], &params->flagUpdateDaa));

    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = params;
    addon_data->freeOthersFPtr = &freeParamsSetLANSettingIpv6;
    // 设置调用so方法指针
    addon_data->callSoMehtodFPtr = &callSoSetLANSettingIpv6;

    // 初始化
    initWorkTsfn(env, argv[9], addon_data);
    return NULL;
}

//----char* getLANSettingIpv6();
napi_value getLANSettingIpv6Sync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 执行so函数
    char* valuePtr = NULL;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getLANSettingIpv6)
    {
        valuePtr = addonDataSo->getLANSettingIpv6();
    }
#else
    valuePtr = getLANSettingIpv6();
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

struct AddonDataCBInParams* callSoGetLANSettingIpv6(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    // 执行so函数
    char* valuePtr = NULL;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->getLANSettingIpv6)
    {
        valuePtr = addonDataSo->getLANSettingIpv6();
    }
#else
    valuePtr = getLANSettingIpv6();
#endif
    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getLANSettingIpv6(napi_env env, napi_callback_info info) {
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
    addon_data->callSoMehtodFPtr = &callSoGetLANSettingIpv6;

    // 初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}
