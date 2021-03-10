#include "dds-napi-common.h"
#include <dlfcn.h>


void *dlopen(const char *pathname, int mode)
{
    return dlopen(pathname, mode);
}

void *dlsym(void *handle, const char *symbol)
{
    return dlsym(handle, symbol);
}

int dlclose(void *handle)
{
    return dlclose(handle);
}

char *dlerror(void)
{
    return dlerror();
}

void catchErrorMsg(napi_env env, napi_status status)
{
    //To Do
}

napi_value getErrorMsg(napi_env env)
{
    napi_value errorMsg;

    if (env)
    {
        napi_get_null(env, &errorMsg);
        const napi_extended_error_info* errorInfo;
        // 获取异常信息
        napi_get_last_error_info(env, &errorInfo);
        //存在错误信息
        if (errorInfo && errorInfo->error_message)
        {
            napi_create_string_utf8(env, errorInfo->error_message, NAPI_AUTO_LENGTH, &errorMsg);
        }
    }

    return errorMsg;
}

void initWorkTsfn(napi_env env, napi_value callbackFun, struct AddonData *addonData)
{
    if (env && callbackFun && addonData)
    {
        napi_value workName;
        // �����̣߳��첽ִ��
        char threadNamePtr[20];
        genRandomString(20, threadNamePtr);
        catchErrorMsg(env, napi_create_string_utf8(
            env,
            threadNamePtr,
            NAPI_AUTO_LENGTH,
            &workName
        ));

        // ��js������������̶߳�����ִ�еĺ���
        // �����Ϳ����ڿ����������߳��е�������
        // napi_create_threadsafe_function�������ã�
        // ����һ��napi_value�־����ã���ֵ�������ԴӶ���̵߳��õ�JS�������������첽�ģ�����ζ�ŵ���
        // JS�ص����õ�ֵ���������ڶ����У����ڶ����е�ÿ��ֵ�����ս�����JS����
        catchErrorMsg(env, napi_create_threadsafe_function(
            env,
            callbackFun,   //JS����
            NULL,
            workName,
            0,
            1,
            NULL,
            NULL,
            NULL,
            callbackJSFunction,
            &(addonData->addonDataWorkTsfn->tsfn)   //����napi_create_threadsafe_function�������첽��ȫJS����
        ));


        // ����ִ�����洴���ĺ���
        // napi_create_async_work�������ã�
        // ���������첽ִ���߼��Ĺ�������
        catchErrorMsg(env, napi_create_async_work(
            env,
            NULL,
            workName,
            // ��������ִ���첽�߼��ı��غ����������ĺ����Ǵӹ����ص��ã������¼�ѭ���̲߳���ִ��
            executeWork,
            // ���첽�߼���ɻ�ȡ��ʱ�����ã������¼�ѭ���߳��е���
            workComplete,
            addonData,          // �û��ṩ�����������ġ��⽫�����ݻ�execute��complete����
            &(addonData->addonDataWorkTsfn->work)
        ));
        // ���̷߳ŵ���ִ�ж�����
        catchErrorMsg(env, napi_queue_async_work(env, addonData->addonDataWorkTsfn->work));
    }
}

void executeWork(napi_env env, void* data)
{
    if (env && data)
    {
        struct AddonData *addon_data = (struct AddonData *)data;
        //获取js-callback函数
        catchErrorMsg(env, napi_acquire_threadsafe_function(addon_data->addonDataWorkTsfn->tsfn));

        struct AddonDataCBInParams *addonDataCBInParams = NULL;
        if (addon_data->callSoMehtodFPtr)
        {
            addonDataCBInParams = addon_data->callSoMehtodFPtr(addon_data->addonDataSo, addon_data->others);
        }
        //获取异常信息
        addonDataCBInParams->error = getErrorMsg(env);

        // 调用js-callback函数
        catchErrorMsg(env, napi_call_threadsafe_function(
            addon_data->addonDataWorkTsfn->tsfn,     // js-callback函数
            addonDataCBInParams,                     // 返回给回调函数的参数
            napi_tsfn_blocking                       // 阻塞模式调用
        ));
        //释放句柄
        catchErrorMsg(env, napi_release_threadsafe_function(
            addon_data->addonDataWorkTsfn->tsfn,
            napi_tsfn_release
        ));
    }
}

void callbackJSFunction(napi_env env, napi_value js_cb, void* context, void* data)
{
    if (env && js_cb && data)
    {
        struct AddonDataCBInParams *addon_data = (struct AddonDataCBInParams *)data;
        napi_value methodResult;
		catchErrorMsg(env, napi_get_null(env, &methodResult));
		if (addon_data->value && addon_data->transferNapiValueFPtr)
		{
            methodResult = addon_data->transferNapiValueFPtr(env, addon_data->value);
		}

        //返回给回调函数的参数，目前有两个，顺序为error、value
        napi_value callbackInParams[2];
        callbackInParams[0] = addon_data->error;
        callbackInParams[1] = methodResult;

        //获取global
        napi_value undefined;
        catchErrorMsg(env, napi_get_undefined(env, &undefined));

        //调用函数
        catchErrorMsg(env, napi_call_function(
            env,
            undefined,              // js回调的this对象
            js_cb,                  // js回调函数句柄
            2,                      // js回调函数接受参数个数
            callbackInParams,       // js回调函数参数数组
            NULL                    // js回调函数中如果有return，将会被result接受�?
        ));
    }

    // 释放内存
    if (data)
    {
        struct AddonDataCBInParams *addon_data = (struct AddonDataCBInParams *)data;
        if (addon_data->value)
        {
            free(addon_data->value);
        }
        free(data);
    }
}

void workComplete(napi_env env, napi_status status, void* data)
{
    if(env && data)
    {
        struct AddonData *addon_data = (struct AddonData *)data;
        if(addon_data->addonDataWorkTsfn)
        {
            if(addon_data->addonDataWorkTsfn->tsfn)
            {
                //释放句柄
                catchErrorMsg(env, napi_release_threadsafe_function(
                    addon_data->addonDataWorkTsfn->tsfn,
                    napi_tsfn_release
                ));
            }
            if (addon_data->addonDataWorkTsfn->work)
            {
                //回收任务
                catchErrorMsg(env, napi_delete_async_work(env, addon_data->addonDataWorkTsfn->work));
            }
        }
    }

    //释放内存
    if (data)
    {
        struct AddonData *addon_data = (struct AddonData *)data;
        //释放addonDataWorkTsfn
        if (addon_data->addonDataWorkTsfn)
        {
            if (addon_data->addonDataWorkTsfn->tsfn)
            {
                addon_data->addonDataWorkTsfn->tsfn = NULL;
            }
            if (addon_data->addonDataWorkTsfn->work)
            {
                addon_data->addonDataWorkTsfn->work = NULL;
            }
            free(addon_data->addonDataWorkTsfn);
        }
        //根据传入的回调函数决定是否需要执行回调函数释放others的内�?
        if (addon_data->others && addon_data->freeOthersFPtr)
        {
            addon_data->freeOthersFPtr(addon_data->others);
        }
        free(data);
    }
}

//转换值为字符�?
napi_value transferNapiString(napi_env env, void *value)
{
    napi_value stringValue;
    if (env)
    {
        catchErrorMsg(env, napi_get_null(env, &stringValue));
        if (value)
        {
            catchErrorMsg(env, napi_create_string_utf8(env, (char *)value, NAPI_AUTO_LENGTH, &stringValue));
        }
    }
    return stringValue;
}

//转换值为Int
napi_value transferNapiInt32(napi_env env, void *value)
{
    napi_value intValue;
    if (env)
    {
        catchErrorMsg(env, napi_get_null(env, &intValue));
        if (value)
        {
            catchErrorMsg(env, napi_create_int32(env, *((int *)value), &intValue));
        }
    }
    return intValue;
}


void genRandomString(int length, char *ouput)
{
    char metachar[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int i = 0;
    srand((unsigned)time(NULL) + rand() + rand());
    for (i = 0; i < length; i++)
    {
        ouput[i] = metachar[rand() % 62];
    }
    ouput[length] = '\0';
}