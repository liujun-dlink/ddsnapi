// libFWValidate
#include <node_api.h>
#include <stdlib.h>
#include "libFWValidate.h"

/**
 * 捕获异常
 */
void catch_error_FWValidate(napi_env env, napi_status status) {
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
bool checkParams_FWValidate(napi_env env, napi_callback_info info, size_t argc, napi_value* argv, bool isAsync) {
    bool rs = false;
    napi_valuetype valueTypeLast; // 最后一个参数的类型
    size_t temp = argc;// 保存应有的参数个数
    // 获取实际的参数个数和参数列表
    catch_error_FWValidate(env, napi_get_cb_info(env, info, &argc, argv, NULL, NULL));
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
void call_js_callback_FWValidate(napi_env env, napi_value js_cb, void* context, void* data)
{
    if (env != NULL)
    {
        // 获取异步线程返回的结果
        napi_value callBackInParams;
        catch_error_FWValidate(env, napi_create_int32(env, (int32_t)data, &callBackInParams));

        //获取global
        napi_value undefined;
        catch_error_FWValidate(env, napi_get_undefined(env, &undefined));

        //调用函数
        catch_error_FWValidate(env, napi_call_function(
            env,
            undefined,   // js回调的this对象
            js_cb,       // js回调函数句柄
            1,           // js回调函数接受参数个数
            &callBackInParams,        // js回调函数参数数组
            NULL         // js回调函数中如果有return，将会被result接受到
        ));
    }
}

//-------int checkHeaderAndPayload(char* strFilePath, char* strFileName);--------
napi_value check_header_and_payload_sync(napi_env env, napi_callback_info info) {
    size_t argc = 3;
    napi_value argv[3];
    bool has_callback = checkParams_FWValidate(env, info, argc, argv, false);// 检查参数个数 
    size_t strLength;
    // 获取参数
    catch_error_FWValidate(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* strFilePath = (char*)malloc((++strLength) * sizeof(char));
    catch_error_FWValidate(env, napi_get_value_string_utf8(env, argv[0], strFilePath, strLength, &strLength));
    // strFileName
    catch_error_FWValidate(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    char* strFileName = (char*)malloc((++strLength) * sizeof(char));
    catch_error_FWValidate(env, napi_get_value_string_utf8(env, argv[1], strFileName, strLength, &strLength));
    // 执行so函数
    int status = checkHeaderAndPayload(strFilePath, strFileName);
    free(strFilePath);
    free(strFileName);
    // 转类型
    napi_value world;
    catch_error_FWValidate(env, napi_create_int32(env, status, &world));
    if (has_callback) {
        // 回调函数
        napi_value* callbackParams = &world;
        size_t callbackArgc = 1;
        napi_value global;
        napi_get_global(env, &global);
        napi_value callbackRs;
        napi_call_function(env, global, argv[2], callbackArgc, callbackParams, &callbackRs);
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
    char* strFilePath;
    char* strFileName;
} AddonData_checkHeaderAndPayload;

/**
 * 执行线程
*/
void execute_work_checkHeaderAndPayload(napi_env env, void* data)
{
    // 传入进来的参数
    AddonData_checkHeaderAndPayload* addon_data = (AddonData_checkHeaderAndPayload*)data;
    //获取js-callback函数
    catch_error_FWValidate(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    // 执行so函数
    int status = checkHeaderAndPayload(addon_data->strFilePath, addon_data->strFileName);
    // 调用js-callback函数
    catch_error_FWValidate(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback函数
        status,                    // call_js_callback_hdd的第四个参数
        napi_tsfn_blocking             // 非阻塞模式调用
    ));
    //释放句柄
    catch_error_FWValidate(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
}


/**
 * 执行线程完成
*/
void work_complete_checkHeaderAndPayload(napi_env env, napi_status status, void* data)
{
    AddonData_checkHeaderAndPayload* addon_data = (AddonData_checkHeaderAndPayload*)data;
    //释放句柄
    catch_error_FWValidate(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    //回收任务
    catch_error_FWValidate(env, napi_delete_async_work(env, addon_data->work));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data->strFilePath);
    free(addon_data->strFileName);
    free(addon_data);
}

napi_value check_header_and_payload(napi_env env, napi_callback_info info) {
    size_t argc = 3;
    napi_value argv[3];
    size_t strLength;
    // 检查参数个数 
    if (!checkParams_FWValidate(env, info, argc, argv, true)) {
        return NULL;
    }
    // 获取参数    
    napi_value js_cb;// 回调函数    
    napi_value work_name;// 给线程起的名字    
    AddonData_checkHeaderAndPayload* addon_data = NULL;// 结构体    
    addon_data = (AddonData_checkHeaderAndPayload*)malloc(sizeof(*addon_data));// 分配内存空间，在work_complete中会释放
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // strFilePath
    catch_error_FWValidate(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    addon_data->strFilePath = (char*)malloc((++strLength) * sizeof(char));
    catch_error_FWValidate(env, napi_get_value_string_utf8(env, argv[0], addon_data->strFilePath, strLength, &strLength));
    // strFileName
    catch_error_FWValidate(env, napi_get_value_string_utf8(env, argv[1], NULL, NULL, &strLength));
    addon_data->strFileName = (char*)malloc((++strLength) * sizeof(char));
    catch_error_FWValidate(env, napi_get_value_string_utf8(env, argv[1], addon_data->strFileName, strLength, &strLength));
    // 回调函数
    js_cb = argv[2];

    // 创建线程，异步执行
    // 创建线程名字
    catch_error_FWValidate(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe checkHeaderAndPayload Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

    // 把js函数变成任意线程都可以执行的函数
    // 这样就可以在开出来的子线程中调用它了
    // napi_create_threadsafe_function函数作用：
    // 创建一个napi_value持久引用，该值包含可以从多个线程调用的JS函数，调用是异步的，这意味着调用
    // JS回调所用的值将被放置在队列中，对于队列中的每个值，最终将调用JS函数
    catch_error_FWValidate(env, napi_create_threadsafe_function(
        env,
        js_cb,   //JS函数
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_FWValidate,
        &(addon_data->tsfn)   //调用napi_create_threadsafe_function创建的异步安全JS函数
    ));

    // 负责执行上面创建的函数
    // napi_create_async_work函数作用：
    // 分配用于异步执行逻辑的工作对象
    catch_error_FWValidate(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_checkHeaderAndPayload,   // 被调用来执行异步逻辑的本地函数，给定的函数是从工作池调用，与主事件循环线程并行执行
        work_complete_checkHeaderAndPayload,  // 当异步逻辑完成或取消时被调用，在主事件循环线程中调用
        addon_data,          // 用户提供的数据上下文。这将被传递回execute和complete函数
        &(addon_data->work)
    ));
    // 将线程放到待执行队列中
    catch_error_FWValidate(env, napi_queue_async_work(env, addon_data->work));
    return NULL;
}