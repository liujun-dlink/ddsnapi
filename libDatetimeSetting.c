#include <string.h>
#include "dds-napi-common.h"
#ifdef CACHE_SO_LIB
#else
	#include "libDatetimeSetting.h"
#endif

napi_value getCityDatetimeSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //����so����
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getCityDatetime)
        {
            valuePtr = addonDataSo->getCityDatetime();
        }
    #else
        valuePtr = getCityDatetime();
    #endif
    // ת����
    napi_value methodReult;
    napi_value errorInfo;
    // Ϊ�丳Ĭ��ֵnull
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }
    errorInfo = getErrorMsg(env);
    // �ص���������
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    // �ж��Ƿ���ڻص�����
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
    //����so����
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
    // �ж��Ƿ���ڻص�����
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // �����ڴ�ռ䣬��work_complete�л��ͷ�
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //��Ҫ�����ֶ���malloc��������ʳ�Ա����work��tsfn����ִ���
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //û����������������Ϊnull����
    addon_data->others = NULL;
    //�Ƿ����ַ�ռ�������������ͷ��ڴ淽��
    addon_data->freeOthersFPtr = NULL;
    addon_data->callSoMehtodFPtr = &callSoGetCityDatetime;
    //��ʼ��
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

//�ͷ�others�������ڴ�
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
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 12)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    //��ȡ����
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
    //����so����
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setCityDatetime)
        {
            valuePtr = addonDataSo->setCityDatetime(strCityId, isDstEnabled, offsetSeconds, startMonth, startWeek, startDay, startTime, endMonth, endWeek, endDay, endTime);
        }
    #else
        valuePtr = setCityDatetime(strCityId, isDstEnabled, offsetSeconds, startMonth, startWeek, startDay, startTime, endMonth, endWeek, endDay, endTime);
    #endif
    // ת����
    napi_value methodReult;
    napi_value errorInfo;
    // Ϊ�丳Ĭ��ֵnull
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    errorInfo = getErrorMsg(env);
    // �ص���������
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    //�ж��Ƿ���ڻص�����
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
    //��ȡ����
    addonDataCBInParams->value = NULL;
    struct AddonDataSetCityDatetime* data = (struct AddonDataSetCityDatetime*)params;
    //����so����
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
    // �ж��Ƿ���ڻص�����
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[11], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    //��ȡ����
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
    // �����ڴ�ռ䣬��work_complete�л��ͷ�
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //��Ҫ�����ֶ���malloc��������ʳ�Ա����work��tsfn����ִ���
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //û����������������Ϊnull����
    addon_data->others = data;
    //�Ƿ����ַ�ռ�������������ͷ��ڴ淽��
    addon_data->freeOthersFPtr = &freeSetCityDatetimeParams;
    //���õ��÷���
    addon_data->callSoMehtodFPtr = &callSoSetCityDatetime;
    //��ʼ��
    initWorkTsfn(env, argv[11], addon_data);
    return NULL;
}

// ----char* getCityDst

//�ͷ�others�������ڴ�
void freeGetCityDstSyncParams(void* params)
{
    free((char *)params);
}

napi_value getCityDstSync(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 2)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    //��ȡ����
    size_t strLength;
    //strCityId
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* strCityId = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], strCityId, strLength, &strLength));
    //����so����
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getCityDst)
        {
            valuePtr = addonDataSo->getCityDst(strCityId);
        }
    #else
        valuePtr = getCityDst(strCityId);
    #endif
    // ת����
    napi_value methodReult;
    napi_value errorInfo;
    // Ϊ�丳Ĭ��ֵnull
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }
    errorInfo = getErrorMsg(env);
    // �ص���������
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    // �ж��Ƿ���ڻص�����
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
    //����so����
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
    // �ж��Ƿ���ڻص�����
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[1], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    //��ȡ����
    size_t strLength;
    //strCityId
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* strCityId = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], strCityId, strLength, &strLength));
    // �����ڴ�ռ䣬��work_complete�л��ͷ�
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //��Ҫ�����ֶ���malloc��������ʳ�Ա����work��tsfn����ִ���
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //û����������������Ϊnull����
    addon_data->others = strCityId;
    //�Ƿ����ַ�ռ�������������ͷ��ڴ淽��
    addon_data->freeOthersFPtr = &freeGetCityDstSyncParams;
    addon_data->callSoMehtodFPtr = &callSoGetCityDst;
    //��ʼ��
    initWorkTsfn(env, argv[1], addon_data);
    return NULL;
}

// ----------------char* getDatetime()
napi_value getDatetimeSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //����so����
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getDatetime)
        {
            valuePtr = addonDataSo->getDatetime();
        }
    #else
        valuePtr = getDatetime();
    #endif
    // ת����
    napi_value methodReult;
    napi_value errorInfo;
    // Ϊ�丳Ĭ��ֵnull
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }
    errorInfo = getErrorMsg(env);
    // �ص���������
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    // �ж��Ƿ���ڻص�����
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
    //����so����
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
    // �ж��Ƿ���ڻص�����
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }

    // �����ڴ�ռ䣬��work_complete�л��ͷ�
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //��Ҫ�����ֶ���malloc��������ʳ�Ա����work��tsfn����ִ���
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //û����������������Ϊnull����
    addon_data->others = NULL;
    //�Ƿ����ַ�ռ�������������ͷ��ڴ淽��
    addon_data->freeOthersFPtr = NULL;
    addon_data->callSoMehtodFPtr = &callSoGetDatetime;
    //��ʼ��
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

// ----------------char* getTimezone()
napi_value getTimezoneSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //����so����
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getTimezone)
        {
            valuePtr = addonDataSo->getTimezone();
        }
    #else
        valuePtr = getTimezone();
    #endif
    // ת����
    napi_value methodReult;
    napi_value errorInfo;
    // Ϊ�丳Ĭ��ֵnull
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }
    errorInfo = getErrorMsg(env);
    // �ص���������
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    // �ж��Ƿ���ڻص�����
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
    //����so����
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
    // �ж��Ƿ���ڻص�����
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // �����ڴ�ռ䣬��work_complete�л��ͷ�
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //��Ҫ�����ֶ���malloc��������ʳ�Ա����work��tsfn����ִ���
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //û����������������Ϊnull����
    addon_data->others = NULL;
    //�Ƿ����ַ�ռ�������������ͷ��ڴ淽��
    addon_data->freeOthersFPtr = NULL;
    addon_data->callSoMehtodFPtr = &callSoGetTimezone;
    //��ʼ��
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

// ----------------char* getDaylightSaving()
napi_value getDaylightSavingSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //����so����
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getDaylightSaving)
        {
            valuePtr = addonDataSo->getDaylightSaving();
        }
    #else
        valuePtr = getDaylightSaving();
    #endif
    // ת����
    napi_value methodReult;
    napi_value errorInfo;
    // Ϊ�丳Ĭ��ֵnull
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    if (valuePtr != NULL)
    {
        catchErrorMsg(env, napi_create_string_utf8(env, valuePtr, NAPI_AUTO_LENGTH, &methodReult));
    }
    errorInfo = getErrorMsg(env);
    // �ص���������
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    // �ж��Ƿ���ڻص�����
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
    //����so����
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
    // �ж��Ƿ���ڻص�����
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // �����ڴ�ռ䣬��work_complete�л��ͷ�
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //��Ҫ�����ֶ���malloc��������ʳ�Ա����work��tsfn����ִ���
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //û����������������Ϊnull����
    addon_data->others = NULL;
    //�Ƿ����ַ�ռ�������������ͷ��ڴ淽��
    addon_data->freeOthersFPtr = NULL;
    addon_data->callSoMehtodFPtr = &callSoGetDaylightSaving;
    //��ʼ��
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

//�ͷ�others�������ڴ�
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
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 5)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    //��ȡ����
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
    //����so����
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setDatetime)
        {
            valuePtr = addonDataSo->setDatetime(isPermanent, enableNTPServer, strServerAddress, strDatetime);
        }
    #else
        valuePtr = setDatetime(isPermanent, enableNTPServer, strServerAddress, strDatetime);
    #endif
    // ת����
    napi_value methodReult;
    napi_value errorInfo;
    // Ϊ�丳Ĭ��ֵnull
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    errorInfo = getErrorMsg(env);
    // �ص���������
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    //�ж��Ƿ���ڻص�����
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
    //��ȡ����
    addonDataCBInParams->value = NULL;
    struct AddonDataSetDatetime* data = (struct AddonDataSetDatetime*)params;
    //����so����
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
    // �ж��Ƿ���ڻص�����
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[4], &valueTypeLast);

    if (valueTypeLast != napi_function) {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    struct AddonDataSetDatetime* data = (struct AddonDataSetDatetime*)malloc(sizeof(struct AddonDataSetDatetime));
    //��ȡ����
    size_t strLength;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &data->isPermanent));
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &data->enableNTPServer));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    data->strServerAddress = (char*)malloc(sizeof(char) * (++strLength));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], data->strServerAddress, strLength, &strLength));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    data->strDatetime = (char*)malloc(sizeof(char) * (++strLength));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], data->strDatetime, strLength, &strLength));
    // �����ڴ�ռ䣬��work_complete�л��ͷ�
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //��Ҫ�����ֶ���malloc��������ʳ�Ա����work��tsfn����ִ���
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //û����������������Ϊnull����
    addon_data->others = data;
    //�Ƿ����ַ�ռ�������������ͷ��ڴ淽��
    addon_data->freeOthersFPtr = &freeSetDatetimeParams;
    //���õ��÷���
    addon_data->callSoMehtodFPtr = &callSoSetDatetime;
    //��ʼ��
    initWorkTsfn(env, argv[4], addon_data);
    return NULL;
}


//------------int setTimezone();

struct AddonDataSetTimezone
{
    int isPermanent;
    char* strTimezone;
};

//�ͷ�others�������ڴ�
void freeSetTimezoneParams(void* params)
{
    struct AddonDataSetTimezone* data = (struct AddonDataSetTimezone*)params;
    free(data->strTimezone);
    free(data);
}

napi_value setTimezoneSync(napi_env env, napi_callback_info info) {
    size_t argc = 3;
    napi_value argv[3];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 3)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    //��ȡ����
    size_t strLength;
    //isPermanent
    int isPermanent = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &isPermanent));
    //strTimezone
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    char* strTimezone = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], strTimezone, strLength, &strLength));
    //����so����
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setTimezone)
        {
            valuePtr = addonDataSo->setTimezone(isPermanent, strTimezone);
        }
    #else
        valuePtr = setTimezone(isPermanent, strTimezone);
    #endif
    // ת����
    napi_value methodReult;
    napi_value errorInfo;
    // Ϊ�丳Ĭ��ֵnull
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    errorInfo = getErrorMsg(env);
    // �ص���������
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    //�ж��Ƿ���ڻص�����
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
    //��ȡ����
    addonDataCBInParams->value = NULL;
    struct AddonDataSetTimezone* data = (struct AddonDataSetTimezone*)params;
    //����so����
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
    // �ж��Ƿ���ڻص�����
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[2], &valueTypeLast);

    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    struct AddonDataSetTimezone* data = (struct AddonDataSetTimezone*)malloc(sizeof(struct AddonDataSetTimezone));
    //��ȡ����
    size_t strLength;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &data->isPermanent));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    data->strTimezone = (char*)malloc(sizeof(char) * (++strLength));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[1], data->strTimezone, strLength, &strLength));
    // �����ڴ�ռ䣬��work_complete�л��ͷ�
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //��Ҫ�����ֶ���malloc��������ʳ�Ա����work��tsfn����ִ���
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //û����������������Ϊnull����
    addon_data->others = data;
    //�Ƿ����ַ�ռ�������������ͷ��ڴ淽��
    addon_data->freeOthersFPtr = &freeSetTimezoneParams;
    //���õ��÷���
    addon_data->callSoMehtodFPtr = &callSoSetTimezone;
    //��ʼ��
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

//�ͷ�others�������ڴ�
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
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 11)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    //��ȡ����
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
    //����so����
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setDaylightSaving)
        {
            valuePtr = addonDataSo->setDaylightSaving(isdst, offsetSeconds, startMonth, startWeek, startDay, startTime, endMonth, endWeek, endDay, endTime);
        }
    #else
        valuePtr = setDaylightSaving(isdst, offsetSeconds, startMonth, startWeek, startDay, startTime, endMonth, endWeek, endDay, endTime);
    #endif
    // ת����
    napi_value methodReult;
    napi_value errorInfo;
    // Ϊ�丳Ĭ��ֵnull
    napi_get_null(env, &methodReult);
    napi_get_null(env, &errorInfo);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    errorInfo = getErrorMsg(env);
    // �ص���������
    napi_value callbackParams[2];
    callbackParams[0] = errorInfo;
    callbackParams[1] = methodReult;
    //�ж��Ƿ���ڻص�����
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
    //��ȡ����
    addonDataCBInParams->value = NULL;
    struct AddonDataSetDaylightSaving* data = (struct AddonDataSetDaylightSaving*)params;
    //����so����
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
    // �ж��Ƿ���ڻص�����
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[10], &valueTypeLast);

    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    struct AddonDataSetDaylightSaving* data = (struct AddonDataSetDaylightSaving*)malloc(sizeof(struct AddonDataSetDaylightSaving));
    //��ȡ����
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
    // �����ڴ�ռ䣬��work_complete�л��ͷ�
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //��Ҫ�����ֶ���malloc��������ʳ�Ա����work��tsfn����ִ���
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //û����������������Ϊnull����
    addon_data->others = data;
    //�Ƿ����ַ�ռ�������������ͷ��ڴ淽��
    addon_data->freeOthersFPtr = &freeSetDaylightSavingParams;
    //���õ��÷���
    addon_data->callSoMehtodFPtr = &callSoSetDaylightSaving;
    //��ʼ��
    initWorkTsfn(env, argv[10], addon_data);
    return NULL;
}