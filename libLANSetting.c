#include <node_api.h>
#include <string.h>
#include <stdlib.h>
#include "libLANSetting.h"

/**
 * 捕获异常
 */
void catch_error_LANSetting(napi_env env, napi_status status) {
    if (status != napi_ok)
    {
        // napi_extended_error_info是一个结构体，包含如下信息
        /**
         * error_message：错误的文本表示
         * engine_reserved：不透明手柄，仅供引擎使用
         * engine_error_code：VM特定错误码
         * error_code：上一个错误的n-api状态代码
        */
        const napi_extended_error_info* error_info = NULL;
        // 获取异常信息
        napi_get_last_error_info(env, &error_info);
        // 创建使用napi_value表示的错误信息
        napi_value error_msg;
        napi_create_string_utf8(
            env,
            error_info->error_message,
            NAPI_AUTO_LENGTH,
            &error_msg
        );
        // 抛出错误，它将会在JS触发一个uncaughtException异常
        napi_fatal_exception(env, error_msg);
        // 退出后续执行
        exit(0);
    }
}

/**
 * 参数个数检查
 */
void checkParams_LANSetting(napi_env env, napi_callback_info info, size_t argc, napi_value* argv) {
    size_t temp = argc;// 保存应有的参数个数
    // 获取实际的参数个数和参数列表
    catch_error_LANSetting(env, napi_get_cb_info(env, info, &argc, argv, NULL, NULL));
    if (argc != temp)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
}

/**
 * 返回值为int的回调处理
 */
void call_js_callback_LANSetting(napi_env env, napi_value js_cb, void* context, void* data)
{
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_LANSetting(env, napi_create_int32(env, (int32_t)data, &callBackInParams));

        //获取global
        napi_value undefined;
        catch_error_LANSetting(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_LANSetting(env, napi_call_function(
            env,
            undefined,   // js回调的this对象
            js_cb,       // js回调函数句柄
            1,           // js回调函数接受参数个数
            &callBackInParams,        // js回调函数参数数组
            NULL         // js回调函数中如果有return，将会被result接受到
        ));
    }
}

/**
 * 返回值为string的回调处理
 */
void call_js_callback_str_LANSetting(napi_env env, napi_value js_cb, void* context, void* data)
{
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_LANSetting(env, napi_create_string_utf8(env, (const char*)data, NAPI_AUTO_LENGTH, &callBackInParams));

        //获取global
        napi_value undefined;
        catch_error_LANSetting(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_LANSetting(env, napi_call_function(
            env,
            undefined,   // js回调的this对象
            js_cb,       // js回调函数句柄
            1,           // js回调函数接受参数个数
            &callBackInParams,        // js回调函数参数数组
            NULL         // js回调函数中如果有return，将会被result接受到
        ));
    }
    if (data != NULL)
    {
        // 释放指针
        free((char*)data);
    }
}

//--int setLANSetting(int isPermanent,int isDHCP,char* argIpAddress,char* argNetmask,char* argGateway,int nDnsType,char* argDNS1,char* argDNS2,int flagUpdateDaa);
napi_value set_LAN_setting_sync(napi_env env, napi_callback_info info) {
    size_t argc = 10;
    napi_value argv[10];
	checkParams_LANSetting(env, info, argc, argv);// 检查参数个数    
	// 获取参数
    size_t strLength;
	//int isPermanent,
    int isPermanent;
    catch_error_LANSetting(env, napi_get_value_int32(env, argv[0], &isPermanent));
	//    int isDHCP,
    int isDHCP;
    catch_error_LANSetting(env, napi_get_value_int32(env, argv[1], &isDHCP));
	//    char* argIpAddress
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    char* argIpAddress = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[2], argIpAddress, strLength, &strLength));
	//    char* argNetmask
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    char* argNetmask = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[3], argNetmask, strLength, &strLength));
	//    char* argGateway
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[4], NULL, NULL, &strLength));
    char* argGateway = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[4], argGateway, strLength, &strLength));
	//    int nDnsType
    int nDnsType;
    catch_error_LANSetting(env, napi_get_value_int32(env, argv[5], &nDnsType));
	//    char* argDNS1
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[6], NULL, NULL, &strLength));
    char* argDNS1 = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[6], argDNS1, strLength, &strLength));
	//    char* argDNS2
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[7], NULL, NULL, &strLength));
    char* argDNS2 = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[7], argDNS2, strLength, &strLength));
	//    int flagUpdateDaa
    int flagUpdateDaa;
    catch_error_LANSetting(env, napi_get_value_int32(env, argv[8], &flagUpdateDaa));
    printf("start setLANSetting\n");
    // 执行so函数
    int status = setLANSetting(isPermanent, isDHCP, argIpAddress, argNetmask, argGateway, nDnsType, argDNS1, argDNS2, flagUpdateDaa);
    printf("end setLANSetting\n");
    free(argIpAddress);
    free(argNetmask);
    free(argGateway);
    free(argDNS1);
    free(argDNS2);
    // 转类型
    napi_value world;
    catch_error_LANSetting(env, napi_create_int32(env, status, &world));
    // 回调函数
    napi_value* callbackParams = &world;
    size_t callbackArgc = 1;
    napi_value global;
    napi_get_global(env, &global);
    napi_value callbackRs;
    napi_call_function(env, global, argv[9], callbackArgc, callbackParams, &callbackRs);
    return world;
}

typedef struct
{
    /**
     * napi_async_work是一个结构体，用于管理异步工作线程
     * 通过napi_create_async_work创建实例
     * 通过napi_delete_async_work删除实例
    */
    napi_async_work work;             // 保存线程的任务
    /**
     * napi_threadsafe_function是一个指针
     * 表示在多线程中可以通过napi_call_threadsafe_function异步调用的JS函数
    */
    napi_threadsafe_function tsfn;    // 保存回调函数
    int isPermanent;
    int isDHCP;
    char* argIpAddress;
    char* argNetmask;
    char* argGateway;
    int nDnsType;
    char* argDNS1;
    char* argDNS2;
    int flagUpdateDaa;
} AddonData_setLANSetting;

/**
 * 执行线程
*/
void execute_work_setLANSetting(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_setLANSetting* addon_data = (AddonData_setLANSetting*)data;
    //获取js-callback函数
    catch_error_LANSetting(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    int status = setLANSetting(addon_data->isPermanent, addon_data->isDHCP, addon_data->argIpAddress, addon_data->argNetmask, 
        addon_data->argGateway, addon_data->nDnsType, addon_data->argDNS1, addon_data->argDNS2, addon_data->flagUpdateDaa);
    // 调用js-callback函数
    catch_error_LANSetting(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        status,                    // 回调函数接收的返回值
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_LANSetting(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}


/**
 * 执行线程完成
*/
void work_complete_setLANSetting(napi_env env, napi_status status, void* data)
{
    AddonData_setLANSetting* addon_data = (AddonData_setLANSetting*)data;
    //释放句柄
    catch_error_LANSetting(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_LANSetting(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data->argIpAddress);
    free(addon_data->argNetmask);
    free(addon_data->argGateway);
    free(addon_data->argDNS1);
    free(addon_data->argDNS2);
    free(addon_data);
}

napi_value set_LAN_setting(napi_env env, napi_callback_info info) {
    size_t argc = 10;
    napi_value argv[10];
    size_t strLength;
    checkParams_LANSetting(env, info, argc, argv);// 检查参数个数    
    // 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_setLANSetting* addon_data = NULL;// 结构体    
    addon_data = (AddonData_setLANSetting*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    //int isPermanent
    catch_error_LANSetting(env, napi_get_value_int32(env, argv[0], &addon_data->isPermanent));
    // int isDHCP
    catch_error_LANSetting(env, napi_get_value_int32(env, argv[1], &addon_data->isDHCP));
    // char* argIpAddress
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    addon_data->argIpAddress = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[2], addon_data->argIpAddress, strLength, &strLength));
    // char* argNetmask
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    addon_data->argNetmask = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[3], addon_data->argNetmask, strLength, &strLength));
    // char* argGateway
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[4], NULL, NULL, &strLength));
    addon_data->argGateway = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[4], addon_data->argGateway, strLength, &strLength));
    // int nDnsType
    catch_error_LANSetting(env, napi_get_value_int32(env, argv[5], &addon_data->nDnsType));
    // char* argDNS1
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[6], NULL, NULL, &strLength));
    addon_data->argDNS1 = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[6], addon_data->argDNS1, strLength, &strLength));
    // char* argDNS2
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[7], NULL, NULL, &strLength));
    addon_data->argDNS2 = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[7], addon_data->argDNS2, strLength, &strLength));
    // int flagUpdateDaa
    catch_error_LANSetting(env, napi_get_value_int32(env, argv[8], &addon_data->flagUpdateDaa));
    // 回调函数
    js_cb = argv[9];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_LANSetting(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe setLANSetting Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_LANSetting(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_LANSetting,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_LANSetting(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_setLANSetting,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_setLANSetting,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_LANSetting(env, napi_queue_async_work(env, addon_data->work));
    return NULL;
}

//-----char* getLANSetting();
napi_value get_LAN_setting_sync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    checkParams_LANSetting(env, info, argc, argv);// 检查参数个数
    // 执行so函数
    char* status = getLANSetting();
    // 转类型
    napi_value world;
    catch_error_LANSetting(env, napi_create_string_utf8(env, status, sizeof(char) * (strlen(status) + 1), &world));
    // 回调函数
    napi_value* callbackParams = &world;
    size_t callbackArgc = 1;
    napi_value global;
    napi_get_global(env, &global);
    napi_value callbackRs;
    napi_call_function(env, global, argv[0], callbackArgc, callbackParams, &callbackRs);
    return world;
}

typedef struct
{
    /**
     * napi_async_work是一个结构体，用于管理异步工作线程
     * 通过napi_create_async_work创建实例
     * 通过napi_delete_async_work删除实例
    */
    napi_async_work work;             // 保存线程的任务
    /**
     * napi_threadsafe_function是一个指针
     * 表示在多线程中可以通过napi_call_threadsafe_function异步调用的JS函数
    */
    napi_threadsafe_function tsfn;    // 保存回调函数
} AddonData_getLANSetting;

/**
 * 执行线程
*/
void execute_work_getLANSetting(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_getLANSetting* addon_data = (AddonData_getLANSetting*)data;
    //获取js-callback函数
    catch_error_LANSetting(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    char* status = getLANSetting();
    char* cbInParam = (char*)malloc(sizeof(char) * (strlen(status) + 1));
    strcpy(cbInParam, status);
    // 调用js-callback函数
    catch_error_LANSetting(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        cbInParam,                    // 回调函数的返回值
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_LANSetting(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}


/**
 * 执行线程完成
*/
void work_complete_getLANSetting(napi_env env, napi_status status, void* data)
{
    AddonData_getLANSetting* addon_data = (AddonData_getLANSetting*)data;
    //释放句柄
    catch_error_LANSetting(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_LANSetting(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data);
}

napi_value get_LAN_setting(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    checkParams_LANSetting(env, info, argc, argv);// 检查参数个数    
    // 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_getLANSetting* addon_data = NULL;// 结构体    
    addon_data = (AddonData_getLANSetting*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 回调函数
    js_cb = argv[0];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_LANSetting(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe getLANSetting Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_LANSetting(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_str_LANSetting,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_LANSetting(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_getLANSetting,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_getLANSetting,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_LANSetting(env, napi_queue_async_work(env, addon_data->work));
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
napi_value set_LAN_setting_ipv6_sync(napi_env env, napi_callback_info info) {
    size_t argc = 10;
    napi_value argv[10];
    checkParams_LANSetting(env, info, argc, argv);// 检查参数个数    
    // 获取参数
    size_t strLength;
    //int isPermanent,
    int isPermanent;
    catch_error_LANSetting(env, napi_get_value_int32(env, argv[0], &isPermanent));    
    // char* argProvision
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    char* argProvision = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[1], argProvision, strLength, &strLength));
    // char* argIpv6Address
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    char* argIpv6Address = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[2], argIpv6Address, strLength, &strLength));
    // int nPrefixLength,
    int nPrefixLength;
    catch_error_LANSetting(env, napi_get_value_int32(env, argv[3], &nPrefixLength));
    // char* argGateway
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[4], NULL, NULL, &strLength));
    char* argGateway = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[4], argGateway, strLength, &strLength));
    // int nDnsType
    int nDnsType;
    catch_error_LANSetting(env, napi_get_value_int32(env, argv[5], &nDnsType));
    // char* argDns1
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[6], NULL, NULL, &strLength));
    char* argDns1 = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[6], argDns1, strLength, &strLength));
    // char* argDns2
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[7], NULL, NULL, &strLength));
    char* argDns2 = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[7], argDns2, strLength, &strLength));
    // int flagUpdateDaa
    int flagUpdateDaa;
    catch_error_LANSetting(env, napi_get_value_int32(env, argv[8], &flagUpdateDaa));
    // 执行so函数
    int status = setLANSettingIpv6(isPermanent, argProvision, argIpv6Address, nPrefixLength, argGateway, nDnsType, argDns1, argDns2, flagUpdateDaa);
    free(argProvision);
    free(argIpv6Address);
    free(argGateway);
    free(argDns1);
    free(argDns2);
    // 转类型
    napi_value world;
    catch_error_LANSetting(env, napi_create_int32(env, status, &world));
    // 回调函数
    napi_value* callbackParams = &world;
    size_t callbackArgc = 1;
    napi_value global;
    napi_get_global(env, &global);
    napi_value callbackRs;
    napi_call_function(env, global, argv[9], callbackArgc, callbackParams, &callbackRs);
    return world;
}

typedef struct
{
    /**
     * napi_async_work是一个结构体，用于管理异步工作线程
     * 通过napi_create_async_work创建实例
     * 通过napi_delete_async_work删除实例
    */
    napi_async_work work;             // 保存线程的任务
    /**
     * napi_threadsafe_function是一个指针
     * 表示在多线程中可以通过napi_call_threadsafe_function异步调用的JS函数
    */
    napi_threadsafe_function tsfn;    // 保存回调函数
    int isPermanent;
    char* argProvision;
    char* argIpv6Address;
    int nPrefixLength;
    char* argGateway;
    int nDnsType;
    char* argDns1;
    char* argDns2;
    int flagUpdateDaa;
} AddonData_setLANSettingIpv6;

/**
 * 执行线程
*/
void execute_work_setLANSettingIpv6(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_setLANSettingIpv6* addon_data = (AddonData_setLANSettingIpv6*)data;
    //获取js-callback函数
    catch_error_LANSetting(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    int status = setLANSettingIpv6(addon_data->isPermanent, addon_data->argProvision, addon_data->argIpv6Address, addon_data->nPrefixLength,
        addon_data->argGateway, addon_data->nDnsType, addon_data->argDns1, addon_data->argDns2, addon_data->flagUpdateDaa);
    // 调用js-callback函数
    catch_error_LANSetting(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        status,                    // 回调函数接收的返回值
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_LANSetting(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}


/**
 * 执行线程完成
*/
void work_complete_setLANSettingIpv6(napi_env env, napi_status status, void* data)
{
    AddonData_setLANSettingIpv6* addon_data = (AddonData_setLANSettingIpv6*)data;
    //释放句柄
    catch_error_LANSetting(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_LANSetting(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data->argProvision);
    free(addon_data->argIpv6Address);
    free(addon_data->argGateway);
    free(addon_data->argDns1);
    free(addon_data->argDns2);
    free(addon_data);
}

napi_value set_LAN_setting_ipv6(napi_env env, napi_callback_info info) {
    size_t argc = 10;
    napi_value argv[10];
    checkParams_LANSetting(env, info, argc, argv);// 检查参数个数    
    // 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_setLANSettingIpv6* addon_data = NULL;// 结构体    
    addon_data = (AddonData_setLANSettingIpv6*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    size_t strLength;
    //int isPermanent
    catch_error_LANSetting(env, napi_get_value_int32(env, argv[0], &addon_data->isPermanent));    
    // char* argProvision
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    addon_data->argProvision = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[1], addon_data->argProvision, strLength, &strLength));
    // char* argIpv6Address
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    addon_data->argIpv6Address = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[2], addon_data->argIpv6Address, strLength, &strLength));
    // int nPrefixLength
    catch_error_LANSetting(env, napi_get_value_int32(env, argv[3], &addon_data->nPrefixLength));
    // char* argGateway
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[4], NULL, NULL, &strLength));
    addon_data->argGateway = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[4], addon_data->argGateway, strLength, &strLength));
    // int nDnsType
    catch_error_LANSetting(env, napi_get_value_int32(env, argv[5], &addon_data->nDnsType));
    // char* argDns1
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[6], NULL, NULL, &strLength));
    addon_data->argDns1 = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[6], addon_data->argDns1, strLength, &strLength));
    // char* argDns2
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[7], NULL, NULL, &strLength));
    addon_data->argDns2 = (char*)malloc((++strLength) * sizeof(char));
    catch_error_LANSetting(env, napi_get_value_string_utf8(env, argv[7], addon_data->argDns2, strLength, &strLength));
    // int flagUpdateDaa
    catch_error_LANSetting(env, napi_get_value_int32(env, argv[8], &addon_data->flagUpdateDaa));
    // 回调函数
    js_cb = argv[9];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_LANSetting(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe setLANSettingIpv6 Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_LANSetting(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_LANSetting,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_LANSetting(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_setLANSettingIpv6,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_setLANSettingIpv6,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_LANSetting(env, napi_queue_async_work(env, addon_data->work));
    return NULL;
}

//----char* getLANSettingIpv6();
napi_value get_LAN_Setting_ipv6_sync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    checkParams_LANSetting(env, info, argc, argv);// 检查参数个数
    // 执行so函数
    char* status = getLANSettingIpv6();
    // 转类型
    napi_value world;
    catch_error_LANSetting(env, napi_create_string_utf8(env, status, sizeof(char) * (strlen(status) + 1), &world));
    // 回调函数
    napi_value* callbackParams = &world;
    size_t callbackArgc = 1;
    napi_value global;
    napi_get_global(env, &global);
    napi_value callbackRs;
    napi_call_function(env, global, argv[0], callbackArgc, callbackParams, &callbackRs);
    return world;
}

typedef struct
{
    /**
     * napi_async_work是一个结构体，用于管理异步工作线程
     * 通过napi_create_async_work创建实例
     * 通过napi_delete_async_work删除实例
    */
    napi_async_work work;             // 保存线程的任务
    /**
     * napi_threadsafe_function是一个指针
     * 表示在多线程中可以通过napi_call_threadsafe_function异步调用的JS函数
    */
    napi_threadsafe_function tsfn;    // 保存回调函数
} AddonData_getLANSettingIpv6;

/**
 * 执行线程
*/
void execute_work_getLANSettingIpv6(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_getLANSettingIpv6* addon_data = (AddonData_getLANSettingIpv6*)data;
    //获取js-callback函数
    catch_error_LANSetting(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    char* status = getLANSettingIpv6();
    char* cbInParam = (char*)malloc(sizeof(char) * (strlen(status) + 1));
    strcpy(cbInParam, status);
    // 调用js-callback函数
    catch_error_LANSetting(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        cbInParam,                    // 回调函数的返回值
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_LANSetting(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}


/**
 * 执行线程完成
*/
void work_complete_getLANSettingIpv6(napi_env env, napi_status status, void* data)
{
    AddonData_getLANSettingIpv6* addon_data = (AddonData_getLANSettingIpv6*)data;
    //释放句柄
    catch_error_LANSetting(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_LANSetting(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data);
}

napi_value get_LAN_Setting_ipv6(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    checkParams_LANSetting(env, info, argc, argv);// 检查参数个数    
    // 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_getLANSettingIpv6* addon_data = NULL;// 结构体    
    addon_data = (AddonData_getLANSettingIpv6*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 回调函数
    js_cb = argv[0];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_LANSetting(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe getLANSettingIpv6 Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_LANSetting(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_str_LANSetting,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_LANSetting(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_getLANSettingIpv6,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_getLANSettingIpv6,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_LANSetting(env, napi_queue_async_work(env, addon_data->work));
    return NULL;
}
