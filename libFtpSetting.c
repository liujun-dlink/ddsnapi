#include <string.h>
#include "dds-napi-common.h"
#ifdef CACHE_SO_LIB
#else
	#include "libFtpSetting.h"
#endif


//------------char* getFtpData();
napi_value getFtpDataSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc != 1)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
        return NULL;
    }
    //����so����
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFtpData)
        {
            valuePtr = addonDataSo->getFtpData();
        }
    #else
        valuePtr = getFtpData();
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
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value global;
        napi_get_global(env, &global);
        napi_value returnVal;
        napi_call_function(env, global, argv[0], 2, callbackParams, &returnVal);
    }
    return methodReult;
}

struct AddonDataCBInParams* callSoGetFtpData(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiString;
    addonDataCBInParams->value = NULL;
    //����so����
    char* valuePtr = NULL;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->getFtpData)
        {
            valuePtr = addonDataSo->getFtpData();
        }
    #else
        valuePtr = getFtpData();
    #endif
    if (valuePtr != NULL)
    {
        addonDataCBInParams->value = (char*)malloc(sizeof(char) * (strlen(valuePtr) + 1));
        strcpy(addonDataCBInParams->value, valuePtr);
    }
    return addonDataCBInParams;
}

napi_value getFtpData(napi_env env, napi_callback_info info) {
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
    addon_data->callSoMehtodFPtr = &callSoGetFtpData;
    //��ʼ��
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}

//------------int setFtpData();

struct AddonDataSetFtpData
{
    char *ftpServer;
    int ftpPort;
    char *ftpUsername;
    char *ftpPassword;
};

//�ͷ�others�������ڴ�
void freeParams(void* params)
{
    struct AddonDataSetFtpData *data = (struct AddonDataSetFtpData*)params;
    free(data->ftpServer);
    free(data->ftpUsername);
    free(data->ftpPassword);
    free(data);
}

napi_value setFtpDataSync(napi_env env, napi_callback_info info) {
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
    //ftpServer
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* ftpServer = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], ftpServer, strLength, &strLength));
    //ftpPort
    int ftpPort = 0;
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &ftpPort));
    //ftpUsername
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    char* ftpUsername = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], ftpUsername, strLength, &strLength));
    //ftpServer
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    char* ftpPassword = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], ftpPassword, strLength, &strLength));
    //����so����
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setFtpData)
        {
            valuePtr = addonDataSo->setFtpData(ftpServer, ftpPort, ftpUsername, ftpPassword);
        }
    #else
        valuePtr = setFtpData(ftpServer, ftpPort, ftpUsername, ftpPassword);
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
    free(ftpServer);
    free(ftpUsername);
    free(ftpPassword);
    return methodReult;
}

struct AddonDataCBInParams* callSoSetFtpData(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    //��ȡ����
    addonDataCBInParams->value = NULL;
    struct AddonDataSetFtpData *data = (struct AddonDataSetFtpData*)params;
    //����so����
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->setFtpData)
        {
            valuePtr = addonDataSo->setFtpData(data->ftpServer, data->ftpPort, data->ftpUsername, data->ftpPassword);
        }
    #else
        valuePtr = setFtpData(data->ftpServer, data->ftpPort, data->ftpUsername, data->ftpPassword);
    #endif
    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;

    return addonDataCBInParams;
}

napi_value setFtpData(napi_env env, napi_callback_info info) {
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
        napi_throw_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    //��ȡ����
    size_t strLength;
    struct AddonDataSetFtpData *data = (struct AddonDataSetFtpData*)malloc(sizeof(struct AddonDataSetFtpData));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    data->ftpServer = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], data->ftpServer, strLength, &strLength));
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &data->ftpPort));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    data->ftpUsername = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], data->ftpUsername, strLength, &strLength));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    data->ftpPassword = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], data->ftpPassword, strLength, &strLength));
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
    addon_data->freeOthersFPtr = &freeParams;
    //���õ��÷���
    addon_data->callSoMehtodFPtr = &callSoSetFtpData;
    //��ʼ��
    initWorkTsfn(env, argv[4], addon_data);
    return NULL;
}