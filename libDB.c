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
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //����so����
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getHWVersion)
        {
            valuePtr = addonDataSo->getHWVersion();
        }
    #else
        valuePtr = getHWVersion();
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

struct AddonDataCBInParams* callSoGetHWVersion(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //����so����
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
    addon_data->callSoMehtodFPtr = &callSoGetHWVersion;
    //��ʼ��
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}


// ----------------char* getNCVersion()
napi_value getNCVersionSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //����so����
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getNCVersion)
        {
            valuePtr = addonDataSo->getNCVersion();
        }
    #else
        valuePtr = getNCVersion();
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

struct AddonDataCBInParams* callSoGetNCVersion(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //����so����
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
    addon_data->callSoMehtodFPtr = &callSoGetNCVersion;
    //��ʼ��
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}


// ----------------char* getFOTASetting()
napi_value getFOTASettingSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //����so����
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFOTASetting)
        {
            valuePtr = addonDataSo->getFOTASetting();
        }
    #else
        valuePtr = getFOTASetting();
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

struct AddonDataCBInParams* callSoGetFOTASetting(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //����so����
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
    addon_data->callSoMehtodFPtr = &callSoGetFOTASetting;
    //��ʼ��
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}


// ----------------char* getFirmwareVersion()
napi_value getFirmwareVersionSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //����so����
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFirmwareVersion)
        {
            valuePtr = addonDataSo->getFirmwareVersion();
        }
    #else
        valuePtr = getFirmwareVersion();
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

struct AddonDataCBInParams* callSoGetFirmwareVersion(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //����so����
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
    addon_data->callSoMehtodFPtr = &callSoGetFirmwareVersion;
    //��ʼ��
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}


// ----------------char* getFirmwareVersionFull()
napi_value getFirmwareVersionFullSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //����so����
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFirmwareVersionFull)
        {
            valuePtr = addonDataSo->getFirmwareVersionFull();
        }
    #else
        valuePtr = getFirmwareVersionFull();
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

struct AddonDataCBInParams* callSoGetFirmwareVersionFull(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //����so����
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
    addon_data->callSoMehtodFPtr = &callSoGetFirmwareVersionFull;
    //��ʼ��
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}


// ----------------char* getFirmwareInfo()
napi_value getFirmwareInfoSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //����so����
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFirmwareInfo)
        {
            valuePtr = addonDataSo->getFirmwareInfo();
        }
    #else
        valuePtr = getFirmwareInfo();
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

struct AddonDataCBInParams* callSoGetFirmwareInfo(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //����so����
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
    addon_data->callSoMehtodFPtr = &callSoGetFirmwareInfo;
    //��ʼ��
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

// ----------------char* getDeviceUUID()
napi_value getDeviceUUIDSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //����so����
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getDeviceUUID)
        {
            valuePtr = addonDataSo->getDeviceUUID();
        }
    #else
        valuePtr = getDeviceUUID();
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

struct AddonDataCBInParams* callSoGetDeviceUUID(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //����so����
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
    addon_data->callSoMehtodFPtr = &callSoGetDeviceUUID;
    //��ʼ��
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

//------------int getFOTAUpdateStatus();
napi_value getFOTAUpdateStatusSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //����so����
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFOTAUpdateStatus)
        {
            valuePtr = addonDataSo->getFOTAUpdateStatus();
        }
    #else
        valuePtr = getFOTAUpdateStatus();
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
    //����so����
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
    //���õ��÷���
    addon_data->callSoMehtodFPtr = &callSoGetFOTAUpdateStatus;
    //��ʼ��
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

//------------int getFirmwareUpgradeStatus();
napi_value getFirmwareUpgradeStatusSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //����so����
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFirmwareUpgradeStatus)
        {
            valuePtr = addonDataSo->getFirmwareUpgradeStatus();
        }
    #else
        valuePtr = getFirmwareUpgradeStatus();
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
    //����so����
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
    //���õ��÷���
    addon_data->callSoMehtodFPtr = &callSoGetFirmwareUpgradeStatus;
    //��ʼ��
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}


// ----int setFOTAUpdateStatus

napi_value setFOTAUpdateStatusSync(napi_env env, napi_callback_info info) {
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
    //nStatus
    int nStatus = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &nStatus));
    //����so����
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setFOTAUpdateStatus)
        {
            valuePtr = addonDataSo->setFOTAUpdateStatus(nStatus);
        }
    #else
        valuePtr = setFOTAUpdateStatus(nStatus);
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

struct AddonDataCBInParams* callSoSetFOTAUpdateStatus(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->value = NULL;
    int nStatus = (int)params;
    //����so����
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
    // �ж��Ƿ���ڻص�����
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[1], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    //nStatus
    int nStatus = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &nStatus));
    // �����ڴ�ռ䣬��work_complete�л��ͷ�
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //��Ҫ�����ֶ���malloc��������ʳ�Ա����work��tsfn����ִ���
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //û����������������Ϊnull����
    addon_data->others = nStatus;
    //�Ƿ����ַ�ռ�������������ͷ��ڴ淽��
    addon_data->freeOthersFPtr = NULL;
    addon_data->callSoMehtodFPtr = &callSoSetFOTAUpdateStatus;
    //��ʼ��
    initWorkTsfn(env, argv[1], addon_data);
    return NULL;
}


// ----int setFirmwareUpgradeStatus

napi_value setFirmwareUpgradeStatusSync(napi_env env, napi_callback_info info) {
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
    //nStatus
    int nStatus = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &nStatus));
    //����so����
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setFirmwareUpgradeStatus)
        {
            valuePtr = addonDataSo->setFirmwareUpgradeStatus(nStatus);
        }
    #else
        valuePtr = setFirmwareUpgradeStatus(nStatus);
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

struct AddonDataCBInParams* callSoSetFirmwareUpgradeStatus(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->value = NULL;
    int nStatus = (int)params;
    //����so����
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
    // �ж��Ƿ���ڻص�����
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[1], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    //nStatus
    int nStatus = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &nStatus));
    // �����ڴ�ռ䣬��work_complete�л��ͷ�
    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    //��Ҫ�����ֶ���malloc��������ʳ�Ա����work��tsfn����ִ���
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    //û����������������Ϊnull����
    addon_data->others = nStatus;
    //�Ƿ����ַ�ռ�������������ͷ��ڴ淽��
    addon_data->freeOthersFPtr = NULL;
    addon_data->callSoMehtodFPtr = &callSoSetFirmwareUpgradeStatus;
    //��ʼ��
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

//�ͷ�others�������ڴ�
void freeSetFOTASettingParams(void* params)
{
    struct AddonDataSetFOTASetting* data = (struct AddonDataSetFOTASetting*)params;
    free(data);
}

napi_value setFOTASettingSync(napi_env env, napi_callback_info info) {
    size_t argc = 6;
    napi_value argv[6];
    //��ȡ����
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
    //����so����
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setFOTASetting)
        {
            valuePtr = addonDataSo->setFOTASetting(nEnable, nWeekday, nHour, nMinute, nUpdateBetaFw);
        }
    #else
        valuePtr = setFOTASetting(nEnable, nWeekday, nHour, nMinute, nUpdateBetaFw);
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
    //��ȡ����
    addonDataCBInParams->value = NULL;
    struct AddonDataSetFOTASetting* data = (struct AddonDataSetFOTASetting*)params;
    //����so����
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
    // �ж��Ƿ���ڻص�����
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[5], &valueTypeLast);

    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    struct AddonDataSetFOTASetting* data = (struct AddonDataSetFOTASetting*)malloc(sizeof(struct AddonDataSetFOTASetting));
    //��ȡ����
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &data->nEnable));
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &data->nWeekday));
    catchErrorMsg(env, napi_get_value_int32(env, argv[2], &data->nHour));
    catchErrorMsg(env, napi_get_value_int32(env, argv[3], &data->nMinute));
    catchErrorMsg(env, napi_get_value_int32(env, argv[4], &data->nUpdateBetaFw));
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
    addon_data->freeOthersFPtr = &freeSetFOTASettingParams;
    //���õ��÷���
    addon_data->callSoMehtodFPtr = &callSoSetFOTASetting;
    //��ʼ��
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

//�ͷ�others�������ڴ�
void freeAnalyzeFirmwareInfoParams(void* params)
{
    struct AddonDataAnalyzeFirmwareInfo* data = (struct AddonDataAnalyzeFirmwareInfo*)params;
    free(data->strFirmwareInfoFromFOTA);
    free(data);
}

napi_value analyzeFirmwareInfoSync(napi_env env, napi_callback_info info) {
    size_t argc = 6;
    napi_value argv[6];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 6)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    // ��һ����������̬���ӿ��ȫ·��(int nMajor, int nMinor, int nRev, char* strFirmwareInfoFromFOTA, int nUpdateBetaFw)
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
    //����so����
    char *valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->analyzeFirmwareInfo)
        {
            valuePtr = addonDataSo->analyzeFirmwareInfo(nMajor, nMinor, nRev, strFirmwareInfoFromFOTA, nUpdateBetaFw);
        }
    #else
        valuePtr = analyzeFirmwareInfo(nMajor, nMinor, nRev, strFirmwareInfoFromFOTA, nUpdateBetaFw);
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
    //�ж��Ƿ���ڻص�����
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
    //��ȡ����
    addonDataCBInParams->value = NULL;
    struct AddonDataAnalyzeFirmwareInfo* data = (struct AddonDataAnalyzeFirmwareInfo*)params;
    //����so����
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
    // �ж��Ƿ���ڻص�����
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[5], &valueTypeLast);

    if (valueTypeLast != napi_function) {
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    struct AddonDataAnalyzeFirmwareInfo* data = (struct AddonDataAnalyzeFirmwareInfo*)malloc(sizeof(struct AddonDataAnalyzeFirmwareInfo));
    //��ȡ����
    size_t strLength;
    catchErrorMsg(env, napi_get_value_int32(env, argv[0], &data->nMajor));
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &data->nMinor));
    catchErrorMsg(env, napi_get_value_int32(env, argv[2], &data->nRev));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    data->strFirmwareInfoFromFOTA = (char*)malloc(sizeof(char) * (++strLength));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], data->strFirmwareInfoFromFOTA, strLength, &strLength));
    catchErrorMsg(env, napi_get_value_int32(env, argv[4], &data->nUpdateBetaFw));
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
    addon_data->freeOthersFPtr = &freeAnalyzeFirmwareInfoParams;
    //���õ��÷���
    addon_data->callSoMehtodFPtr = &callSoAnalyzeFirmwareInfo;
    //��ʼ��
    initWorkTsfn(env, argv[5], addon_data);
    return NULL;
}