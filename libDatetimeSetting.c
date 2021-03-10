#include <string.h>
#include "dds-napi-common.h"
#ifdef CACHE_SO_LIB
#else
	#include "libDatetimeSetting.h"
#endif

napi_value getCityDatetimeSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getCityDatetime)
        {
            valuePtr = addonDataSo->getCityDatetime();
        }
    #else
        valuePtr = getCityDatetime();
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

struct AddonDataCBInParams* callSoGetCityDatetime(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getCityDatetime)
        {
            valuePtr = addonDataSo->getCityDatetime();
        }
    #else
        valuePtr = getCityDatetime();
    #endif

    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getCityDatetime(napi_env env, napi_callback_info info) {
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
    addon_data->callSoMehtodFPtr = &callSoGetCityDatetime;
    //初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

//------------int setCityDatetime();

struct AddonDataSetCityDatetime
{
    char* strCityId;
    int isDstEnabled;
    int offsetSeconds;
    int startMonth;
    int startWeek;
    int startDay;
    char* startTime;
    int endMonth;
    int endWeek;
    int endDay;
    char* endTime;
};

//释放others参数的内存
void freeSetCityDatetimeParams(void* params)
{
    struct AddonDataSetCityDatetime* data = (struct AddonDataSetCityDatetime*)params;
    free(data->strCityId);
    free(data->startTime);
    free(data->endTime);
    free(data);
}

napi_value setCityDatetimeSync(napi_env env, napi_callback_info info) {
    size_t argc = 12;
    napi_value argv[12];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 12)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    //获取参数
    size_t strLength;
    //strCityId
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* strCityId = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], strCityId, strLength, &strLength));
    //isDstEnabled
    int isDstEnabled = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &isDstEnabled));
    //offsetSeconds
    int offsetSeconds = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[2], &offsetSeconds));
    //startMonth
    int startMonth = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[3], &startMonth));
    //startWeek
    int startWeek = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[4], &startWeek));
    //startDay
    int startDay = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[5], &startDay));
    //strTimezone
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[6], NULL, NULL, &strLength));
    char* startTime = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[6], startTime, strLength, &strLength));
    //endMonth
    int endMonth = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[7], &endMonth));
    //endWeek
    int endWeek = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[8], &endWeek));
    //endDay
    int endDay = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[9], &endDay));
    //endTime
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[10], NULL, NULL, &strLength));
    char* endTime = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[10], endTime, strLength, &strLength));
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setCityDatetime)
        {
            valuePtr = addonDataSo->setCityDatetime(strCityId, isDstEnabled, offsetSeconds, startMonth, startWeek, startDay, startTime, endMonth, endWeek, endDay, endTime);
        }
    #else
        valuePtr = setCityDatetime(strCityId, isDstEnabled, offsetSeconds, startMonth, startWeek, startDay, startTime, endMonth, endWeek, endDay, endTime);
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
    napi_typeof(env, argv[11], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[11], 2, callbackParams, &returnVal);
    }
    free(strCityId);
    free(startTime);
    free(endTime);
    return methodReult;
}

struct AddonDataCBInParams* callSoSetCityDatetime(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    //获取传参
    addonDataCBInParams->value = NULL;
    struct AddonDataSetCityDatetime* data = (struct AddonDataSetCityDatetime*)params;
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setCityDatetime)
        {
            valuePtr = addonDataSo->setCityDatetime(data->strCityId, data->isDstEnabled, data->offsetSeconds, data->startMonth, data->startWeek, data->startDay, data->startTime,
                data->endMonth, data->endWeek, data->endDay, data->endTime);
        }
    #else
        valuePtr = setCityDatetime(data->strCityId, data->isDstEnabled, data->offsetSeconds, data->startMonth, data->startWeek, data->startDay, data->startTime,
            data->endMonth, data->endWeek, data->endDay, data->endTime);
    #endif
    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;

    return addonDataCBInParams;
}

napi_value setCityDatetime(napi_env env, napi_callback_info info) {
    size_t argc = 12;
    napi_value argv[12];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 12)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[11], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    //获取参数
    size_t strLength;
    struct AddonDataSetCityDatetime* data = (struct AddonDataSetCityDatetime*)malloc(sizeof(struct AddonDataSetCityDatetime));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    data->strCityId = (char*)malloc(sizeof(char) * (++strLength));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], data->strCityId, strLength, &strLength));
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &data->isDstEnabled));
    catchErrorMsg(env, napi_get_value_int32(env, argv[2], &data->offsetSeconds));
    catchErrorMsg(env, napi_get_value_int32(env, argv[3], &data->startMonth));
    catchErrorMsg(env, napi_get_value_int32(env, argv[4], &data->startWeek));
    catchErrorMsg(env, napi_get_value_int32(env, argv[5], &data->startDay));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[6], NULL, NULL, &strLength));
    data->startTime = (char*)malloc(sizeof(char) * (++strLength));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[6], data->startTime, strLength, &strLength));
    catchErrorMsg(env, napi_get_value_int32(env, argv[7], &data->endMonth));
    catchErrorMsg(env, napi_get_value_int32(env, argv[8], &data->endWeek));
    catchErrorMsg(env, napi_get_value_int32(env, argv[9], &data->endDay));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[10], NULL, NULL, &strLength));
    data->endTime = (char*)malloc(sizeof(char) * (++strLength));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[10], data->endTime, strLength, &strLength));
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
    addon_data->freeOthersFPtr = &freeSetCityDatetimeParams;
    //设置调用方法
    addon_data->callSoMehtodFPtr = &callSoSetCityDatetime;
    //初始化
    initWorkTsfn(env, argv[11], addon_data);
    return NULL;
}

// ----char* getCityDst

//释放others参数的内存
void freeGetCityDstSyncParams(void* params)
{
    free((char *)params);
}

napi_value getCityDstSync(napi_env env, napi_callback_info info) {
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
    //获取参数
    size_t strLength;
    //strCityId
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* strCityId = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], strCityId, strLength, &strLength));
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getCityDst)
        {
            valuePtr = addonDataSo->getCityDst(strCityId);
        }
    #else
        valuePtr = getCityDst(strCityId);
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
    napi_typeof(env, argv[1], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[1], 2, callbackParams, &returnVal);
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoGetCityDst(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    char* strCityId = (char*)params;
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getCityDst)
        {
            valuePtr = addonDataSo->getCityDst(strCityId);
        }
    #else
        valuePtr = getCityDst(strCityId);
    #endif

    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getCityDst(napi_env env, napi_callback_info info) {
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
    //获取参数
    size_t strLength;
    //strCityId
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* strCityId = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], strCityId, strLength, &strLength));
    // 分配内存空间，在work_complete中会释放
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //需要进行手动的malloc，否则访问成员变量work和tsfn会出现错误
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //没有其他参数即设置为null即可
    addon_data->others = strCityId;
    //非分配地址空间参数，即无需释放内存方法
    addon_data->freeOthersFPtr = &freeGetCityDstSyncParams;
    addon_data->callSoMehtodFPtr = &callSoGetCityDst;
    //初始化
    initWorkTsfn(env, argv[1], addon_data);
    return NULL;
}

// ----------------char* getDatetime()
napi_value getDatetimeSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getDatetime)
        {
            valuePtr = addonDataSo->getDatetime();
        }
    #else
        valuePtr = getDatetime();
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

struct AddonDataCBInParams* callSoGetDatetime(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getDatetime)
        {
            valuePtr = addonDataSo->getDatetime();
        }
    #else
        valuePtr = getDatetime();
    #endif

    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getDatetime(napi_env env, napi_callback_info info) {
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
    addon_data->callSoMehtodFPtr = &callSoGetDatetime;
    //初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

// ----------------char* getTimezone()
napi_value getTimezoneSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getTimezone)
        {
            valuePtr = addonDataSo->getTimezone();
        }
    #else
        valuePtr = getTimezone();
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

struct AddonDataCBInParams* callSoGetTimezone(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getTimezone)
        {
            valuePtr = addonDataSo->getTimezone();
        }
    #else
        valuePtr = getTimezone();
    #endif

    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getTimezone(napi_env env, napi_callback_info info) {
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
    addon_data->callSoMehtodFPtr = &callSoGetTimezone;
    //初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

// ----------------char* getDaylightSaving()
napi_value getDaylightSavingSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getDaylightSaving)
        {
            valuePtr = addonDataSo->getDaylightSaving();
        }
    #else
        valuePtr = getDaylightSaving();
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

struct AddonDataCBInParams* callSoGetDaylightSaving(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //调用so函数
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getDaylightSaving)
        {
            valuePtr = addonDataSo->getDaylightSaving();
        }
    #else
        valuePtr = getDaylightSaving();
    #endif

    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getDaylightSaving(napi_env env, napi_callback_info info) {
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
    addon_data->callSoMehtodFPtr = &callSoGetDaylightSaving;
    //初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

//------------int setDatetime();

struct AddonDataSetDatetime
{
    int isPermanent;
    int enableNTPServer;
    char* strServerAddress;
    char* strDatetime;
};

//释放others参数的内存
void freeSetDatetimeParams(void* params)
{
    struct AddonDataSetDatetime* data = (struct AddonDataSetDatetime*)params;
    free(data->strServerAddress);
    free(data->strDatetime);
    free(data);
}

napi_value setDatetimeSync(napi_env env, napi_callback_info info) {
    size_t argc = 5;
    napi_value argv[5];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 5)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    //获取参数
    size_t strLength;
    //isPermanent
    int isPermanent = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &isPermanent));
    //enableNTPServer
    int enableNTPServer = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &enableNTPServer));
    //strServerAddress
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    char* strServerAddress = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], strServerAddress, strLength, &strLength));
    //strDatetime
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    char* strDatetime = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], strDatetime, strLength, &strLength));
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setDatetime)
        {
            valuePtr = addonDataSo->setDatetime(isPermanent, enableNTPServer, strServerAddress, strDatetime);
        }
    #else
        valuePtr = setDatetime(isPermanent, enableNTPServer, strServerAddress, strDatetime);
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
    napi_typeof(env, argv[4], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[4], 2, callbackParams, &returnVal);
    }
    free(strServerAddress);
    free(strDatetime);
    return methodReult;
}

struct AddonDataCBInParams* callSoSetDatetime(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    //获取传参
    addonDataCBInParams->value = NULL;
    struct AddonDataSetDatetime* data = (struct AddonDataSetDatetime*)params;
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setDatetime)
        {
            valuePtr = addonDataSo->setDatetime(data->isPermanent, data->enableNTPServer, data->strServerAddress, data->strDatetime);
        }
    #else
        valuePtr = setDatetime(data->isPermanent, data->enableNTPServer, data->strServerAddress, data->strDatetime);
    #endif
    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;

    return addonDataCBInParams;
}

napi_value setDatetime(napi_env env, napi_callback_info info) {
    size_t argc = 5;
    napi_value argv[5];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 5)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[4], &valueTypeLast);

    if (valueTypeLast != napi_function) {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    struct AddonDataSetDatetime* data = (struct AddonDataSetDatetime*)malloc(sizeof(struct AddonDataSetDatetime));
    //获取参数
    size_t strLength;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &data->isPermanent));
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &data->enableNTPServer));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    data->strServerAddress = (char*)malloc(sizeof(char) * (++strLength));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], data->strServerAddress, strLength, &strLength));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    data->strDatetime = (char*)malloc(sizeof(char) * (++strLength));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], data->strDatetime, strLength, &strLength));
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
    addon_data->freeOthersFPtr = &freeSetDatetimeParams;
    //设置调用方法
    addon_data->callSoMehtodFPtr = &callSoSetDatetime;
    //初始化
    initWorkTsfn(env, argv[4], addon_data);
    return NULL;
}


//------------int setTimezone();

struct AddonDataSetTimezone
{
    int isPermanent;
    char* strTimezone;
};

//释放others参数的内存
void freeSetTimezoneParams(void* params)
{
    struct AddonDataSetTimezone* data = (struct AddonDataSetTimezone*)params;
    free(data->strTimezone);
    free(data);
}

napi_value setTimezoneSync(napi_env env, napi_callback_info info) {
    size_t argc = 3;
    napi_value argv[3];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 3)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    //获取参数
    size_t strLength;
    //isPermanent
    int isPermanent = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &isPermanent));
    //strTimezone
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    char* strTimezone = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], strTimezone, strLength, &strLength));
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setTimezone)
        {
            valuePtr = addonDataSo->setTimezone(isPermanent, strTimezone);
        }
    #else
        valuePtr = setTimezone(isPermanent, strTimezone);
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
    napi_typeof(env, argv[2], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[2], 2, callbackParams, &returnVal);
    }
    free(strTimezone);
    return methodReult;
}

struct AddonDataCBInParams* callSoSetTimezone(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    //获取传参
    addonDataCBInParams->value = NULL;
    struct AddonDataSetTimezone* data = (struct AddonDataSetTimezone*)params;
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setTimezone)
        {
            valuePtr = addonDataSo->setTimezone(data->isPermanent, data->strTimezone);
        }
    #else
        valuePtr = setTimezone(data->isPermanent, data->strTimezone);
    #endif
    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;

    return addonDataCBInParams;
}

napi_value setTimezone(napi_env env, napi_callback_info info) {
    size_t argc = 3;
    napi_value argv[3];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 3)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[2], &valueTypeLast);

    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    struct AddonDataSetTimezone* data = (struct AddonDataSetTimezone*)malloc(sizeof(struct AddonDataSetTimezone));
    //获取参数
    size_t strLength;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &data->isPermanent));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    data->strTimezone = (char*)malloc(sizeof(char) * (++strLength));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], data->strTimezone, strLength, &strLength));
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
    addon_data->freeOthersFPtr = &freeSetTimezoneParams;
    //设置调用方法
    addon_data->callSoMehtodFPtr = &callSoSetTimezone;
    //初始化
    initWorkTsfn(env, argv[2], addon_data);
    return NULL;
}


//------------int setDaylightSaving();

struct AddonDataSetDaylightSaving
{
    int isdst;
    int offsetSeconds;
    int startMonth;
    int startWeek;
    int startDay;
    char* startTime;
    int endMonth;
    int endWeek;
    int endDay;
    char* endTime;
};

//释放others参数的内存
void freeSetDaylightSavingParams(void* params)
{
    struct AddonDataSetDaylightSaving* data = (struct AddonDataSetDaylightSaving*)params;
    free(data->startTime);
    free(data->endTime);
    free(data);
}

napi_value setDaylightSavingSync(napi_env env, napi_callback_info info) {
    size_t argc = 11;
    napi_value argv[11];
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 11)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    //获取参数
    size_t strLength;
    //isdst
    int isdst = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &isdst));
    //offsetSeconds
    int offsetSeconds = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &offsetSeconds));
    //startMonth
    int startMonth = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[2], &startMonth));
    //startWeek
    int startWeek = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[3], &startWeek));
    //startDay
    int startDay = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[4], &startDay));
    //strTimezone
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[5], NULL, NULL, &strLength));
    char* startTime = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[5], startTime, strLength, &strLength));
    //endMonth
    int endMonth = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[6], &endMonth));
    //endWeek
    int endWeek = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[7], &endWeek));
    //endDay
    int endDay = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[8], &endDay));
    //endTime
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[9], NULL, NULL, &strLength));
    char* endTime = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[9], endTime, strLength, &strLength));
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setDaylightSaving)
        {
            valuePtr = addonDataSo->setDaylightSaving(isdst, offsetSeconds, startMonth, startWeek, startDay, startTime, endMonth, endWeek, endDay, endTime);
        }
    #else
        valuePtr = setDaylightSaving(isdst, offsetSeconds, startMonth, startWeek, startDay, startTime, endMonth, endWeek, endDay, endTime);
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
    napi_typeof(env, argv[10], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[10], 2, callbackParams, &returnVal);
    }
    free(startTime);
    free(endTime);
    return methodReult;
}

struct AddonDataCBInParams* callSoSetDaylightSaving(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    //获取传参
    addonDataCBInParams->value = NULL;
    struct AddonDataSetDaylightSaving* data = (struct AddonDataSetDaylightSaving*)params;
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setDaylightSaving)
        {
            valuePtr = addonDataSo->setDaylightSaving(data->isdst, data->offsetSeconds, data->startMonth, data->startWeek, data->startDay, data->startTime,
                data->endMonth, data->endWeek, data->endDay, data->endTime);
        }
    #else
        valuePtr = setDaylightSaving(data->isdst, data->offsetSeconds, data->startMonth, data->startWeek, data->startDay, data->startTime,
            data->endMonth, data->endWeek, data->endDay, data->endTime);
    #endif
    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;

    return addonDataCBInParams;
}

napi_value setDaylightSaving(napi_env env, napi_callback_info info) {
    size_t argc = 11;
    napi_value argv[11];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 11)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    // 判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[10], &valueTypeLast);

    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    struct AddonDataSetDaylightSaving* data = (struct AddonDataSetDaylightSaving*)malloc(sizeof(struct AddonDataSetDaylightSaving));
    //获取参数
    size_t strLength;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &data->isdst));
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &data->offsetSeconds));
    catchErrorMsg(env, napi_get_value_int32(env, argv[2], &data->startMonth));
    catchErrorMsg(env, napi_get_value_int32(env, argv[3], &data->startWeek));
    catchErrorMsg(env, napi_get_value_int32(env, argv[4], &data->startDay));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[5], NULL, NULL, &strLength));
    data->startTime = (char*)malloc(sizeof(char) * (++strLength));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[5], data->startTime, strLength, &strLength));
    catchErrorMsg(env, napi_get_value_int32(env, argv[6], &data->endMonth));
    catchErrorMsg(env, napi_get_value_int32(env, argv[7], &data->endWeek));
    catchErrorMsg(env, napi_get_value_int32(env, argv[8], &data->endDay));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[9], NULL, NULL, &strLength));
    data->endTime = (char*)malloc(sizeof(char) * (++strLength));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[9], data->endTime, strLength, &strLength));
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
    addon_data->freeOthersFPtr = &freeSetDaylightSavingParams;
    //设置调用方法
    addon_data->callSoMehtodFPtr = &callSoSetDaylightSaving;
    //初始化
    initWorkTsfn(env, argv[10], addon_data);
    return NULL;
}