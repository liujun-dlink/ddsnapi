#include "node_api.h"
#include <stdlib.h>
#include "libFtpSetting.h"
#include <string.h>


void catch_error_ftp_setting(napi_env env, napi_status status) {
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

/**********************getFtpData**********************************/
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
} AddonData_ftp_data;

void work_complete_ftp_data(napi_env env, napi_status status, void* data) {
    AddonData_ftp_data* addon_data = (AddonData_ftp_data*)data;

    //释放句柄
    catch_error_ftp_setting(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_ftp_setting(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    free(addon_data);
}

void call_js_callback_ftp_data(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_ftp_setting(env, napi_create_string_utf8(
            env,
            (const char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_ftp_setting(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_ftp_setting(env, napi_call_function(
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

 napi_value get_ftp_data_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_ftp_setting(env, napi_get_cb_info(
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
    char* time = getFtpData();
    //转类型
    napi_value world;
    catch_error_ftp_setting(env, napi_create_string_utf8(env, time, NAPI_AUTO_LENGTH, &world));
    //回调函数
    napi_value* callbackParams = &world;
    size_t callbackArgc = 1;
    napi_value global;
    napi_get_global(env, &global);
    napi_value callbackRs;
    napi_call_function(env, global, argv[0], callbackArgc, callbackParams, &callbackRs);
    return world;
}

/**
 * 执行线程
*/
 void execute_work_ftp_data(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_ftp_data* addon_data = (AddonData_ftp_data*)data;
    //获取js-callback函数
    catch_error_ftp_setting(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    char* time = getFtpData();
    char* uptime = (char*)malloc((strlen(time) + 1) * sizeof(char));
    //uptime = time;
    strcpy(uptime, time);
    // 调用js-callback函数
    catch_error_ftp_setting(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        uptime,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_ftp_setting(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value get_ftp_data(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_ftp_setting(env, napi_get_cb_info(
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
    // 创建线程名字
    catch_error_ftp_setting(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 分配内存空间，在work_complete中会释放
    AddonData_ftp_data* addon_data = (AddonData_ftp_data*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_ftp_setting(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_ftp_data,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_ftp_setting(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_ftp_data,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_ftp_data,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_ftp_setting(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}
/**********************setFtpData**********************************/

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
    char* ftpServer;
    int ftpPort;
    char* ftpUsername;
    char* ftpPassword;
} AddonData_set_ftp_data;

 napi_value set_ftp_data_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 5;
    napi_value argv[5];
    catch_error_ftp_setting(env, napi_get_cb_info(
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
    //获取参数
    size_t strLength;
    //ftpServer
    catch_error_ftp_setting(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* ftpServer = (char*)malloc((++strLength) * sizeof(char));
    catch_error_ftp_setting(env, napi_get_value_string_utf8(env, argv[0], ftpServer, strLength, &strLength));
    //ftpPort
    int ftpPort;
    catch_error_ftp_setting(env, napi_get_value_int32(env, argv[1], &ftpPort));
    //ftpUsername
    catch_error_ftp_setting(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    char* ftpUsername = (char*)malloc((++strLength) * sizeof(char));
    catch_error_ftp_setting(env, napi_get_value_string_utf8(env, argv[2], ftpUsername, strLength, &strLength));
    //ftpServer
    catch_error_ftp_setting(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    char* ftpPassword = (char*)malloc((++strLength) * sizeof(char));
    catch_error_ftp_setting(env, napi_get_value_string_utf8(env, argv[3], ftpPassword, strLength, &strLength));
    //执行so函数
    int time = setFtpData(ftpServer, ftpPort, ftpUsername, ftpPassword);
    //转类型
    napi_value world;
    catch_error_ftp_setting(env, napi_create_int32(env, time, &world));
    //回调函数
    napi_value* callbackParams = &world;
    size_t callbackArgc = 1;
    napi_value global;
    napi_get_global(env, &global);
    napi_value callbackRs;
    napi_call_function(env, global, argv[4], callbackArgc, callbackParams, &callbackRs);
    free(ftpServer);
    free(ftpUsername);
    free(ftpPassword);
    return world;
}

void work_complete_set_ftp_data(napi_env env, napi_status status, void* data) {
    AddonData_set_ftp_data* addon_data = (AddonData_set_ftp_data*)data;

    //释放句柄
    catch_error_ftp_setting(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_ftp_setting(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    free(addon_data->ftpServer);
    free(addon_data->ftpUsername);
    free(addon_data->ftpPassword);
    free(addon_data);
}

void call_js_callback_set_ftp_data(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_ftp_setting(env, napi_create_int32(
            env,
            data,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_ftp_setting(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_ftp_setting(env, napi_call_function(
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
 void execute_work_set_ftp_data(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_set_ftp_data* addon_data = (AddonData_set_ftp_data*)data;
    //获取js-callback函数
    catch_error_ftp_setting(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    int result = setFtpData(addon_data->ftpServer, addon_data->ftpPort, addon_data->ftpUsername, addon_data->ftpPassword);
    // 调用js-callback函数
    catch_error_ftp_setting(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        result,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_ftp_setting(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value set_ftp_data(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 5;
    napi_value argv[5];
    catch_error_ftp_setting(env, napi_get_cb_info(
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
    //获取参数
    // 分配内存空间，在work_complete中会释放
    size_t strLength;
    AddonData_set_ftp_data* addon_data = (AddonData_set_ftp_data*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    catch_error_ftp_setting(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    addon_data->ftpServer = (char*)malloc((++strLength) * sizeof(char));
    catch_error_ftp_setting(env, napi_get_value_string_utf8(env, argv[0], addon_data->ftpServer, strLength, &strLength));
    catch_error_ftp_setting(env, napi_get_value_int32(env, argv[1], &addon_data->ftpPort));
    catch_error_ftp_setting(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    addon_data->ftpUsername = (char*)malloc((++strLength) * sizeof(char));
    catch_error_ftp_setting(env, napi_get_value_string_utf8(env, argv[2], addon_data->ftpUsername, strLength, &strLength));
    catch_error_ftp_setting(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    addon_data->ftpPassword = (char*)malloc((++strLength) * sizeof(char));
    catch_error_ftp_setting(env, napi_get_value_string_utf8(env, argv[3], addon_data->ftpPassword, strLength, &strLength));
    // 创建线程名字
    catch_error_ftp_setting(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_ftp_setting(env, napi_create_threadsafe_function(
        env,
        argv[4],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_set_ftp_data,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_ftp_setting(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_set_ftp_data,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_set_ftp_data,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_ftp_setting(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}