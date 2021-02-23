// libCLI
#include <node_api.h>
#include <string.h>
#include <stdlib.h>
#include "libCLI.h"

/**
 * 捕获异常
 */
void catch_error_cli(napi_env env, napi_status status) {
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
 * 参数个数检查, 并判断是否包含回调函数
 */
bool checkParams_cli(napi_env env, napi_callback_info info, size_t argc, napi_value* argv, bool isAsync) {
    bool rs = false;
    napi_valuetype valueTypeLast; // 最后一个参数的类型
    size_t temp = argc;// 保存应有的参数个数
    // 获取实际的参数个数和参数列表
    catch_error_cli(env, napi_get_cb_info(env, info, &argc, argv, NULL, NULL));
    if (isAsync) {
        // 异步调用，需包含回调函数
        if (argc != temp)
        {            
            napi_throw_error(env, "EINVAL", "Async call argument count mismatch.");
            return rs;
        }
        napi_typeof(env, argv[argc - 1], &valueTypeLast);
        if (valueTypeLast != napi_function) {
            napi_throw_type_error(env, NULL, "Async call has not callback function.");
            return rs;
        }
        rs = true;
    }
    else {
        // 同步调用
        if (argc != temp && argc != temp - 1)
        {
            napi_throw_error(env, "EINVAL", "Sync call argument count mismatch");
        }
        if (argc == temp) {
            napi_typeof(env, argv[argc - 1], &valueTypeLast);
            if (valueTypeLast == napi_function) {
                rs = true;
            }
        }    
    }
    return rs;
}

/**
 * 返回值为int的回调处理
 */
void call_js_callback_cli(napi_env env, napi_value js_cb, void* context, void* data)
{
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_cli(env, napi_create_int32(env, (int32_t)data, &callBackInParams));

        //获取global
        napi_value undefined;
        catch_error_cli(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_cli(env, napi_call_function(
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
void call_js_callback_str_cli(napi_env env, napi_value js_cb, void* context, void* data)
{
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_cli(env, napi_create_string_utf8(env, (const char*)data, NAPI_AUTO_LENGTH, &callBackInParams));

        //获取global
        napi_value undefined;
        catch_error_cli(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_cli(env, napi_call_function(
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

//---------char* getSystemInfo();-------------
napi_value get_system_info_sync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    bool has_callback = checkParams_cli(env, info, argc, argv, false);// 检查参数个数
    // 执行so函数
    char* status = getSystemInfo();
    // 转类型
    napi_value world;
    catch_error_cli(env, napi_create_string_utf8(env, status, NAPI_AUTO_LENGTH, &world));
    if (has_callback) {
        // 回调函数
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[argc - 1], callbackArgc, callbackParams, &callbackRs);
    }
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
} AddonData_getSystemInfo;

/**
 * 执行线程
*/
void execute_work_getSystemInfo(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_getSystemInfo* addon_data = (AddonData_getSystemInfo*)data;
    //获取js-callback函数
    catch_error_cli(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    char* status = getSystemInfo();
    char* cbInParam = (char*)malloc(sizeof(char) * (strlen(status) + 1));
    strcpy(cbInParam, status);
    // 调用js-callback函数
    catch_error_cli(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        cbInParam,                    // 回调函数的返回值
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}


/**
 * 执行线程完成
*/
void work_complete_getSystemInfo(napi_env env, napi_status status, void* data)
{
    AddonData_getSystemInfo* addon_data = (AddonData_getSystemInfo*)data;
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_cli(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data);
}

napi_value get_system_info(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    // 检查参数个数
    if (!checkParams_cli(env, info, argc, argv, true)) {
        return NULL;
    }
    // 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_getSystemInfo* addon_data = NULL;// 结构体    
    addon_data = (AddonData_getSystemInfo*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 回调函数
    js_cb = argv[0];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_cli(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe getSystemInfo Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_cli(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_str_cli,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_cli(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_getSystemInfo,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_getSystemInfo,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_cli(env, napi_queue_async_work(env, addon_data->work));
    return NULL;
}

//-------------int setAdminPassword(char* password);
napi_value set_admin_password_sync(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    bool has_callback = checkParams_cli(env, info, argc, argv, false);// 检查参数个数    
    // 获取参数
    size_t strLength;
    // password    
    catch_error_cli(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* password = (char*)malloc((++strLength) * sizeof(char));
    catch_error_cli(env, napi_get_value_string_utf8(env, argv[0], password, strLength, &strLength));
    // 执行so函数
    int status = setAdminPassword(password);
    free(password);
    // 转类型
    napi_value world;
    catch_error_cli(env, napi_create_int32(env, status, &world));
    if (has_callback) {
        // 回调函数
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[1], callbackArgc, callbackParams, &callbackRs);
    }
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
    char* password;
} AddonData_setAdminPassword;

/**
 * 执行线程
*/
void execute_work_setAdminPassword(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_setAdminPassword* addon_data = (AddonData_setAdminPassword*)data;
    //获取js-callback函数
    catch_error_cli(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    int status = setAdminPassword(addon_data->password);
    // 调用js-callback函数
    catch_error_cli(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        status,                    // 回调函数接收的返回值
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}

/**
 * 执行线程完成
*/
void work_complete_setAdminPassword(napi_env env, napi_status status, void* data)
{
    AddonData_setAdminPassword* addon_data = (AddonData_setAdminPassword*)data;
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_cli(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data->password);
    free(addon_data);
}

napi_value set_admin_password(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    size_t strLength;
    // 检查参数个数
    if (!checkParams_cli(env, info, argc, argv, true)) {
        return NULL;
    }
    // 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_setAdminPassword* addon_data = NULL;// 结构体    
    addon_data = (AddonData_setAdminPassword*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // password    
    catch_error_cli(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    addon_data->password = (char*)malloc((++strLength) * sizeof(char));
    catch_error_cli(env, napi_get_value_string_utf8(env, argv[0], addon_data->password, strLength, &strLength));
    // 回调函数
    js_cb = argv[1];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_cli(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe setAdminPassword Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_cli(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_cli,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_cli(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_setAdminPassword,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_setAdminPassword,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_cli(env, napi_queue_async_work(env, addon_data->work));
    return NULL;
}

//-------------int save();
napi_value cli_save_sync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    bool has_callback = checkParams_cli(env, info, argc, argv, false);// 检查参数个数
    // 执行so函数
    int status = save();
    // 转类型
    napi_value world;
    catch_error_cli(env, napi_create_int32(env, status, &world));
    if (has_callback) {
        // 回调函数
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[0], callbackArgc, callbackParams, &callbackRs);
    }
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
} AddonData_cli_save;

/**
 * 执行线程
*/
void execute_work_cli_save(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_cli_save* addon_data = (AddonData_cli_save*)data;
    //获取js-callback函数
    catch_error_cli(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    int status = save();
    // 调用js-callback函数
    catch_error_cli(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        status,                    // call_js_callback_cli的第四个参数
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}


/**
 * 执行线程完成
*/
void work_complete_cli_save(napi_env env, napi_status status, void* data)
{
    AddonData_cli_save* addon_data = (AddonData_cli_save*)data;
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_cli(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data);
}

napi_value cli_save(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    // 检查参数个数
    if (!checkParams_cli(env, info, argc, argv, true)) {
        return NULL;
    }   
    // 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_cli_save* addon_data = NULL;// 结构体    
    addon_data = (AddonData_cli_save*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 回调函数
    js_cb = argv[0];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_cli(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe cli_save Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_cli(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_cli,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_cli(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_cli_save,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_cli_save,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_cli(env, napi_queue_async_work(env, addon_data->work));
    return NULL;
}

//------------char* getDeviceName();
napi_value get_device_name_sync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    bool has_callback = checkParams_cli(env, info, argc, argv, false);// 检查参数个数
    // 执行so函数
    char* status = getDeviceName();
    // 转类型
    napi_value world;
    catch_error_cli(env, napi_create_string_utf8(env, status, NAPI_AUTO_LENGTH, &world));
    if (has_callback) {
        // 回调函数
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[0], callbackArgc, callbackParams, &callbackRs);
    }
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
} AddonData_getDeviceName;

/**
 * 执行线程
*/
void execute_work_getDeviceName(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_getDeviceName* addon_data = (AddonData_getDeviceName*)data;
    //获取js-callback函数
    catch_error_cli(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    char* status = getDeviceName();
    char* cbInParam = (char*)malloc(sizeof(char) * (strlen(status) + 1));
    strcpy(cbInParam, status);
    // 调用js-callback函数
    catch_error_cli(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        cbInParam,                    // 回调函数的返回值
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}


/**
 * 执行线程完成
*/
void work_complete_getDeviceName(napi_env env, napi_status status, void* data)
{
    AddonData_getDeviceName* addon_data = (AddonData_getDeviceName*)data;
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_cli(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data);
}

napi_value get_device_name(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    // 检查参数个数
    if (!checkParams_cli(env, info, argc, argv, true)) {
        return NULL;
    }    
    // 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_getDeviceName* addon_data = NULL;// 结构体    
    addon_data = (AddonData_getDeviceName*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 回调函数
    js_cb = argv[0];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_cli(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe getDeviceName Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_cli(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_str_cli,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_cli(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_getDeviceName,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_getDeviceName,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_cli(env, napi_queue_async_work(env, addon_data->work));
    return NULL;
}

//------------int setDeviceName(char* strDeviceName);
napi_value set_device_name_sync(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    bool has_callback = checkParams_cli(env, info, argc, argv, false);// 检查参数个数    
    // 获取参数
    size_t strLength;
    // strDeviceName    
    catch_error_cli(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* strDeviceName = (char*)malloc((++strLength) * sizeof(char));
    catch_error_cli(env, napi_get_value_string_utf8(env, argv[0], strDeviceName, strLength, &strLength));
    // 执行so函数
    int status = setDeviceName(strDeviceName);
    free(strDeviceName);
    // 转类型
    napi_value world;
    catch_error_cli(env, napi_create_int32(env, status, &world));
    if (has_callback) {
        // 回调函数
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[1], callbackArgc, callbackParams, &callbackRs);
    }
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
    char* strDeviceName;
} AddonData_setDeviceName;

/**
 * 执行线程
*/
void execute_work_setDeviceName(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_setDeviceName* addon_data = (AddonData_setDeviceName*)data;
    //获取js-callback函数
    catch_error_cli(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    int status = setDeviceName(addon_data->strDeviceName);
    // 调用js-callback函数
    catch_error_cli(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        status,                    // 回调函数接收的返回值
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}

/**
 * 执行线程完成
*/
void work_complete_setDeviceName(napi_env env, napi_status status, void* data)
{
    AddonData_setDeviceName* addon_data = (AddonData_setDeviceName*)data;
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_cli(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data->strDeviceName);
    free(addon_data);
}

napi_value set_device_name(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    size_t strLength;
    // 检查参数个数
    if (!checkParams_cli(env, info, argc, argv, true)) {
        return NULL;
    }   
    // 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_setDeviceName* addon_data = NULL;// 结构体    
    addon_data = (AddonData_setDeviceName*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // strDeviceName    
    catch_error_cli(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    addon_data->strDeviceName = (char*)malloc((++strLength) * sizeof(char));
    catch_error_cli(env, napi_get_value_string_utf8(env, argv[0], addon_data->strDeviceName, strLength, &strLength));
    // 回调函数
    js_cb = argv[1];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_cli(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe setDeviceName Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_cli(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_cli,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_cli(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_setDeviceName,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_setDeviceName,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_cli(env, napi_queue_async_work(env, addon_data->work));
    return NULL;
}

//------------char* getPerformance();
napi_value get_performance_sync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    bool has_callback = checkParams_cli(env, info, argc, argv, false);// 检查参数个数
    // 执行so函数
    char* status = getPerformance();
    // 转类型
    napi_value world;
    catch_error_cli(env, napi_create_string_utf8(env, status, NAPI_AUTO_LENGTH, &world));
    if (has_callback) {
        // 回调函数
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[0], callbackArgc, callbackParams, &callbackRs);
    }
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
} AddonData_getPerformance;

/**
 * 执行线程
*/
void execute_work_getPerformance(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_getPerformance* addon_data = (AddonData_getPerformance*)data;
    //获取js-callback函数
    catch_error_cli(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    char* status = getPerformance();
    char* cbInParam = (char*)malloc(sizeof(char) * (strlen(status) + 1));
    strcpy(cbInParam, status);
    // 调用js-callback函数
    catch_error_cli(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        cbInParam,                    // 回调函数的返回值
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}


/**
 * 执行线程完成
*/
void work_complete_getPerformance(napi_env env, napi_status status, void* data)
{
    AddonData_getPerformance* addon_data = (AddonData_getPerformance*)data;
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_cli(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data);
}

napi_value get_performance(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    // 检查参数个数
    if (!checkParams_cli(env, info, argc, argv, true)) {
        return NULL;
    }

	// 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_getPerformance* addon_data = NULL;// 结构体    
    addon_data = (AddonData_getPerformance*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 回调函数
    js_cb = argv[0];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_cli(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe getPerformance Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_cli(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_str_cli,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_cli(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_getPerformance,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_getPerformance,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_cli(env, napi_queue_async_work(env, addon_data->work));
    return NULL;
}

//------------int getWebAccessPort(void);
napi_value get_web_access_port_sync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    bool has_callback = checkParams_cli(env, info, argc, argv, false);// 检查参数个数
    // 执行so函数
    int status = getWebAccessPort();
    // 转类型
    napi_value world;
    catch_error_cli(env, napi_create_int32(env, status, &world));
    if (has_callback) {
        // 回调函数
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[0], callbackArgc, callbackParams, &callbackRs);
    }
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
} AddonData_getWebAccessPort;

/**
 * 执行线程
*/
void execute_work_getWebAccessPort(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_getWebAccessPort* addon_data = (AddonData_getWebAccessPort*)data;
    //获取js-callback函数
    catch_error_cli(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    int status = getWebAccessPort();
    // 调用js-callback函数
    catch_error_cli(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        status,                    // 回调函数接收的返回值
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}

/**
 * 执行线程完成
*/
void work_complete_getWebAccessPort(napi_env env, napi_status status, void* data)
{
    AddonData_getWebAccessPort* addon_data = (AddonData_getWebAccessPort*)data;
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_cli(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data);
}

napi_value get_web_access_port(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    // 检查参数个数
    if (!checkParams_cli(env, info, argc, argv, true)) {
        return NULL;
    }    
    // 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_getWebAccessPort* addon_data = NULL;// 结构体    
    addon_data = (AddonData_getWebAccessPort*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 回调函数
    js_cb = argv[0];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_cli(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe getWebAccessPort Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_cli(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_cli,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_cli(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_getWebAccessPort,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_getWebAccessPort,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_cli(env, napi_queue_async_work(env, addon_data->work));
    return NULL;
}

//------------int setWebAccessPort(int nPortNumber);
napi_value set_web_access_port_sync(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    bool has_callback = checkParams_cli(env, info, argc, argv, false);// 检查参数个数    
    // 获取参数
    // nPortNumber    
    int nPortNumber;
    catch_error_cli(env, napi_get_value_int32(env, argv[0], &nPortNumber));
    // 执行so函数
    int status = setWebAccessPort(nPortNumber);
    // 转类型
    napi_value world;
    catch_error_cli(env, napi_create_int32(env, status, &world));
    if (has_callback) {
        // 回调函数
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[1], callbackArgc, callbackParams, &callbackRs);
    }
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
    int nPortNumber;
} AddonData_setWebAccessPort;

/**
 * 执行线程
*/
void execute_work_setWebAccessPort(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_setWebAccessPort* addon_data = (AddonData_setWebAccessPort*)data;
    //获取js-callback函数
    catch_error_cli(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    int status = setWebAccessPort(addon_data->nPortNumber);
    // 调用js-callback函数
    catch_error_cli(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        status,                    // 回调函数接收的返回值
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}

/**
 * 执行线程完成
*/
void work_complete_setWebAccessPort(napi_env env, napi_status status, void* data)
{
    AddonData_setWebAccessPort* addon_data = (AddonData_setWebAccessPort*)data;
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_cli(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data);
}

napi_value set_web_access_port(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    size_t strLength;
    // 检查参数个数
    if (!checkParams_cli(env, info, argc, argv, true)) {
        return NULL;
    }    
    // 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_setWebAccessPort* addon_data = NULL;// 结构体    
    addon_data = (AddonData_setWebAccessPort*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // nPortNumber    
    catch_error_cli(env, napi_get_value_int32(env, argv[0], &addon_data->nPortNumber));
    // 回调函数
    js_cb = argv[1];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_cli(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe setWebAccessPort Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_cli(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_cli,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_cli(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_setWebAccessPort,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_setWebAccessPort,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_cli(env, napi_queue_async_work(env, addon_data->work));
    return NULL;
}

//------------int getNginxAccessPort();
napi_value get_nginx_access_port_sync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    bool has_callback = checkParams_cli(env, info, argc, argv, false);// 检查参数个数
    // 执行so函数
    int status = getNginxAccessPort();
    // 转类型
    napi_value world;
    catch_error_cli(env, napi_create_int32(env, status, &world));
    if (has_callback) {
        // 回调函数
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[0], callbackArgc, callbackParams, &callbackRs);
    }
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
} AddonData_getNginxAccessPort;

/**
 * 执行线程
*/
void execute_work_getNginxAccessPort(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_getNginxAccessPort* addon_data = (AddonData_getNginxAccessPort*)data;
    //获取js-callback函数
    catch_error_cli(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    int status = getNginxAccessPort();
    // 调用js-callback函数
    catch_error_cli(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        status,                    // 回调函数接收的返回值
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}

/**
 * 执行线程完成
*/
void work_complete_getNginxAccessPort(napi_env env, napi_status status, void* data)
{
    AddonData_getNginxAccessPort* addon_data = (AddonData_getNginxAccessPort*)data;
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_cli(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data);
}

napi_value get_nginx_access_port(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    // 检查参数个数
    if (!checkParams_cli(env, info, argc, argv, true)) {
        return NULL;
    }    
    // 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_getNginxAccessPort* addon_data = NULL;// 结构体    
    addon_data = (AddonData_getNginxAccessPort*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 回调函数
    js_cb = argv[0];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_cli(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe getNginxAccessPort Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_cli(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_cli,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_cli(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_getNginxAccessPort,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_getNginxAccessPort,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_cli(env, napi_queue_async_work(env, addon_data->work));
    return NULL;
}

//------------int setNginxAccessPort(int nPortNumber);
napi_value set_nginx_access_port_sync(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    bool has_callback = checkParams_cli(env, info, argc, argv, false);// 检查参数个数    
    // 获取参数
    // nPortNumber    
    int nPortNumber;
    catch_error_cli(env, napi_get_value_int32(env, argv[0], &nPortNumber));
    
    // 执行so函数
    int status = setNginxAccessPort(nPortNumber);
    // 转类型
    napi_value world;
    catch_error_cli(env, napi_create_int32(env, status, &world));
    if (has_callback) {
        // 回调函数
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[1], callbackArgc, callbackParams, &callbackRs);
    }
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
    int nPortNumber;
} AddonData_setNginxAccessPort;

/**
 * 执行线程
*/
void execute_work_setNginxAccessPort(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_setNginxAccessPort* addon_data = (AddonData_setNginxAccessPort*)data;
    //获取js-callback函数
    catch_error_cli(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    int status = setNginxAccessPort(addon_data->nPortNumber);
    // 调用js-callback函数
    catch_error_cli(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        status,                    // 回调函数接收的返回值
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}

/**
 * 执行线程完成
*/
void work_complete_setNginxAccessPort(napi_env env, napi_status status, void* data)
{
    AddonData_setNginxAccessPort* addon_data = (AddonData_setNginxAccessPort*)data;
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_cli(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data);
}

napi_value set_nginx_access_port(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    size_t strLength;
    // 检查参数个数
    if (!checkParams_cli(env, info, argc, argv, true)) {
        return NULL;
    }    
    // 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_setNginxAccessPort* addon_data = NULL;// 结构体    
    addon_data = (AddonData_setNginxAccessPort*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // nPortNumber    
    catch_error_cli(env, napi_get_value_int32(env, argv[0], &addon_data->nPortNumber));
    // 回调函数
    js_cb = argv[1];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_cli(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe setNginxAccessPort Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_cli(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_cli,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_cli(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_setNginxAccessPort,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_setNginxAccessPort,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_cli(env, napi_queue_async_work(env, addon_data->work));
    return NULL;
}

//-------------char* getNvrVersion();
napi_value get_nvr_version_sync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    bool has_callback = checkParams_cli(env, info, argc, argv, false);// 检查参数个数
    // 执行so函数
    char* status = getNvrVersion();
    // 转类型
    napi_value world;
    catch_error_cli(env, napi_create_string_utf8(env, status, NAPI_AUTO_LENGTH, &world));
    if (has_callback) {
        // 回调函数
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[0], callbackArgc, callbackParams, &callbackRs);
    }
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
} AddonData_getNvrVersion;

/**
 * 执行线程
*/
void execute_work_getNvrVersion(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_getNvrVersion* addon_data = (AddonData_getNvrVersion*)data;
    //获取js-callback函数
    catch_error_cli(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    char* status = getNvrVersion();
    char* cbInParam = (char*)malloc(sizeof(char) * (strlen(status) + 1));
    strcpy(cbInParam, status);
    // 调用js-callback函数
    catch_error_cli(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        cbInParam,                    // 回调函数的返回值
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}


/**
 * 执行线程完成
*/
void work_complete_getNvrVersion(napi_env env, napi_status status, void* data)
{
    AddonData_getNvrVersion* addon_data = (AddonData_getNvrVersion*)data;
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_cli(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data);
}

napi_value get_nvr_version(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    // 检查参数个数
    if (!checkParams_cli(env, info, argc, argv, true)) {
        return NULL;
    }    
    // 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_getNvrVersion* addon_data = NULL;// 结构体    
    addon_data = (AddonData_getNvrVersion*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // 回调函数
    js_cb = argv[0];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_cli(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe getNvrVersion Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_cli(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_str_cli,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_cli(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_getNvrVersion,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_getNvrVersion,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_cli(env, napi_queue_async_work(env, addon_data->work));
    return NULL;
}

//-------------int setMACAddress(char* macAddress);
napi_value set_MAC_address_sync(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    bool has_callback = checkParams_cli(env, info, argc, argv, false);// 检查参数个数    
    // 获取参数
    size_t strLength;
    // macAddress    
    catch_error_cli(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* macAddress = (char*)malloc((++strLength) * sizeof(char));
    catch_error_cli(env, napi_get_value_string_utf8(env, argv[0], macAddress, strLength, &strLength));
    // 执行so函数
    int status = setMACAddress(macAddress);
    free(macAddress);
    // 转类型
    napi_value world;
    catch_error_cli(env, napi_create_int32(env, status, &world));
    if (has_callback) {
        // 回调函数
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[1], callbackArgc, callbackParams, &callbackRs);
    }
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
    char* macAddress;
} AddonData_setMACAddress;

/**
 * 执行线程
*/
void execute_work_setMACAddress(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_setMACAddress* addon_data = (AddonData_setMACAddress*)data;
    //获取js-callback函数
    catch_error_cli(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    int status = setMACAddress(addon_data->macAddress);
    // 调用js-callback函数
    catch_error_cli(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        status,                    // 回调函数接收的返回值
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}

/**
 * 执行线程完成
*/
void work_complete_setMACAddress(napi_env env, napi_status status, void* data)
{
    AddonData_setMACAddress* addon_data = (AddonData_setMACAddress*)data;
    //释放句柄
    catch_error_cli(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_cli(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data->macAddress);
    free(addon_data);
}

napi_value set_MAC_address(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value argv[2];
    size_t strLength;
    // 检查参数个数
    if (!checkParams_cli(env, info, argc, argv, true)) {
        return NULL;
    }    
    // 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_setMACAddress* addon_data = NULL;// 结构体    
    addon_data = (AddonData_setMACAddress*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // macAddress    
    catch_error_cli(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    addon_data->macAddress = (char*)malloc((++strLength) * sizeof(char));
    catch_error_cli(env, napi_get_value_string_utf8(env, argv[0], addon_data->macAddress, strLength, &strLength));
    // 回调函数
    js_cb = argv[1];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_cli(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe setMACAddress Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_cli(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_cli,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_cli(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_setMACAddress,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_setMACAddress,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_cli(env, napi_queue_async_work(env, addon_data->work));
    return NULL;
}