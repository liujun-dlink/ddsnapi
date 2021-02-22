#include "node_api.h"
#include <stdlib.h>
#include "libUptime.h"
#include <string.h>

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
} AddonData_getUptime;

void catch_error_getUptime(napi_env env, napi_status status) {
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

void work_complete_getUptime(napi_env env, napi_status status, void* data) {
    AddonData_getUptime* addon_data = (AddonData_getUptime*)data;

    //释放句柄
    catch_error_getUptime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_getUptime(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    //free(addon_data->libPath);
    free(addon_data);
}

void call_js_callback_getUptime(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_getUptime(env, napi_create_string_utf8(
            env,
            (const char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_getUptime(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_getUptime(env, napi_call_function(
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
        // 释放数据
        free((char *)data);
    }
}

napi_value get_uptime_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_getUptime(env, napi_get_cb_info(
        env,
        info,
        &argc,
        argv,
        NULL,
        NULL
    ));
    if (argc != 1)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
    //执行so函数
    char* time = getUptime();
    //转类型
    napi_value world;
    catch_error_getUptime(env, napi_create_string_utf8(env, time, NAPI_AUTO_LENGTH, &world));
    //回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[0], callbackArgc, callbackParams, &callbackRs);
    }
    return world;
}

/**
 * 执行线程
*/
void execute_work_getUptime(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_getUptime* addon_data = (AddonData_getUptime*)data;
    //获取js-callback函数
    catch_error_getUptime(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    char* time = getUptime();
    char* uptime = (char*)malloc(sizeof(char) *(strlen(time) + 1));
    strcpy(uptime, time);
    // 调用js-callback函数
    catch_error_getUptime(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        uptime,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_getUptime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

napi_value get_uptime(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_getUptime(env, napi_get_cb_info(
        env,
        info,
        &argc,
        argv,
        NULL,
        NULL
    ));
    if (argc != 1)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
    //判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[0], &valueTypeLast);

    if (valueTypeLast != napi_function) {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // 创建线程名字
    catch_error_getUptime(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 分配内存空间，在work_complete中会释放
    AddonData_getUptime* addon_data = (AddonData_getUptime*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_getUptime(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_getUptime,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_getUptime(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_getUptime,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_getUptime,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_getUptime(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}
/*
napi_value init(napi_env env, napi_value exports)
{

    // 获取性能数据
    napi_property_descriptor getUptimeSync = {
        "getUptimeSync",
        NULL,
        get_uptime_sync,
        NULL,
        NULL,
        NULL,
        napi_default,
        NULL
    };
    //暴露接口
    napi_define_properties(
        env,
        exports,
        1,
        &getUptimeSync
    );

    // 获取性能数据
    napi_property_descriptor getUptime = {
        "getUptime",
        NULL,
        get_uptime,
        NULL,
        NULL,
        NULL,
        napi_default,
        NULL
    };
    //暴露接口
    napi_define_properties(
        env,
        exports,
        1,
        &getUptime
    );

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init);*/