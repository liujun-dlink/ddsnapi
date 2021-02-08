#include "node_api.h"
#include <stdlib.h>
#include "libDB.h"
#include <string.h>

void catch_error_libDB(napi_env env, napi_status status) {
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
/***************************getHWVersion**************************/
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
} AddonData_get_hw_version;


void work_complete_get_hw_version(napi_env env, napi_status status, void* data) {
    AddonData_get_hw_version* addon_data = (AddonData_get_hw_version*)data;

    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_libDB(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    //free(addon_data->libPath);
    free(addon_data);
}

void call_js_callback_get_hw_version(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_libDB(env, napi_create_string_utf8(
            env,
            (const char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_libDB(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_libDB(env, napi_call_function(
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

 napi_value get_hw_version_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    char* version = getHWVersion();
    //转类型
    napi_value world;
    catch_error_libDB(env, napi_create_string_utf8(env, version, NAPI_AUTO_LENGTH, &world));
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
 void execute_work_get_hw_version(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_get_hw_version* addon_data = (AddonData_get_hw_version*)data;
    //获取js-callback函数
    catch_error_libDB(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    char* version = getHWVersion();
    char* v = (char*)malloc(sizeof(char) * (strlen(version) + 1));
    strcpy(v, version);
    // 调用js-callback函数
    catch_error_libDB(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        v,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value get_hw_version(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    catch_error_libDB(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 分配内存空间，在work_complete中会释放
    AddonData_get_hw_version* addon_data = (AddonData_get_hw_version*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_libDB(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_get_hw_version,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_libDB(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_get_hw_version,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_get_hw_version,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_libDB(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}

/***************************getNCVersion**************************/
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
} AddonData_get_nc_version;


void work_complete_get_nc_version(napi_env env, napi_status status, void* data) {
    AddonData_get_nc_version* addon_data = (AddonData_get_nc_version*)data;

    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_libDB(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    //free(addon_data->libPath);
    free(addon_data);
}

void call_js_callback_get_nc_version(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_libDB(env, napi_create_string_utf8(
            env,
            (const char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_libDB(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_libDB(env, napi_call_function(
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

 napi_value get_nc_version_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    char* version = getNCVersion();
    //转类型
    napi_value world;
    catch_error_libDB(env, napi_create_string_utf8(env, version, NAPI_AUTO_LENGTH, &world));
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
 void execute_work_get_nc_version(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_get_nc_version* addon_data = (AddonData_get_nc_version*)data;
    //获取js-callback函数
    catch_error_libDB(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    char* version = getNCVersion();
    char* v = (char*)malloc(sizeof(char) * (strlen(version) + 1));
    strcpy(v, version);
    // 调用js-callback函数
    catch_error_libDB(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        v,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value get_nc_version(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    catch_error_libDB(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 分配内存空间，在work_complete中会释放
    AddonData_get_nc_version* addon_data = (AddonData_get_nc_version*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_libDB(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_get_nc_version,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_libDB(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_get_nc_version,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_get_nc_version,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_libDB(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}

/***************************get_fota_setting**************************/
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
} AddonData_get_fota_setting;


void work_complete_get_fota_setting(napi_env env, napi_status status, void* data) {
    AddonData_get_fota_setting* addon_data = (AddonData_get_fota_setting*)data;

    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_libDB(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    //free(addon_data->libPath);
    free(addon_data);
}

void call_js_callback_get_fota_setting(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_libDB(env, napi_create_string_utf8(
            env,
            (const char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_libDB(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_libDB(env, napi_call_function(
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

 napi_value get_fota_setting_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    char* result = getFOTASetting();
    //转类型
    napi_value world;
    catch_error_libDB(env, napi_create_string_utf8(env, result, NAPI_AUTO_LENGTH, &world));
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
 void execute_work_get_fota_setting(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_get_fota_setting* addon_data = (AddonData_get_fota_setting*)data;
    //获取js-callback函数
    catch_error_libDB(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    char* result = getFOTASetting();
    char* rs = (char*)malloc(sizeof(char) * (strlen(result) + 1));
    strcpy(rs, result);
    // 调用js-callback函数
    catch_error_libDB(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        rs,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value get_fota_setting(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    catch_error_libDB(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 分配内存空间，在work_complete中会释放
    AddonData_get_fota_setting* addon_data = (AddonData_get_fota_setting*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_libDB(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_get_fota_setting,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_libDB(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_get_fota_setting,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_get_fota_setting,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_libDB(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}

/***************************get_firmware_version**************************/
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
} AddonData_get_firmware_version;


void work_complete_get_firmware_version(napi_env env, napi_status status, void* data) {
    AddonData_get_firmware_version* addon_data = (AddonData_get_firmware_version*)data;

    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_libDB(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    //free(addon_data->libPath);
    free(addon_data);
}

void call_js_callback_get_firmware_version(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_libDB(env, napi_create_string_utf8(
            env,
            (const char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_libDB(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_libDB(env, napi_call_function(
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

 napi_value get_firmware_version_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    char* result = getFirmwareVersion();
    //转类型
    napi_value world;
    catch_error_libDB(env, napi_create_string_utf8(env, result, NAPI_AUTO_LENGTH, &world));
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
 void execute_work_get_firmware_version(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_get_firmware_version* addon_data = (AddonData_get_firmware_version*)data;
    //获取js-callback函数
    catch_error_libDB(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    char* result = getFirmwareVersion();
    char* rs = (char*)malloc(sizeof(char) * (strlen(result) + 1));
    strcpy(rs, result);
    // 调用js-callback函数
    catch_error_libDB(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        rs,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value get_firmware_version(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    catch_error_libDB(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 分配内存空间，在work_complete中会释放
    AddonData_get_firmware_version* addon_data = (AddonData_get_firmware_version*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_libDB(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_get_firmware_version,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_libDB(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_get_firmware_version,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_get_firmware_version,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_libDB(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}
/***************************getFirmwareVersionFull**************************/
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
} AddonData_get_firmware_version_full;


void work_complete_get_firmware_version_full(napi_env env, napi_status status, void* data) {
    AddonData_get_firmware_version_full* addon_data = (AddonData_get_firmware_version_full*)data;

    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_libDB(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    //free(addon_data->libPath);
    free(addon_data);
}

void call_js_callback_get_firmware_version_full(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_libDB(env, napi_create_string_utf8(
            env,
            (const char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_libDB(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_libDB(env, napi_call_function(
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

 napi_value get_firmware_version_full_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    char* result = getFirmwareVersionFull();
    //转类型
    napi_value world;
    catch_error_libDB(env, napi_create_string_utf8(env, result, NAPI_AUTO_LENGTH, &world));
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
 void execute_work_get_firmware_version_full(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_get_firmware_version_full* addon_data = (AddonData_get_firmware_version_full*)data;
    //获取js-callback函数
    catch_error_libDB(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    char* result = getFirmwareVersionFull();
    char* rs = (char*)malloc(sizeof(char) * (strlen(result) + 1));
    strcpy(rs, result);
    // 调用js-callback函数
    catch_error_libDB(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        rs,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value get_firmware_version_full(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    catch_error_libDB(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 分配内存空间，在work_complete中会释放
    AddonData_get_firmware_version_full* addon_data = (AddonData_get_firmware_version_full*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_libDB(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_get_firmware_version_full,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_libDB(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_get_firmware_version_full,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_get_firmware_version_full,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_libDB(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}
/***************************get_firmware_info**************************/
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
} AddonData_get_firmware_info;


void work_complete_get_firmware_info(napi_env env, napi_status status, void* data) {
    AddonData_get_firmware_info* addon_data = (AddonData_get_firmware_info*)data;

    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_libDB(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    //free(addon_data->libPath);
    free(addon_data);
}

void call_js_callback_get_firmware_info(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_libDB(env, napi_create_string_utf8(
            env,
            (const char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_libDB(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_libDB(env, napi_call_function(
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

 napi_value get_firmware_info_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    char* result = getFirmwareInfo();
    //转类型
    napi_value world;
    catch_error_libDB(env, napi_create_string_utf8(env, result, NAPI_AUTO_LENGTH, &world));
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
 void execute_work_get_firmware_info(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_get_firmware_info* addon_data = (AddonData_get_firmware_info*)data;
    //获取js-callback函数
    catch_error_libDB(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    char* result = getFirmwareInfo();
    char* rs = (char*)malloc(sizeof(char) * (strlen(result) + 1));
    strcpy(rs, result);
    // 调用js-callback函数
    catch_error_libDB(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        rs,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value get_firmware_info(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    catch_error_libDB(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 分配内存空间，在work_complete中会释放
    AddonData_get_firmware_info* addon_data = (AddonData_get_firmware_info*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_libDB(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_get_firmware_info,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_libDB(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_get_firmware_info,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_get_firmware_info,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_libDB(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}

/***************************getDeviceUUID**************************/
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
} AddonData_get_device_uuid;


void work_complete_get_device_uuid(napi_env env, napi_status status, void* data) {
    AddonData_get_device_uuid* addon_data = (AddonData_get_device_uuid*)data;

    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_libDB(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    //free(addon_data->libPath);
    free(addon_data);
}

void call_js_callback_get_device_uuid(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_libDB(env, napi_create_string_utf8(
            env,
            (const char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_libDB(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_libDB(env, napi_call_function(
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

 napi_value get_device_uuid_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    char* result = getDeviceUUID();
    //转类型
    napi_value world;
    catch_error_libDB(env, napi_create_string_utf8(env, result, NAPI_AUTO_LENGTH, &world));
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
 void execute_work_get_device_uuid(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_get_device_uuid* addon_data = (AddonData_get_device_uuid*)data;
    //获取js-callback函数
    catch_error_libDB(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    char* result = getDeviceUUID();
    char* rs = (char*)malloc(sizeof(char) * (strlen(result) + 1));
    strcpy(rs, result);
    // 调用js-callback函数
    catch_error_libDB(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        rs,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value get_device_uuid(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    catch_error_libDB(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 分配内存空间，在work_complete中会释放
    AddonData_get_device_uuid* addon_data = (AddonData_get_device_uuid*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_libDB(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_get_device_uuid,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_libDB(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_get_device_uuid,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_get_device_uuid,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_libDB(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}

/***************************get_firmware_upgrade_status**************************/
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
} AddonData_get_firmware_upgrade_status;


void work_complete_get_firmware_upgrade_status(napi_env env, napi_status status, void* data) {
    AddonData_get_firmware_upgrade_status* addon_data = (AddonData_get_firmware_upgrade_status*)data;

    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_libDB(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    //free(addon_data->libPath);
    free(addon_data);
}

void call_js_callback_get_firmware_upgrade_status(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_libDB(env, napi_create_int32(
            env,
            data,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_libDB(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_libDB(env, napi_call_function(
            env,
            undefined,   // js回调的this对象
            js_cb,       // js回调函数句柄
            1,           // js回调函数接受参数个数
            &callBackInParams,        // js回调函数参数数组
            NULL         // js回调函数中如果有return，将会被result接受到
        ));
    }
}

 napi_value get_firmware_upgrade_status_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    int result = getFirmwareUpgradeStatus();
    //转类型
    napi_value world;
    catch_error_libDB(env, napi_create_int32(env, result, &world));
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
 void execute_work_get_firmware_upgrade_status(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_get_firmware_upgrade_status* addon_data = (AddonData_get_firmware_upgrade_status*)data;
    //获取js-callback函数
    catch_error_libDB(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    int result = getFirmwareUpgradeStatus();
    //char* rs = (char*)malloc(sizeof(strlen(result)));
    //strcpy(rs, result);
    // 调用js-callback函数
    catch_error_libDB(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        result,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value get_firmware_upgrade_status(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    catch_error_libDB(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 分配内存空间，在work_complete中会释放
    AddonData_get_firmware_upgrade_status* addon_data = (AddonData_get_firmware_upgrade_status*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_libDB(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_get_firmware_upgrade_status,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_libDB(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_get_firmware_upgrade_status,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_get_firmware_upgrade_status,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_libDB(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}

/***************************get_fota_update_status**************************/
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
} AddonData_get_fota_update_status;


void work_complete_get_fota_update_status(napi_env env, napi_status status, void* data) {
    AddonData_get_fota_update_status* addon_data = (AddonData_get_fota_update_status*)data;

    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_libDB(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    //free(addon_data->libPath);
    free(addon_data);
}

void call_js_callback_get_fota_update_status(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_libDB(env, napi_create_int32(
            env,
            data,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_libDB(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_libDB(env, napi_call_function(
            env,
            undefined,   // js回调的this对象
            js_cb,       // js回调函数句柄
            1,           // js回调函数接受参数个数
            &callBackInParams,        // js回调函数参数数组
            NULL         // js回调函数中如果有return，将会被result接受到
        ));
    }
}

 napi_value get_fota_update_status_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    int result = getFOTAUpdateStatus();
    //转类型
    napi_value world;
    catch_error_libDB(env, napi_create_int32(env, result, &world));
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
 void execute_work_get_fota_update_status(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_get_fota_update_status* addon_data = (AddonData_get_fota_update_status*)data;
    //获取js-callback函数
    catch_error_libDB(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    int result = getFOTAUpdateStatus();
    //char* rs = (char*)malloc(sizeof(strlen(result)));
    //strcpy(rs, result);
    // 调用js-callback函数
    catch_error_libDB(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        result,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value get_fota_update_status(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 1;
    napi_value argv[1];
    catch_error_libDB(env, napi_get_cb_info(
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
    catch_error_libDB(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 分配内存空间，在work_complete中会释放
    AddonData_get_fota_update_status* addon_data = (AddonData_get_fota_update_status*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_libDB(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_get_fota_update_status,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_libDB(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_get_fota_update_status,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_get_fota_update_status,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_libDB(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}

/***************************setFOTASetting**************************/
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
    int nEnable;
    int nWeekday;
    int nHour;
    int nMinute;
    int nUpdateBetaFw;
} AddonData_set_fota_setting;


void work_complete_set_fota_setting(napi_env env, napi_status status, void* data) {
    AddonData_set_fota_setting* addon_data = (AddonData_set_fota_setting*)data;

    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_libDB(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    //free(addon_data->libPath);
    free(addon_data);
}

void call_js_callback_set_fota_setting(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_libDB(env, napi_create_int32(
            env,
            data,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_libDB(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_libDB(env, napi_call_function(
            env,
            undefined,   // js回调的this对象
            js_cb,       // js回调函数句柄
            1,           // js回调函数接受参数个数
            &callBackInParams,        // js回调函数参数数组
            NULL         // js回调函数中如果有return，将会被result接受到
        ));
    }
}

 napi_value set_fota_setting_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 6;
    napi_value argv[6];
    catch_error_libDB(env, napi_get_cb_info(
        env,
        info,
        &argc,
        argv,
        NULL,
        NULL
    ));
    if (argc != 6)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
    //nEnable
    int nEnable;
    catch_error_libDB(env, napi_get_value_int32(env, argv[0], &nEnable));
    //nWeekday
    int nWeekday;
    catch_error_libDB(env, napi_get_value_int32(env, argv[1], &nWeekday));
    //nHour
    int nHour;
    catch_error_libDB(env, napi_get_value_int32(env, argv[2], &nHour));
    //nMinute
    int nMinute;
    catch_error_libDB(env, napi_get_value_int32(env, argv[3], &nMinute));
    //nUpdateBetaFw
    int nUpdateBetaFw;
    catch_error_libDB(env, napi_get_value_int32(env, argv[4], &nUpdateBetaFw));
    //执行so函数
    int result = setFOTASetting(nEnable, nWeekday, nHour, nMinute, nUpdateBetaFw);
    //转类型
    napi_value world;
    catch_error_libDB(env, napi_create_int32(env, result, &world));
    //回调函数
    napi_value* callbackParams = &world;
    size_t callbackArgc = 1;
    napi_value global;
    napi_get_global(env, &global);
    napi_value callbackRs;
    napi_call_function(env, global, argv[5], callbackArgc, callbackParams, &callbackRs);
    return world;
}

/**
 * 执行线程
*/
 void execute_work_set_fota_setting(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_set_fota_setting* addon_data = (AddonData_set_fota_setting*)data;
    //获取js-callback函数
    catch_error_libDB(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    int result = setFOTASetting(addon_data->nEnable, addon_data->nWeekday, addon_data->nHour, addon_data->nMinute, addon_data->nUpdateBetaFw);
    //char* rs = (char*)malloc(sizeof(strlen(result)));
    //strcpy(rs, result);
    // 调用js-callback函数
    catch_error_libDB(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        result,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value set_fota_setting(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 6;
    napi_value argv[6];
    catch_error_libDB(env, napi_get_cb_info(
        env,
        info,
        &argc,
        argv,
        NULL,
        NULL
    ));
    if (argc != 6)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
    // 创建线程名字
    catch_error_libDB(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    //nEnable
    int nEnable;
    catch_error_libDB(env, napi_get_value_int32(env, argv[0], &nEnable));
    //nWeekday
    int nWeekday;
    catch_error_libDB(env, napi_get_value_int32(env, argv[1], &nWeekday));
    //nHour
    int nHour;
    catch_error_libDB(env, napi_get_value_int32(env, argv[2], &nHour));
    //nMinute
    int nMinute;
    catch_error_libDB(env, napi_get_value_int32(env, argv[3], &nMinute));
    //nUpdateBetaFw
    int nUpdateBetaFw;
    catch_error_libDB(env, napi_get_value_int32(env, argv[4], &nUpdateBetaFw));
    // 分配内存空间，在work_complete中会释放
    AddonData_set_fota_setting* addon_data = (AddonData_set_fota_setting*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    addon_data->nEnable = nEnable;
    addon_data->nWeekday = nWeekday;
    addon_data->nHour = nHour;
    addon_data->nMinute = nMinute;
    addon_data->nUpdateBetaFw = nUpdateBetaFw;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_libDB(env, napi_create_threadsafe_function(
        env,
        argv[5],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_set_fota_setting,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_libDB(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_set_fota_setting,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_set_fota_setting,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_libDB(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}

/***************************set_fota_update_status**************************/
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
    int nStatus;
} AddonData_set_fota_update_status;


void work_complete_set_fota_update_status(napi_env env, napi_status status, void* data) {
    AddonData_set_fota_update_status* addon_data = (AddonData_set_fota_update_status*)data;

    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_libDB(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    //free(addon_data->libPath);
    free(addon_data);
}

void call_js_callback_set_fota_update_status(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_libDB(env, napi_create_int32(
            env,
            data,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_libDB(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_libDB(env, napi_call_function(
            env,
            undefined,   // js回调的this对象
            js_cb,       // js回调函数句柄
            1,           // js回调函数接受参数个数
            &callBackInParams,        // js回调函数参数数组
            NULL         // js回调函数中如果有return，将会被result接受到
        ));
    }
}

 napi_value set_fota_update_status_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 2;
    napi_value argv[2];
    catch_error_libDB(env, napi_get_cb_info(
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
    //nStatus
    int nStatus;
    catch_error_libDB(env, napi_get_value_int32(env, argv[0], &nStatus));
    //执行so函数
    int result = setFOTAUpdateStatus(nStatus);
    //转类型
    napi_value world;
    catch_error_libDB(env, napi_create_int32(env, result, &world));
    //回调函数
    napi_value* callbackParams = &world;
    size_t callbackArgc = 1;
    napi_value global;
    napi_get_global(env, &global);
    napi_value callbackRs;
    napi_call_function(env, global, argv[1], callbackArgc, callbackParams, &callbackRs);
    return world;
}

/**
 * 执行线程
*/
 void execute_work_set_fota_update_status(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_set_fota_update_status* addon_data = (AddonData_set_fota_update_status*)data;
    //获取js-callback函数
    catch_error_libDB(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    int result = setFOTAUpdateStatus(addon_data->nStatus);
    //char* rs = (char*)malloc(sizeof(strlen(result)));
    //strcpy(rs, result);
    // 调用js-callback函数
    catch_error_libDB(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        result,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value set_fota_update_status(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 2;
    napi_value argv[2];
    catch_error_libDB(env, napi_get_cb_info(
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
    //nStatus
    int nStatus;
    catch_error_libDB(env, napi_get_value_int32(env, argv[0], &nStatus));
    // 创建线程名字
    catch_error_libDB(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 分配内存空间，在work_complete中会释放
    AddonData_set_fota_update_status* addon_data = (AddonData_set_fota_update_status*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    addon_data->nStatus = nStatus;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_libDB(env, napi_create_threadsafe_function(
        env,
        argv[1],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_set_fota_update_status,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_libDB(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_set_fota_update_status,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_set_fota_update_status,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_libDB(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}

/***************************set_firmware_upgrade_status**************************/
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
    int nStatus;
} AddonData_set_firmware_upgrade_status;


void work_complete_set_firmware_upgrade_status(napi_env env, napi_status status, void* data) {
    AddonData_set_firmware_upgrade_status* addon_data = (AddonData_set_firmware_upgrade_status*)data;

    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_libDB(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    //free(addon_data->libPath);
    free(addon_data);
}

void call_js_callback_set_firmware_upgrade_status(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_libDB(env, napi_create_int32(
            env,
            data,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_libDB(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_libDB(env, napi_call_function(
            env,
            undefined,   // js回调的this对象
            js_cb,       // js回调函数句柄
            1,           // js回调函数接受参数个数
            &callBackInParams,        // js回调函数参数数组
            NULL         // js回调函数中如果有return，将会被result接受到
        ));
    }
}

 napi_value set_firmware_upgrade_status_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 2;
    napi_value argv[2];
    catch_error_libDB(env, napi_get_cb_info(
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
    //nStatus
    int nStatus;
    catch_error_libDB(env, napi_get_value_int32(env, argv[0], &nStatus));
    //执行so函数
    int result = setFirmwareUpgradeStatus(nStatus);
    //转类型
    napi_value world;
    catch_error_libDB(env, napi_create_int32(env, result, &world));
    //回调函数
    napi_value* callbackParams = &world;
    size_t callbackArgc = 1;
    napi_value global;
    napi_get_global(env, &global);
    napi_value callbackRs;
    napi_call_function(env, global, argv[1], callbackArgc, callbackParams, &callbackRs);
    return world;
}

/**
 * 执行线程
*/
 void execute_work_set_firmware_upgrade_status(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_set_firmware_upgrade_status* addon_data = (AddonData_set_firmware_upgrade_status*)data;
    //获取js-callback函数
    catch_error_libDB(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    int result = setFirmwareUpgradeStatus(addon_data->nStatus);
    //char* rs = (char*)malloc(sizeof(strlen(result)));
    //strcpy(rs, result);
    // 调用js-callback函数
    catch_error_libDB(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        result,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value set_firmware_upgrade_status(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 2;
    napi_value argv[2];
    catch_error_libDB(env, napi_get_cb_info(
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
    // 创建线程名字
    catch_error_libDB(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    //nStatus
    int nStatus;
    catch_error_libDB(env, napi_get_value_int32(env, argv[0], &nStatus));
    // 分配内存空间，在work_complete中会释放
    AddonData_set_firmware_upgrade_status* addon_data = (AddonData_set_firmware_upgrade_status*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    addon_data->nStatus = nStatus;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_libDB(env, napi_create_threadsafe_function(
        env,
        argv[1],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_set_firmware_upgrade_status,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_libDB(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_set_firmware_upgrade_status,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_set_firmware_upgrade_status,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_libDB(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}

/***************************analyze_firmware_info**************************/
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
    int nMajor;
    int nMinor;
    int nRev;
    char* strFirmwareInfoFromFOTA;
    int nUpdateBetaFw;
} AddonData_analyze_firmware_info;


void work_complete_analyze_firmware_info(napi_env env, napi_status status, void* data) {
    AddonData_analyze_firmware_info* addon_data = (AddonData_analyze_firmware_info*)data;

    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //回收任务
    catch_error_libDB(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux上在这里释放地址会存在问题（在回调函数中将没有办法获取到这个值），但是Window上没有问题
    free(addon_data->strFirmwareInfoFromFOTA);
    free(addon_data);
}

void call_js_callback_analyze_firmware_info(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_libDB(env, napi_create_string_utf8(
            env,
            (const char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //获取global
        napi_value undefined;
        catch_error_libDB(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_libDB(env, napi_call_function(
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

 napi_value analyze_firmware_info_sync(napi_env env, napi_callback_info info) {
    //获取参数
    size_t argc = 6;
    napi_value argv[6];
    catch_error_libDB(env, napi_get_cb_info(
        env,
        info,
        &argc,
        argv,
        NULL,
        NULL
    ));
    if (argc != 6)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
    // 第一个参数：动态链接库的全路径(int nMajor, int nMinor, int nRev, char* strFirmwareInfoFromFOTA, int nUpdateBetaFw)
    size_t strLength;
    //nMajor
    int nMajor;
    catch_error_libDB(env, napi_get_value_int32(env, argv[0], &nMajor));
    //nMinor
    int nMinor;
    catch_error_libDB(env, napi_get_value_int32(env, argv[1], &nMinor));
    //nRev
    int nRev;
    catch_error_libDB(env, napi_get_value_int32(env, argv[2], &nRev));
    //strFirmwareInfoFromFOTA
    catch_error_libDB(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    char strFirmwareInfoFromFOTA[++strLength];
    catch_error_libDB(env, napi_get_value_string_utf8(env, argv[3], strFirmwareInfoFromFOTA, strLength, &strLength));
    //nUpdateBetaFw
    int nUpdateBetaFw;
    catch_error_libDB(env, napi_get_value_int32(env, argv[4], &nUpdateBetaFw));
    //执行so函数
    char* info = analyzeFirmwareInfo(nMajor, nMinor, nRev, strFirmwareInfoFromFOTA, nUpdateBetaFw);
    //转类型
    napi_value world;
    catch_error_libDB(env, napi_create_string_utf8(env, info, NAPI_AUTO_LENGTH, &world));
    //回调函数
    napi_value* callbackParams = &world;
    size_t callbackArgc = 1;
    napi_value global;
    napi_get_global(env, &global);
    napi_value callbackRs;
    napi_call_function(env, global, argv[5], callbackArgc, callbackParams, &callbackRs);
    return world;
}

/**
 * 执行线程
*/
 void execute_work_analyze_firmware_info(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_analyze_firmware_info* addon_data = (AddonData_analyze_firmware_info*)data;
    //获取js-callback函数
    catch_error_libDB(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //执行so函数
    char* info = analyzeFirmwareInfo(addon_data->nMajor, addon_data->nMinor, addon_data->nRev, addon_data->strFirmwareInfoFromFOTA, addon_data->nUpdateBetaFw);
    char* v = (char*)malloc(sizeof(char) * (strlen(info) + 1));
    strcpy(v, info);
    // 调用js-callback函数
    catch_error_libDB(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        v,                    // call_js_callback的第四个参数
        //"Async Callback Result",
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_libDB(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_release指示当前线程不再调用线程安全函数，napi_tsfn_abort指示出当前线程外，其它线程不应再调用线程安全函数
    ));

}

 napi_value analyze_firmware_info(napi_env env, napi_callback_info info) {
    // 给线程起的名字
    napi_value work_name;
    //获取参数
    size_t argc = 6;
    napi_value argv[6];
    catch_error_libDB(env, napi_get_cb_info(
        env,
        info,
        &argc,
        argv,
        NULL,
        NULL
    ));
    if (argc != 6)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch");
    }
    // 创建线程名字
    catch_error_libDB(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // 第一个参数：动态链接库的全路径(int nMajor, int nMinor, int nRev, char* strFirmwareInfoFromFOTA, int nUpdateBetaFw)
    size_t strLength;
    //nMajor
    int nMajor;
    catch_error_libDB(env, napi_get_value_int32(env, argv[0], &nMajor));
    //nMinor
    int nMinor;
    catch_error_libDB(env, napi_get_value_int32(env, argv[1], &nMinor));
    //nRev
    int nRev;
    catch_error_libDB(env, napi_get_value_int32(env, argv[2], &nRev));
    //strFirmwareInfoFromFOTA
    catch_error_libDB(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    char strFirmwareInfoFromFOTA[++strLength];
    catch_error_libDB(env, napi_get_value_string_utf8(env, argv[3], strFirmwareInfoFromFOTA, strLength, &strLength));
    //nUpdateBetaFw
    int nUpdateBetaFw;
    catch_error_libDB(env, napi_get_value_int32(env, argv[4], &nUpdateBetaFw));
    // 分配内存空间，在work_complete中会释放
    AddonData_analyze_firmware_info* addon_data = (AddonData_analyze_firmware_info*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    addon_data->work = NULL;
    addon_data->nMajor = nMajor;
    addon_data->nMinor = nMinor;
    addon_data->nRev = nRev;
    addon_data->strFirmwareInfoFromFOTA = (char*)malloc(sizeof(char) * (strlen(strFirmwareInfoFromFOTA) + 1));
    strcpy(addon_data->strFirmwareInfoFromFOTA, strFirmwareInfoFromFOTA);
    addon_data->strFirmwareInfoFromFOTA = NULL;
    addon_data->nUpdateBetaFw = nUpdateBetaFw;
    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_libDB(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_analyze_firmware_info,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));
    // 负责执行上面创建的函数
   // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_libDB(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_analyze_firmware_info,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_analyze_firmware_info,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_libDB(env, napi_queue_async_work(
        env,
        addon_data->work   //要执行线程的句柄
    ));
    return NULL;
}
/**************************************init**************************************/
napi_value init(napi_env env, napi_value exports)
{
    /***************************getHWVersion**************************/
    // 获取性能数据
    napi_property_descriptor getHWVersionSync = {
        "getHWVersionSync",
        NULL,
        get_hw_version_sync,
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
        &getHWVersionSync
    );

    // 获取性能数据
    napi_property_descriptor getHWVersion = {
        "getHWVersion",
        NULL,
        get_hw_version,
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
        &getHWVersion
    );
    /***************************getNCVersion**************************/
    // 获取性能数据
    napi_property_descriptor getNCVersionSync = {
        "getNCVersionSync",
        NULL,
        get_nc_version_sync,
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
        &getNCVersionSync
    );

    // 获取性能数据
    napi_property_descriptor getNCVersion = {
        "getNCVersion",
        NULL,
        get_nc_version,
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
        &getNCVersion
    );
    /***************************getFOTASetting**************************/
    // 获取性能数据
    napi_property_descriptor getFOTASettingSync = {
        "getFOTASettingSync",
        NULL,
        get_fota_setting_sync,
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
        &getFOTASettingSync
    );

    // 获取性能数据
    napi_property_descriptor getFOTASetting = {
        "getFOTASetting",
        NULL,
        get_fota_setting,
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
        &getFOTASetting
    );
    /***************************getFirmwareVersion**************************/
    // 获取性能数据
    napi_property_descriptor getFirmwareVersionSync = {
        "getFirmwareVersionSync",
        NULL,
        get_firmware_version_sync,
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
        &getFirmwareVersionSync
    );

    // 获取性能数据
    napi_property_descriptor getFirmwareVersion = {
        "getFirmwareVersion",
        NULL,
        get_firmware_version,
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
        &getFirmwareVersion
    );
    /***************************getFirmwareVersionFull**************************/
    // 获取性能数据
    napi_property_descriptor getFirmwareVersionFullSync = {
        "getFirmwareVersionFullSync",
        NULL,
        get_firmware_version_full_sync,
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
        &getFirmwareVersionFullSync
    );

    // 获取性能数据
    napi_property_descriptor getFirmwareVersionFull = {
        "getFirmwareVersionFull",
        NULL,
        get_firmware_version_full,
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
        &getFirmwareVersionFull
    );
    /***************************getFirmwareInfo**************************/
// 获取性能数据
    napi_property_descriptor getFirmwareInfoSync = {
        "getFirmwareInfoSync",
        NULL,
        get_firmware_info_sync,
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
        &getFirmwareInfoSync
    );

    // 获取性能数据
    napi_property_descriptor getFirmwareInfo = {
        "getFirmwareInfo",
        NULL,
        get_firmware_info,
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
        &getFirmwareInfo
    );
    /***************************getDeviceUUID**************************/
// 获取性能数据
    napi_property_descriptor getDeviceUUIDSync = {
        "getDeviceUUIDSync",
        NULL,
        get_device_uuid_sync,
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
        &getDeviceUUIDSync
    );

    // 获取性能数据
    napi_property_descriptor getDeviceUUID = {
        "getDeviceUUID",
        NULL,
        get_device_uuid,
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
        &getDeviceUUID
    );
    /***************************getFirmwareUpgradeStatus**************************/
// 获取性能数据
    napi_property_descriptor getFirmwareUpgradeStatusSync = {
        "getFirmwareUpgradeStatusSync",
        NULL,
        get_firmware_upgrade_status_sync,
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
        &getFirmwareUpgradeStatusSync
    );

    // 获取性能数据
    napi_property_descriptor getFirmwareUpgradeStatus = {
        "getFirmwareUpgradeStatus",
        NULL,
        get_firmware_upgrade_status,
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
        &getFirmwareUpgradeStatus
    );
    /***************************getFOTAUpdateStatus**************************/
    // 获取性能数据
    napi_property_descriptor getFOTAUpdateStatusSync = {
        "getFOTAUpdateStatusSync",
        NULL,
        get_fota_update_status_sync,
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
        &getFOTAUpdateStatusSync
    );

    // 获取性能数据
    napi_property_descriptor getFOTAUpdateStatus = {
        "getFOTAUpdateStatus",
        NULL,
        get_fota_update_status,
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
        &getFOTAUpdateStatus
    );
    /***************************setFOTASetting**************************/
    // 获取性能数据
    napi_property_descriptor setFOTASettingSync = {
        "setFOTASettingSync",
        NULL,
        set_fota_setting_sync,
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
        &setFOTASettingSync
    );

    // 获取性能数据
    napi_property_descriptor setFOTASetting = {
        "setFOTASetting",
        NULL,
        set_fota_setting,
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
        &setFOTASetting
    );
    /***************************setFOTAUpdateStatus**************************/
// 获取性能数据
    napi_property_descriptor setFOTAUpdateStatusSync = {
        "setFOTAUpdateStatusSync",
        NULL,
        set_fota_update_status_sync,
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
        &setFOTAUpdateStatusSync
    );

    // 获取性能数据
    napi_property_descriptor setFOTAUpdateStatus = {
        "setFOTAUpdateStatus",
        NULL,
        set_fota_update_status,
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
        &setFOTAUpdateStatus
    );
    /***************************setFirmwareUpgradeStatus**************************/
// 获取性能数据
    napi_property_descriptor setFirmwareUpgradeStatusSync = {
        "setFirmwareUpgradeStatusSync",
        NULL,
        set_firmware_upgrade_status_sync,
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
        &setFirmwareUpgradeStatusSync
    );

    // 获取性能数据
    napi_property_descriptor setFirmwareUpgradeStatus = {
        "setFirmwareUpgradeStatus",
        NULL,
        set_firmware_upgrade_status,
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
        &setFirmwareUpgradeStatus
    );
    /***************************analyzeFirmwareInfo**************************/
// 获取性能数据
    napi_property_descriptor analyzeFirmwareInfoSync = {
        "analyzeFirmwareInfoSync",
        NULL,
        analyze_firmware_info_sync,
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
        &analyzeFirmwareInfoSync
    );

    // 获取性能数据
    napi_property_descriptor analyzeFirmwareInfo = {
        "analyzeFirmwareInfo",
        NULL,
        analyze_firmware_info,
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
        &analyzeFirmwareInfo
    );
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init);