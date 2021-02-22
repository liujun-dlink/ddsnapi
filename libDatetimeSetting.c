#include <node_api.h>
#include <stdlib.h>
#include <string.h>
#include "libDatetimeSetting.h"


void catch_error_get_datetime(napi_env env, napi_status status) {
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
/****************************get_datetime***************************************/
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
} AddonData_get_datetime;

void work_complete_get_datetime(napi_env env, napi_status status, void* data) {
    AddonData_get_datetime* addon_data = (AddonData_get_datetime*)data;

    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_get_datetime(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    free(addon_data);
}

void call_js_callback_get_datetime(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_get_datetime(env, napi_create_string_utf8(
            env,
            (const char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_get_datetime(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_get_datetime(env, napi_call_function(
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
        free((char*)data);
    }
}

 napi_value get_datetime_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_get_datetime(env, napi_get_cb_info(
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
    char* time = getDatetime();
    //转类型
    napi_value world;
    catch_error_get_datetime(env, napi_create_string_utf8(env, time, NAPI_AUTO_LENGTH, &world));
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
 void execute_work_get_datetime(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_get_datetime* addon_data = (AddonData_get_datetime*)data;
    //获取js-callback函数
    catch_error_get_datetime(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    char* time = getDatetime();
    char* uptime = (char*)malloc(sizeof(char) * (strlen(time) + 1));
    strcpy(uptime, time);
    // 调用js-callback函数
    catch_error_get_datetime(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        uptime,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value get_datetime(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_get_datetime(env, napi_get_cb_info(
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
    catch_error_get_datetime(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 分配内存空间，在work_complete中会释放
    AddonData_get_datetime* addon_data = (AddonData_get_datetime*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_get_datetime(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_get_datetime,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_get_datetime(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_get_datetime,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_get_datetime,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_get_datetime(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}
/****************************get_timezone***************************************/
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
} AddonData_get_timezone;

 napi_value get_timezone_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_get_datetime(env, napi_get_cb_info(
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
    char* time = getTimezone();
    //转类型
    napi_value world;
    catch_error_get_datetime(env, napi_create_string_utf8(env, time, NAPI_AUTO_LENGTH, &world));
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

void work_complete_get_timezone(napi_env env, napi_status status, void* data) {
    AddonData_get_timezone* addon_data = (AddonData_get_timezone*)data;

    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_get_datetime(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    free(addon_data);
}

void call_js_callback_get_timezone(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_get_datetime(env, napi_create_string_utf8(
            env,
            (const char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_get_datetime(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_get_datetime(env, napi_call_function(
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
        free((char*)data);
    }
}
/**
 * 执行线程
*/
 void execute_work_get_timezone(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_get_timezone* addon_data = (AddonData_get_timezone*)data;
    //获取js-callback函数
    catch_error_get_datetime(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    char* time = getTimezone();
    char* uptime = (char*)malloc(sizeof(char) * (strlen(time) +1));
    strcpy(uptime, time);
    // 调用js-callback函数
    catch_error_get_datetime(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        uptime,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value get_timezone(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_get_datetime(env, napi_get_cb_info(
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
    catch_error_get_datetime(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 分配内存空间，在work_complete中会释放
    AddonData_get_timezone* addon_data = (AddonData_get_timezone*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_get_datetime(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_get_timezone,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_get_datetime(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_get_timezone,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_get_timezone,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_get_datetime(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}
/****************************get_daylight_saving***************************************/
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
} AddonData_get_daylight_saving;

 napi_value get_daylight_saving_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_get_datetime(env, napi_get_cb_info(
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
    char* result = getDaylightSaving();
    //转类型
    napi_value world;
    catch_error_get_datetime(env, napi_create_string_utf8(env, result, NAPI_AUTO_LENGTH, &world));
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

void work_complete_get_daylight_saving(napi_env env, napi_status status, void* data) {
    AddonData_get_daylight_saving* addon_data = (AddonData_get_daylight_saving*)data;

    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_get_datetime(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    free(addon_data);
}

void call_js_callback_get_daylight_saving(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_get_datetime(env, napi_create_string_utf8(
            env,
            (const char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_get_datetime(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_get_datetime(env, napi_call_function(
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
        free((char*)data);
    }
}
/**
 * 执行线程
*/
 void execute_work_get_daylight_saving(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_get_daylight_saving* addon_data = (AddonData_get_daylight_saving*)data;
    //获取js-callback函数
    catch_error_get_datetime(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    char* result = getDaylightSaving();
    char* rs = (char*)malloc(sizeof(char) * (strlen(result) + 1));
    strcpy(rs, result);
    // 调用js-callback函数
    catch_error_get_datetime(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        rs,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value get_daylight_saving(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_get_datetime(env, napi_get_cb_info(
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
    catch_error_get_datetime(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 分配内存空间，在work_complete中会释放
    AddonData_get_daylight_saving* addon_data = (AddonData_get_daylight_saving*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_get_datetime(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_get_daylight_saving,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_get_datetime(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_get_daylight_saving,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_get_daylight_saving,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_get_datetime(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}
/****************************get_city_datetime***************************************/
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
} AddonData_get_city_datetime;

 napi_value get_city_datetime_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_get_datetime(env, napi_get_cb_info(
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
    char* result = getCityDatetime();
    //转类型
    napi_value world;
    catch_error_get_datetime(env, napi_create_string_utf8(env, result, NAPI_AUTO_LENGTH, &world));
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

void work_complete_get_city_datetime(napi_env env, napi_status status, void* data) {
    AddonData_get_city_datetime* addon_data = (AddonData_get_city_datetime*)data;

    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_get_datetime(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    free(addon_data);
}

void call_js_callback_get_city_datetime(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_get_datetime(env, napi_create_string_utf8(
            env,
            (const char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_get_datetime(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_get_datetime(env, napi_call_function(
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
        free((char*)data);
    }
}
/**
 * 执行线程
*/
 void execute_work_get_city_datetime(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_get_city_datetime* addon_data = (AddonData_get_city_datetime*)data;
    //获取js-callback函数
    catch_error_get_datetime(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    char* result = getCityDatetime();
    char* rs = (char*)malloc(sizeof(char) * (strlen(result) + 1));
    strcpy(rs, result);
    // 调用js-callback函数
    catch_error_get_datetime(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        rs,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value get_city_datetime(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_get_datetime(env, napi_get_cb_info(
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
    catch_error_get_datetime(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 分配内存空间，在work_complete中会释放
    AddonData_get_city_datetime* addon_data = (AddonData_get_city_datetime*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_get_datetime(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_get_city_datetime,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_get_datetime(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_get_city_datetime,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_get_city_datetime,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_get_datetime(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}

/****************************setDatetime***************************************/
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
    int enableNTPServer;
    char* strServerAddress;
    char* strDatetime;
} AddonData_set_datetime;

 napi_value set_datetime_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 5;
    napi_value argv[5];
    catch_error_get_datetime(env, napi_get_cb_info(
        env,
        info,
        &argc,
        argv,
        NULL,
        NULL
    ));
    if (argc != 5)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
    //获取参数(int isPermanent, int enableNTPServer, char* strServerAddress, char* strDatetime)
    size_t strLength;
    //isPermanent
    int isPermanent;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[0], &isPermanent));
    //enableNTPServer
    int enableNTPServer;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[1], &enableNTPServer));
    //strServerAddress
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    char* strServerAddress = (char*)malloc((++strLength) * sizeof(char));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[2], strServerAddress, strLength, &strLength));
    //strDatetime
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    char* strDatetime = (char*)malloc((++strLength) * sizeof(char));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[3], strDatetime, strLength, &strLength));
    //执行so函数
    int result = setDatetime(isPermanent, enableNTPServer, strServerAddress, strDatetime);
    //转类型
    napi_value world;
    catch_error_get_datetime(env, napi_create_int32(env, result, &world));
    //回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[4], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[4], callbackArgc, callbackParams, &callbackRs);
    }
    free(strServerAddress);
    free(strDatetime);
    return world;
}

void work_complete_set_datetime(napi_env env, napi_status status, void* data) {
    AddonData_set_datetime* addon_data = (AddonData_set_datetime*)data;

    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_get_datetime(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    free(addon_data->strServerAddress);
    free(addon_data->strDatetime);
    free(addon_data);
}

void call_js_callback_set_datetime(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_get_datetime(env, napi_create_int32(
            env,
            data,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_get_datetime(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_get_datetime(env, napi_call_function(
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
        free((char*)data);
    }
}
/**
 * 执行线程
*/
 void execute_work_set_datetime(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_set_datetime* addon_data = (AddonData_set_datetime*)data;
    //获取js-callback函数
    catch_error_get_datetime(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    int result = setDatetime(addon_data->isPermanent, addon_data->enableNTPServer, addon_data->strServerAddress, addon_data->strDatetime);
    // 调用js-callback函数
    catch_error_get_datetime(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        result,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value set_datetime(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 5;
    napi_value argv[5];
    catch_error_get_datetime(env, napi_get_cb_info(
        env,
        info,
        &argc,
        argv,
        NULL,
        NULL
    ));
    if (argc != 5)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
    //判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[4], &valueTypeLast);

    if (valueTypeLast != napi_function) {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // 创建线程名字
    catch_error_get_datetime(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    //获取参数(int isPermanent, int enableNTPServer, char* strServerAddress, char* strDatetime)
    size_t strLength;
    AddonData_set_datetime* addon_data = (AddonData_set_datetime*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[0], &addon_data->isPermanent));
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[1], &addon_data->enableNTPServer));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    addon_data->strServerAddress = (char*)malloc(sizeof(char) * (++strLength));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[2], addon_data->strServerAddress, strLength, &strLength));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    addon_data->strDatetime = (char*)malloc(sizeof(char) * (++strLength));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[3], addon_data->strDatetime, strLength, &strLength));
    //strServerAddress
    //strDatetime
    // 分配内存空间，在work_complete中会释放
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_get_datetime(env, napi_create_threadsafe_function(
        env,
        argv[4],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_set_datetime,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_get_datetime(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_set_datetime,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_set_datetime,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_get_datetime(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}
/****************************set_timezone***************************************/
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
    char* strTimezone;
} AddonData_set_timezone;

 napi_value set_timezone_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 3;
    napi_value argv[3];
    catch_error_get_datetime(env, napi_get_cb_info(
        env,
        info,
        &argc,
        argv,
        NULL,
        NULL
    ));
    if (argc != 3)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
    //获取参数(int isPermanent, int enableNTPServer, char* strServerAddress, char* strDatetime)
    size_t strLength;
    //isPermanent
    int isPermanent;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[0], &isPermanent));
    //strTimezone
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    char* strTimezone = (char*)malloc((++strLength) * sizeof(char));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[1], strTimezone, strLength, &strLength));
    //执行so函数
    int result = setTimezone(isPermanent, strTimezone);
    //转类型
    napi_value world;
    catch_error_get_datetime(env, napi_create_int32(env, result, &world));
    //回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[2], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[2], callbackArgc, callbackParams, &callbackRs);
    }
    free(strTimezone);
    return world;
}

void work_complete_set_timezone(napi_env env, napi_status status, void* data) {
    AddonData_set_timezone* addon_data = (AddonData_set_timezone*)data;

    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_get_datetime(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    free(addon_data->strTimezone);
    free(addon_data);
}

void call_js_callback_set_timezone(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_get_datetime(env, napi_create_int32(
            env,
            data,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_get_datetime(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_get_datetime(env, napi_call_function(
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
 * 执行线程
*/
 void execute_work_set_timezone(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_set_timezone* addon_data = (AddonData_set_timezone*)data;
    //获取js-callback函数
    catch_error_get_datetime(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    int result = setTimezone(addon_data->isPermanent, addon_data->strTimezone);
    // 调用js-callback函数
    catch_error_get_datetime(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        result,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value set_timezone(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 3;
    napi_value argv[3];
    catch_error_get_datetime(env, napi_get_cb_info(
        env,
        info,
        &argc,
        argv,
        NULL,
        NULL
    ));
    if (argc != 3)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
    //判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[2], &valueTypeLast);

    if (valueTypeLast != napi_function) {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // 创建线程名字
    catch_error_get_datetime(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    //获取参数(int isPermanent, int enableNTPServer, char* strServerAddress, char* strDatetime)
    size_t strLength;
    // 分配内存空间，在work_complete中会释放
    AddonData_set_timezone* addon_data = (AddonData_set_timezone*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[0], &addon_data->isPermanent));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    addon_data->strTimezone = (char*)malloc(sizeof(char) * (++strLength));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[1], addon_data->strTimezone, strLength, &strLength));
    //isPermanent
    int isPermanent;
    //strTimezone

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_get_datetime(env, napi_create_threadsafe_function(
        env,
        argv[2],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_set_timezone,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_get_datetime(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_set_timezone,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_set_timezone,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_get_datetime(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}
/****************************set_daylight_saving***************************************/
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
} AddonData_set_daylight_saving;

 napi_value set_daylight_saving_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 11;
    napi_value argv[11];
    catch_error_get_datetime(env, napi_get_cb_info(
        env,
        info,
        &argc,
        argv,
        NULL,
        NULL
    ));
    if (argc != 11)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
    //获取参数(int isdst, int offsetSeconds, int startMonth, int startWeek, int startDay, char* startTime,
    //int endMonth, int endWeek, int endDay, char* endTime)
    size_t strLength;
    //isdst
    int isdst;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[0], &isdst));
    //offsetSeconds
    int offsetSeconds;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[1], &offsetSeconds));
    //startMonth
    int startMonth;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[2], &startMonth));
    //startWeek
    int startWeek;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[3], &startWeek));
    //startDay
    int startDay;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[4], &startDay));
    //strTimezone
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[5], NULL, NULL, &strLength));
    char* startTime = (char*)malloc((++strLength) * sizeof(char));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[5], startTime, strLength, &strLength));
    //endMonth
    int endMonth;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[6], &endMonth));
    //endWeek
    int endWeek;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[7], &endWeek));
    //endDay
    int endDay;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[8], &endDay));
    //endTime
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[9], NULL, NULL, &strLength));
    char* endTime = (char*)malloc((++strLength) * sizeof(char));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[9], endTime, strLength, &strLength));
    //执行so函数
    int result = setDaylightSaving(isdst,offsetSeconds,startMonth, startWeek, startDay, startTime,endMonth, endWeek, endDay, endTime);
    //转类型
    napi_value world;
    catch_error_get_datetime(env, napi_create_int32(env, result, &world));
    //回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[10], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[10], callbackArgc, callbackParams, &callbackRs);
    }
    free(startTime);
    free(endTime);
    return world;
}

void work_complete_set_daylight_saving(napi_env env, napi_status status, void* data) {
    AddonData_set_daylight_saving* addon_data = (AddonData_set_daylight_saving*)data;

    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_get_datetime(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    free(addon_data->startTime);
    free(addon_data->endTime);
    free(addon_data);
}

void call_js_callback_set_daylight_saving(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_get_datetime(env, napi_create_int32(
            env,
            data,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_get_datetime(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_get_datetime(env, napi_call_function(
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
 * 执行线程
*/
 void execute_work_set_daylight_saving(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_set_daylight_saving* addon_data = (AddonData_set_daylight_saving*)data;
    //获取js-callback函数
    catch_error_get_datetime(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    int result = setDaylightSaving(addon_data->isdst, addon_data->offsetSeconds, addon_data->startMonth, addon_data->startWeek, addon_data->startDay, addon_data->startTime,
        addon_data->endMonth, addon_data->endWeek, addon_data->endDay, addon_data->endTime);
    // 调用js-callback函数
    catch_error_get_datetime(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        result,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value set_daylight_saving(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 11;
    napi_value argv[11];
    catch_error_get_datetime(env, napi_get_cb_info(
        env,
        info,
        &argc,
        argv,
        NULL,
        NULL
    ));
    if (argc != 11)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
    //判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[10], &valueTypeLast);

    if (valueTypeLast != napi_function) {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // 创建线程名字
    catch_error_get_datetime(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    //获取参数(int isdst, int offsetSeconds, int startMonth, int startWeek, int startDay, char* startTime,
//int endMonth, int endWeek, int endDay, char* endTime)
    size_t strLength;
    // 分配内存空间，在work_complete中会释放
    AddonData_set_daylight_saving* addon_data = (AddonData_set_daylight_saving*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[0], &addon_data->isdst));
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[1], &addon_data->offsetSeconds));
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[2], &addon_data->startMonth));
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[3], &addon_data->startWeek));
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[4], &addon_data->startDay));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[5], NULL, NULL, &strLength));
    addon_data->startTime = (char*)malloc(sizeof(char) * (++strLength));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[5], addon_data->startTime, strLength, &strLength));
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[6], &addon_data->endMonth));
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[7], &addon_data->endWeek));
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[8], &addon_data->endDay));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[9], NULL, NULL, &strLength));
    addon_data->endTime = (char*)malloc(sizeof(char) * (++strLength));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[9], addon_data->endTime, strLength, &strLength));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_get_datetime(env, napi_create_threadsafe_function(
        env,
        argv[10],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_set_daylight_saving,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_get_datetime(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_set_daylight_saving,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_set_daylight_saving,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_get_datetime(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}
/****************************set_city_datetime***************************************/
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
} AddonData_set_city_datetime;

 napi_value set_city_datetime_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 12;
    napi_value argv[12];
    catch_error_get_datetime(env, napi_get_cb_info(
        env,
        info,
        &argc,
        argv,
        NULL,
        NULL
    ));
    if (argc != 12)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
    //获取参数(char* strCityId, int isDstEnabled, int offsetSeconds, int startMonth, int startWeek, int startDay, char* startTime,
    //int endMonth, int endWeek, int endDay, char* endTime)
    size_t strLength;
    //strCityId
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* strCityId = (char*)malloc((++strLength) * sizeof(char));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[0], strCityId, strLength, &strLength));
    //isDstEnabled
    int isDstEnabled;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[1], &isDstEnabled));
    //offsetSeconds
    int offsetSeconds;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[2], &offsetSeconds));
    //startMonth
    int startMonth;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[3], &startMonth));
    //startWeek
    int startWeek;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[4], &startWeek));
    //startDay
    int startDay;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[5], &startDay));
    //strTimezone
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[6], NULL, NULL, &strLength));
    char* startTime = (char*)malloc((++strLength) * sizeof(char));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[6], startTime, strLength, &strLength));
    //endMonth
    int endMonth;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[7], &endMonth));
    //endWeek
    int endWeek;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[8], &endWeek));
    //endDay
    int endDay;
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[9], &endDay));
    //endTime
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[10], NULL, NULL, &strLength));
    char* endTime = (char*)malloc((++strLength) * sizeof(char));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[10], endTime, strLength, &strLength));
    //执行so函数
    int result = setCityDatetime(strCityId, isDstEnabled, offsetSeconds, startMonth, startWeek, startDay, startTime, endMonth, endWeek, endDay, endTime);
    //转类型
    napi_value world;
    catch_error_get_datetime(env, napi_create_int32(env, result, &world));
    //回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[11], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[11], callbackArgc, callbackParams, &callbackRs);
    }
    free(strCityId);
    free(startTime);
    free(endTime);
    return world;
}

void work_complete_set_city_datetime(napi_env env, napi_status status, void* data) {
    AddonData_set_city_datetime* addon_data = (AddonData_set_city_datetime*)data;

    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_get_datetime(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    free(addon_data->strCityId);
    free(addon_data->startTime);
    free(addon_data->endTime);
    free(addon_data);
}

void call_js_callback_set_city_datetime(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_get_datetime(env, napi_create_int32(
            env,
            data,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_get_datetime(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_get_datetime(env, napi_call_function(
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
 * 执行线程
*/
 void execute_work_set_city_datetime(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_set_city_datetime* addon_data = (AddonData_set_city_datetime*)data;
    //获取js-callback函数
    catch_error_get_datetime(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    int result = setCityDatetime(addon_data->strCityId,addon_data->isDstEnabled, addon_data->offsetSeconds, addon_data->startMonth, addon_data->startWeek, addon_data->startDay, addon_data->startTime,
        addon_data->endMonth, addon_data->endWeek, addon_data->endDay, addon_data->endTime);
    // 调用js-callback函数
    catch_error_get_datetime(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        result,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value set_city_datetime(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 12;
    napi_value argv[12];
    catch_error_get_datetime(env, napi_get_cb_info(
        env,
        info,
        &argc,
        argv,
        NULL,
        NULL
    ));
    if (argc != 12)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
    //判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[11], &valueTypeLast);

    if (valueTypeLast != napi_function) {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // 创建线程名字
    catch_error_get_datetime(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    //获取参数(char* strCityId, int isDstEnabled, int offsetSeconds, int startMonth, int startWeek, int startDay, char* startTime,
     //int endMonth, int endWeek, int endDay, char* endTime)
    size_t strLength;
    // 分配内存空间，在work_complete中会释放
    AddonData_set_city_datetime* addon_data = (AddonData_set_city_datetime*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    addon_data->strCityId = (char*)malloc(sizeof(char) * (++strLength));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[0], addon_data->strCityId, strLength, &strLength));
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[1], &addon_data->isDstEnabled));
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[2], &addon_data->offsetSeconds));
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[3], &addon_data->startMonth));
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[4], &addon_data->startWeek));
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[5], &addon_data->startDay));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[6], NULL, NULL, &strLength));
    addon_data->startTime = (char*)malloc(sizeof(char) * (++strLength));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[6], addon_data->startTime, strLength, &strLength));
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[7], &addon_data->endMonth));
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[8], &addon_data->endWeek));
    catch_error_get_datetime(env, napi_get_value_int32(env, argv[9], &addon_data->endDay));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[10], NULL, NULL, &strLength));
    addon_data->endTime = (char*)malloc(sizeof(char) * (++strLength));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[10], addon_data->endTime, strLength, &strLength));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_get_datetime(env, napi_create_threadsafe_function(
        env,
        argv[11],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_set_city_datetime,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_get_datetime(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_set_city_datetime,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_set_city_datetime,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_get_datetime(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}
/*********************************getCityDst**********************************************************/
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
    char* strCityId;
} AddonData_get_city_dst;

 napi_value get_city_dst_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 2;
    napi_value argv[2];
    catch_error_get_datetime(env, napi_get_cb_info(
        env,
        info,
        &argc,
        argv,
        NULL,
        NULL
    ));
    if (argc != 2)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
    //获取参数
    size_t strLength;
    //strCityId
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* strCityId = (char*)malloc((++strLength) * sizeof(char));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[0], strCityId, strLength, &strLength));
    //执行so函数
    char* result = getCityDst(strCityId);
    //转类型
    napi_value world;
    catch_error_get_datetime(env, napi_create_string_utf8(env, result, NAPI_AUTO_LENGTH, &world));
    //回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[1], &valueTypeLast);
    if (valueTypeLast == napi_function) {
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[1], callbackArgc, callbackParams, &callbackRs);
    }
    free(strCityId);
    return world;
}

void work_complete_get_city_dst(napi_env env, napi_status status, void* data) {
    AddonData_get_city_dst* addon_data = (AddonData_get_city_dst*)data;

    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_get_datetime(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    free(addon_data->strCityId);
    free(addon_data);
}

void call_js_callback_get_city_dst(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_get_datetime(env, napi_create_string_utf8(
            env,
            (const char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_get_datetime(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_get_datetime(env, napi_call_function(
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
        free((char*)data);
    }
}
/**
 * 执行线程
*/
 void execute_work_get_city_dst(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_get_city_dst* addon_data = (AddonData_get_city_dst*)data;
    //获取js-callback函数
    catch_error_get_datetime(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    char* result = getCityDst(addon_data->strCityId);
    char* rs = (char*)malloc(sizeof(char) * (strlen(result) + 1));
    strcpy(rs, result);
    // 调用js-callback函数
    catch_error_get_datetime(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        rs,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_get_datetime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value get_city_dst(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 2;
    napi_value argv[2];
    catch_error_get_datetime(env, napi_get_cb_info(
        env,
        info,
        &argc,
        argv,
        NULL,
        NULL
    ));
    if (argc != 2)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
    //判断是否存在回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[1], &valueTypeLast);

    if (valueTypeLast != napi_function) {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    // 创建线程名字
    catch_error_get_datetime(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    //获取参数
    size_t strLength;
    AddonData_get_city_dst* addon_data = (AddonData_get_city_dst*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    addon_data->strCityId = (char*)malloc(sizeof(char) * (++strLength));
    catch_error_get_datetime(env, napi_get_value_string_utf8(env, argv[0], addon_data->strCityId, strLength, &strLength));
    // 分配内存空间，在work_complete中会释放
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_get_datetime(env, napi_create_threadsafe_function(
        env,
        argv[1],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_get_city_dst,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_get_datetime(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_get_city_dst,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_get_city_dst,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_get_datetime(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}
//
///****************************init***************************************/
//napi_value init(napi_env env, napi_value exports)
//{
//    /******************************getDatetimeSync***********************************************************/
//    // 获取性能数据
//    napi_property_descriptor getDatetimeSync = {
//        "getDatetimeSync",
//        NULL,
//        get_datetime_sync,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &getDatetimeSync
//    );
//
//    // 获取性能数据
//    napi_property_descriptor getDatetime = {
//        "getDatetime",
//        NULL,
//        get_datetime,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &getDatetime
//    );
//    /******************************getTimezone***********************************************************/
//    // 获取性能数据
//    napi_property_descriptor getTimezoneSync = {
//        "getTimezoneSync",
//        NULL,
//        get_timezone_sync,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &getTimezoneSync
//    );
//
//    // 获取性能数据
//    napi_property_descriptor getTimezone = {
//        "getTimezone",
//        NULL,
//        get_timezone,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &getTimezone
//    );
//    /******************************getDaylightSaving***********************************************************/
//   // 获取性能数据
//    napi_property_descriptor getDaylightSavingSync = {
//        "getDaylightSavingSync",
//        NULL,
//        get_daylight_saving_sync,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &getDaylightSavingSync
//    );
//
//    // 获取性能数据
//    napi_property_descriptor getDaylightSaving = {
//        "getDaylightSaving",
//        NULL,
//        get_daylight_saving,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &getDaylightSaving
//    );
//    /******************************getCityDatetime***********************************************************/
//   // 获取性能数据
//    napi_property_descriptor getCityDatetimeSync = {
//        "getCityDatetimeSync",
//        NULL,
//        get_city_datetime_sync,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &getCityDatetimeSync
//    );
//
//    // 获取性能数据
//    napi_property_descriptor getCityDatetime = {
//        "getCityDatetime",
//        NULL,
//        get_city_datetime,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &getCityDatetime
//    );
//    /******************************setDatetime***********************************************************/
//  // 获取性能数据
//    napi_property_descriptor setDatetimeSync = {
//        "setDatetimeSync",
//        NULL,
//        set_datetime_sync,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &setDatetimeSync
//    );
//
//    // 获取性能数据
//    napi_property_descriptor setDatetime = {
//        "setDatetime",
//        NULL,
//        set_datetime,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &setDatetime
//    );
//    /******************************setTimezone***********************************************************/
//// 获取性能数据
//    napi_property_descriptor setTimezoneSync = {
//        "setTimezoneSync",
//        NULL,
//        set_timezone_sync,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &setTimezoneSync
//    );
//
//    // 获取性能数据
//    napi_property_descriptor setTimezone = {
//        "setTimezone",
//        NULL,
//        set_timezone,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &setTimezone
//    );
//    /******************************setDaylightSaving***********************************************************/
//// 获取性能数据
//    napi_property_descriptor setDaylightSavingSync = {
//        "setDaylightSavingSync",
//        NULL,
//        set_daylight_saving_sync,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &setDaylightSavingSync
//    );
//
//    // 获取性能数据
//    napi_property_descriptor setDaylightSaving = {
//        "setDaylightSaving",
//        NULL,
//        set_daylight_saving,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &setDaylightSaving
//    );
//    /******************************setCityDatetime***********************************************************/
//    // 获取性能数据
//    napi_property_descriptor setCityDatetimeSync = {
//        "setCityDatetimeSync",
//        NULL,
//        set_city_datetime_sync,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &setCityDatetimeSync
//    );
//
//    // 获取性能数据
//    napi_property_descriptor setCityDatetime = {
//        "setCityDatetime",
//        NULL,
//        set_city_datetime,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &setCityDatetime
//    );
//    /******************************getCityDst***********************************************************/
//// 获取性能数据
//    napi_property_descriptor getCityDstSync = {
//        "getCityDstSync",
//        NULL,
//        get_city_dst_sync,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &getCityDstSync
//    );
//
//    // 获取性能数据
//    napi_property_descriptor getCityDst = {
//        "getCityDst",
//        NULL,
//        get_city_dst,
//        NULL,
//        NULL,
//        NULL,
//        napi_default,
//        NULL
//    };
//    //暴露接口
//    napi_define_properties(
//        env,
//        exports,
//        1,
//        &getCityDst
//    );
//    return exports;
//}
//
//NAPI_MODULE(NODE_GYP_MODULE_NAME, init);
