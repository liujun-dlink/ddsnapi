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
    //获取参数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    //调用so函数
    int valuePtr = 0;
    #ifdef CACHE_SO_LIB
        if (addonDataSo && addonDataSo->killConsoles)
        {
            valuePtr = addonDataSo->killConsoles();
        }
    #else
        valuePtr = killConsoles();
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
    //调用so函数
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
    //设置调用方法
    addon_data->callSoMehtodFPtr = &callSoKillConsoles;
    //初始化
    initWorkTsfn(env, argv[0], addon_data);
    return NULL;
}