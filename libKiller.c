#include <string.h>
#include "dds-napi-common.h"
#ifdef CACHE_SO_LIB
#else
#include "libKiller.h"
#endif


//------------int killConsoles();
napi_value killConsolesSync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    //��ȡ����
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //����so����
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->killConsoles)
        {
            valuePtr = addonDataSo->killConsoles();
        }
    #else
        valuePtr = killConsoles();
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

struct AddonDataCBInParams* callSoKillConsoles(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->value = NULL;
    //����so����
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->killConsoles)
        {
            valuePtr = addonDataSo->killConsoles();
        }
    #else
        valuePtr = killConsoles();
    #endif
    addonDataCBInParams->value = malloc(sizeof(int));
    *((int *)(addonDataCBInParams->value)) = valuePtr;

    return addonDataCBInParams;
}

napi_value killConsoles(napi_env env, napi_callback_info info) {
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
    addon_data->callSoMehtodFPtr = &callSoKillConsoles;
    //��ʼ��
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}