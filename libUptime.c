#include <node_api.h>
#include <stdlib.h>
#include <string.h>
#include "libUptime.h"

typedef struct
{
    napi_async_work work;
    napi_threadsafe_function tsfn;
} AddonData;

void catch_error_uptime(napi_env env, napi_status status) {
    if (status != napi_ok)
    {
        const napi_extended_error_info* error_info = NULL;
        napi_get_last_error_info(env, &error_info);
        napi_value error_msg;
        napi_create_string_utf8(
            env,
            error_info->error_message,
            NAPI_AUTO_LENGTH,
            &error_msg
        );
        napi_fatal_exception(env, error_msg);
        exit(0);
    }
}

void work_complete_uptime(napi_env env, napi_status status, void* data) {
    AddonData* addon_data = (AddonData*)data;
    catch_error_uptime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));
    catch_error_uptime(env, napi_delete_async_work(
        env, addon_data->work
    ));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    free(addon_data);
}

void call_js_callback_uptime(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        napi_value callBackInParams;
        catch_error_uptime(env, napi_create_string_utf8(
            env,
            (char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));
        napi_value undefined;
        catch_error_uptime(env, napi_get_undefined(env, &undefined));
        catch_error_uptime(env, napi_call_function(
            env,
            undefined,
            js_cb,
            1,
            &callBackInParams,
            NULL
        ));
    }
    if (data != NULL)
    {
        free((char *)data);
    }
}

napi_value get_uptime_sync(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    catch_error_uptime(env, napi_get_cb_info(
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
    //ִ��so����
    char* time = getUptime();
    //ת����
    napi_value world;
    catch_error_uptime(env, napi_create_string_utf8(env, time, NAPI_AUTO_LENGTH, &world));
    //�ص�����
    napi_value* callbackParams = &world;
    size_t callbackArgc = 1;
    napi_value global;
    napi_get_global(env, &global);
    napi_value callbackRs;
    napi_call_function(env, global, argv[0], callbackArgc, callbackParams, &callbackRs);
    return world;
}

static void execute_work_uptime(napi_env env, void* data)
{
    AddonData* addon_data = (AddonData*)data;
    catch_error_uptime(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    char* time = getUptime();
	char* uptime = (char*)malloc(sizeof(char) * (strlen(time) + 1));
	strcpy(uptime, time);
    catch_error_uptime(env, napi_call_threadsafe_function(
        addon_data->tsfn,
        uptime,
        napi_tsfn_blocking
    ));
    catch_error_uptime(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

}

napi_value get_uptime(napi_env env, napi_callback_info info) {
    napi_value work_name;
    size_t argc = 1;
    napi_value argv[1];
    catch_error_uptime(env, napi_get_cb_info(
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
    catch_error_uptime(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    AddonData* addon_data = (AddonData*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    catch_error_uptime(env, napi_create_threadsafe_function(
        env,
        argv[0],
        NULL,
        work_name,
        5,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_uptime,
        &(addon_data->tsfn)
    ));
    catch_error_uptime(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_uptime,
        work_complete_uptime,
        addon_data,
        &(addon_data->work)
    ));
    catch_error_uptime(env, napi_queue_async_work(
        env,
        addon_data->work
    ));
    return NULL;
}

// napi_value init(napi_env env, napi_value exports)
// {

//     // ��ȡ��������
//     napi_property_descriptor getUptimeSync = {
//         "getUptimeSync",
//         NULL,
//         get_uptime_sync,
//         NULL,
//         NULL,
//         NULL,
//         napi_default,
//         NULL
//     };
//     //��¶�ӿ�
//     napi_define_properties(
//         env,
//         exports,
//         1,
//         &getUptimeSync
//     );

//     // ��ȡ��������
//     napi_property_descriptor getUptime = {
//         "getUptime",
//         NULL,
//         get_uptime,
//         NULL,
//         NULL,
//         NULL,
//         napi_default,
//         NULL
//     };
//     //��¶�ӿ�
//     napi_define_properties(
//         env,
//         exports,
//         1,
//         &getUptime
//     );

//     return exports;
// }

// NAPI_MODULE(NODE_GYP_MODULE_NAME, init);
