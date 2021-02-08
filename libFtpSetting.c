#include <node_api.h>
#include <stdlib.h>
#include <string.h>
#include "libFtpSetting.h"



void catch_error_ftp_data(napi_env env, napi_status status) {
    if (status != napi_ok)
    {
        // napi_extended_error_info��һ���ṹ�壬����������Ϣ
        /**
         * error_message��������ı����?
         * engine_reserved����͸���ֱ�����������ʹ��
         * engine_error_code��VM�ض�������
         * error_code����һ�������n-api״̬����
        */
        const napi_extended_error_info* error_info = NULL;
        // ��ȡ�쳣��Ϣ
        napi_get_last_error_info(env, &error_info);
        // ����ʹ��napi_value��ʾ�Ĵ�����Ϣ
        napi_value error_msg;
        napi_create_string_utf8(
            env,
            error_info->error_message,
            NAPI_AUTO_LENGTH,
            &error_msg
        );
        // �׳�������������JS����һ��uncaughtException�쳣
        napi_fatal_exception(env, error_msg);
        // �˳�����ִ��
        exit(0);
    }
}

/**********************getFtpData**********************************/
typedef struct
{
    /**
     * napi_async_work��һ���ṹ�壬���ڹ����첽�����߳�
     * ͨ��napi_create_async_work����ʵ��
     * ͨ��napi_delete_async_workɾ��ʵ��
    */
    napi_async_work work;             // �����̵߳�����
    /**
     * napi_threadsafe_function��һ��ָ��
     * ��ʾ�ڶ��߳��п���ͨ��napi_call_threadsafe_function�첽���õ�JS����
    */
    napi_threadsafe_function tsfn;    // ����ص�����?
} AddonData_ftp_data;

void work_complete_ftp_data(napi_env env, napi_status status, void* data) {
    AddonData_ftp_data* addon_data = (AddonData_ftp_data*)data;

    //�ͷž��?
    catch_error_ftp_data(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //��������
    catch_error_ftp_data(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux���������ͷŵ�ַ��������⣨�ڻص������н�û�а취��ȡ�����ֵ��������Window��û������
    free(addon_data);
}

void call_js_callback_ftp_data(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // ��ȡ�첽�̷߳��صĽ��?
        napi_value callBackInParams;
        catch_error_ftp_data(env, napi_create_string_utf8(
            env,
            (char*)data,
            NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //��ȡglobal
        napi_value undefined;
        catch_error_ftp_data(env, napi_get_undefined(env, &undefined));

        //���ú���
        catch_error_ftp_data(env, napi_call_function(
            env,
            undefined,   // js�ص���this����
            js_cb,       // js�ص��������?
            1,           // js�ص��������ܲ�������
            &callBackInParams,        // js�ص�������������
            NULL         // js�ص������������return�����ᱻresult���ܵ�
        ));
    }
    if (data != NULL)
    {
        // �ͷ�����
        free((char*)data);
    }
}

napi_value get_ftp_data_sync(napi_env env, napi_callback_info info) {
    //��ȡ����
    size_t argc = 1;
    napi_value argv[1];
    catch_error_ftp_data(env, napi_get_cb_info(
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
    char* time = getFtpData();
    //ת����
    napi_value world;
    catch_error_ftp_data(env, napi_create_string_utf8(env, time, NAPI_AUTO_LENGTH, &world));
    //�ص�����
    napi_value* callbackParams = &world;
    size_t callbackArgc = 1;
    napi_value global;
    napi_get_global(env, &global);
    napi_value callbackRs;
    napi_call_function(env, global, argv[0], callbackArgc, callbackParams, &callbackRs);
    return world;
}

/**
 * ִ���߳�
*/
static void execute_work_ftp_data(napi_env env, void* data)
{
    // ��������Ĳ���?
    AddonData_ftp_data* addon_data = (AddonData_ftp_data*)data;
    //��ȡjs-callback����
    catch_error_ftp_data(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //ִ��so����
    char* time = getFtpData();
    char* uptime = (char*)malloc(sizeof(char) * (strlen(time) + 1));
    strcpy(uptime, time);
    // printf("sizeof(strlen(time)): %i\n", sizeof(strlen(time)));
    // printf("sizeof(char) * strlen(time): %i\n", sizeof(char) * strlen(time));
    // printf("sizeof(char*) * strlen(time): %i\n", sizeof(char*) * strlen(time));
    // printf("strlen(time): %i\n", strlen(time));
    // ����js-callback����
    catch_error_ftp_data(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback����
        uptime,                    // call_js_callback�ĵ��ĸ�����
        napi_tsfn_blocking             // ������ģʽ����
    ));
    //�ͷž��?
    catch_error_ftp_data(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_releaseָʾ��ǰ�̲߳��ٵ����̰߳�ȫ������napi_tsfn_abortָʾ����ǰ�߳��⣬�����̲߳�Ӧ�ٵ����̰߳�ȫ����
    ));

}

napi_value get_ftp_data(napi_env env, napi_callback_info info) {
    // ���߳��������?
    napi_value work_name;
    //��ȡ����
    size_t argc = 1;
    napi_value argv[1];
    catch_error_ftp_data(env, napi_get_cb_info(
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
    // �����߳�����
    catch_error_ftp_data(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));
    // �����ڴ�ռ䣬��work_complete�л��ͷ�
    AddonData_ftp_data* addon_data = (AddonData_ftp_data*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // ��js������������̶߳�����ִ�еĺ���?
    // �����Ϳ����ڿ����������߳��е�������
    // napi_create_threadsafe_function�������ã�
    // ����һ��napi_value�־����ã���ֵ�������ԴӶ���̵߳��õ�JS�������������첽�ģ�����ζ�ŵ���
    // JS�ص����õ�ֵ���������ڶ����У����ڶ����е�ÿ��ֵ�����ս�����JS����
    catch_error_ftp_data(env, napi_create_threadsafe_function(
        env,
        argv[0],   //JS����
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_ftp_data,
        &(addon_data->tsfn)   //����napi_create_threadsafe_function�������첽��ȫJS����
    ));
    // ����ִ�����洴���ĺ���
   // napi_create_async_work�������ã�
    // ���������첽ִ���߼��Ĺ�������
    catch_error_ftp_data(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_ftp_data,   // ��������ִ���첽�߼��ı��غ����������ĺ����Ǵӹ����ص��ã������¼�ѭ���̲߳���ִ��
        work_complete_ftp_data,  // ���첽�߼���ɻ�ȡ��ʱ�����ã������¼�ѭ���߳��е���?
        addon_data,          // �û��ṩ�����������ġ��⽫�����ݻ�execute��complete����
        &(addon_data->work)
    ));
    // ���̷߳ŵ���ִ�ж�����
    catch_error_ftp_data(env, napi_queue_async_work(
        env,
        addon_data->work   //Ҫִ���̵߳ľ��?
    ));
    return NULL;
}
/**********************setFtpData**********************************/

typedef struct
{
    /**
     * napi_async_work��һ���ṹ�壬���ڹ����첽�����߳�
     * ͨ��napi_create_async_work����ʵ��
     * ͨ��napi_delete_async_workɾ��ʵ��
    */
    napi_async_work work;             // �����̵߳�����
    /**
     * napi_threadsafe_function��һ��ָ��
     * ��ʾ�ڶ��߳��п���ͨ��napi_call_threadsafe_function�첽���õ�JS����
    */
    napi_threadsafe_function tsfn;    // ����ص�����?
    char* ftpServer;
    int ftpPort;
    char* ftpUsername;
    char* ftpPassword;
} AddonData_set_ftp_data;

napi_value set_ftp_data_sync(napi_env env, napi_callback_info info) {
    //��ȡ����
    size_t argc = 5;
    napi_value argv[5];
    catch_error_ftp_data(env, napi_get_cb_info(
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
    //��ȡ����
    size_t strLength;
    //ftpServer
    catch_error_ftp_data(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char ftpServer[++strLength];
    catch_error_ftp_data(env, napi_get_value_string_utf8(env, argv[0], ftpServer, strLength, &strLength));
    //ftpPort
    int ftpPort;
    catch_error_ftp_data(env, napi_get_value_int32(env, argv[1], &ftpPort));
    //ftpUsername
    catch_error_ftp_data(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    char ftpUsername[++strLength];
    catch_error_ftp_data(env, napi_get_value_string_utf8(env, argv[2], ftpUsername, strLength, &strLength));
    //ftpServer
    catch_error_ftp_data(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    char ftpPassword[++strLength];
    catch_error_ftp_data(env, napi_get_value_string_utf8(env, argv[3], ftpPassword, strLength, &strLength));
    //ִ��so����
    int time = setFtpData(ftpServer, ftpPort, ftpUsername, ftpPassword);
    //ת����
    napi_value world;
    catch_error_ftp_data(env, napi_create_int32(env, time, &world));
    //�ص�����
    napi_value* callbackParams = &world;
    size_t callbackArgc = 1;
    napi_value global;
    napi_get_global(env, &global);
    napi_value callbackRs;
    napi_call_function(env, global, argv[4], callbackArgc, callbackParams, &callbackRs);
    return world;
}

void work_complete_set_ftp_data(napi_env env, napi_status status, void* data) {
    AddonData_set_ftp_data* addon_data = (AddonData_set_ftp_data*)data;

    //�ͷž��?
    catch_error_ftp_data(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release
    ));

    //��������
    catch_error_ftp_data(env, napi_delete_async_work(
        env, addon_data->work
    ));

    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    // Linux���������ͷŵ�ַ��������⣨�ڻص������н�û�а취��ȡ�����ֵ��������Window��û������
    free(addon_data->ftpServer);
    free(addon_data->ftpUsername);
    free(addon_data->ftpPassword);
    free(addon_data);
}

void call_js_callback_set_ftp_data(napi_env env, napi_value js_cb, void* context, void* data) {
    if (env != NULL)
    {
        // ��ȡ�첽�̷߳��صĽ��?
        napi_value callBackInParams;
        catch_error_ftp_data(env, napi_create_uint32(
            env,
            data,
            //NAPI_AUTO_LENGTH,
            &callBackInParams
        ));

        //��ȡglobal
        napi_value undefined;
        catch_error_ftp_data(env, napi_get_undefined(env, &undefined));

        //���ú���
        catch_error_ftp_data(env, napi_call_function(
            env,
            undefined,   // js�ص���this����
            js_cb,       // js�ص��������?
            1,           // js�ص��������ܲ�������
            &callBackInParams,        // js�ص�������������
            NULL         // js�ص������������return�����ᱻresult���ܵ�
        ));
    }
}

/**
 * ִ���߳�
*/
static void execute_work_set_ftp_data(napi_env env, void* data)
{
    // ��������Ĳ���?
    AddonData_set_ftp_data* addon_data = (AddonData_set_ftp_data*)data;
    //��ȡjs-callback����
    catch_error_ftp_data(env, napi_acquire_threadsafe_function(addon_data->tsfn));
    //ִ��so����
    int result = setFtpData(addon_data->ftpServer, addon_data->ftpPort, addon_data->ftpUsername, addon_data->ftpPassword);
    // ����js-callback����
    catch_error_ftp_data(env, napi_call_threadsafe_function(
        addon_data->tsfn,                       // js-callback����
        result,                    // call_js_callback�ĵ��ĸ�����
        //"Async Callback Result",
        napi_tsfn_blocking             // ������ģʽ����
    ));
    //�ͷž��?
    catch_error_ftp_data(env, napi_release_threadsafe_function(
        addon_data->tsfn,
        napi_tsfn_release       //napi_tsfn_releaseָʾ��ǰ�̲߳��ٵ����̰߳�ȫ������napi_tsfn_abortָʾ����ǰ�߳��⣬�����̲߳�Ӧ�ٵ����̰߳�ȫ����
    ));

}

napi_value set_ftp_data(napi_env env, napi_callback_info info) {
    // ���߳��������?
    napi_value work_name;
    //��ȡ����
    size_t argc = 5;
    napi_value argv[5];
    catch_error_ftp_data(env, napi_get_cb_info(
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
    //��ȡ����
    size_t strLength;
    //ftpServer
    catch_error_ftp_data(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char ftpServer[++strLength];
    catch_error_ftp_data(env, napi_get_value_string_utf8(env, argv[0], ftpServer, strLength, &strLength));
    //ftpPort
    int ftpPort;
    catch_error_ftp_data(env, napi_get_value_int32(env, argv[1], &ftpPort));
    //ftpUsername
    catch_error_ftp_data(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    char ftpUsername[++strLength];
    catch_error_ftp_data(env, napi_get_value_string_utf8(env, argv[2], ftpUsername, strLength, &strLength));
    //ftpServer
    catch_error_ftp_data(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    char ftpPassword[++strLength];
    catch_error_ftp_data(env, napi_get_value_string_utf8(env, argv[3], ftpPassword, strLength, &strLength));
    // �����߳�����
    catch_error_ftp_data(env, napi_create_string_utf8(
        env,
        "N-API Thread-safe Get Performancen Work Item",
        NAPI_AUTO_LENGTH,
        &work_name
    ));

//     // �����ڴ�ռ䣬��work_complete�л��ͷ�
    AddonData_set_ftp_data* addon_data = (AddonData_set_ftp_data*)malloc(sizeof(*addon_data));
    addon_data->work = NULL;
    addon_data->tsfn = NULL;
    addon_data->ftpPort = ftpPort;
    addon_data->ftpServer = (char*)malloc(sizeof(char) * ((strlen(ftpServer) + 1)));
    strcpy(addon_data->ftpServer, ftpServer);
    addon_data->ftpUsername = (char*)malloc(sizeof(char) * (strlen(ftpUsername) + 1));
    strcpy(addon_data->ftpUsername, ftpUsername);
    addon_data->ftpPassword = (char*)malloc(sizeof(char) * (strlen(ftpPassword) + 1));
    strcpy(addon_data->ftpPassword, ftpPassword);
//     // ��js������������̶߳�����ִ�еĺ���?
//     // �����Ϳ����ڿ����������߳��е�������
//     // napi_create_threadsafe_function�������ã�
//     // ����һ��napi_value�־����ã���ֵ�������ԴӶ���̵߳��õ�JS�������������첽�ģ�����ζ�ŵ���
//     // JS�ص����õ�ֵ���������ڶ����У����ڶ����е�ÿ��ֵ�����ս�����JS����
    catch_error_ftp_data(env, napi_create_threadsafe_function(
        env,
        argv[4],   //JS����
        NULL,
        work_name,
        1,
        1,
        NULL,
        NULL,
        NULL,
        call_js_callback_set_ftp_data,
        &(addon_data->tsfn)   //����napi_create_threadsafe_function�������첽��ȫJS����
    ));
    // ����ִ�����洴���ĺ���
   // napi_create_async_work�������ã�
    // ���������첽ִ���߼��Ĺ�������
    catch_error_ftp_data(env, napi_create_async_work(
        env,
        NULL,
        work_name,
        execute_work_set_ftp_data,   // ��������ִ���첽�߼��ı��غ����������ĺ����Ǵӹ����ص��ã������¼�ѭ���̲߳���ִ��
        work_complete_set_ftp_data,  // ���첽�߼���ɻ�ȡ��ʱ�����ã������¼�ѭ���߳��е���?
        addon_data,          // �û��ṩ�����������ġ��⽫�����ݻ�execute��complete����
        &(addon_data->work)
    ));
    // ���̷߳ŵ���ִ�ж�����
    catch_error_ftp_data(env, napi_queue_async_work(
        env,
        addon_data->work   //Ҫִ���̵߳ľ��?
    ));
    return NULL;
}

/***************************init***********************************/

// napi_value init(napi_env env, napi_value exports)
// {
//     /************************getFtpData********************************/
//     // ��ȡ��������
//     napi_property_descriptor getFtpDataSync = {
//         "getFtpDataSync",
//         NULL,
//         get_ftp_data_sync,
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
//         &getFtpDataSync
//     );

//     // ��ȡ��������
//     napi_property_descriptor getFtpData = {
//         "getFtpData",
//         NULL,
//         get_ftp_data,
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
//         &getFtpData
//     );
//     /**********************setFtpData**********************************/
//     // ��ȡ��������
//     napi_property_descriptor setFtpDataSync = {
//         "setFtpDataSync",
//         NULL,
//         set_ftp_data_sync,
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
//         &setFtpDataSync
//     );

//     // ��ȡ��������
//     napi_property_descriptor setFtpData = {
//         "setFtpData",
//         NULL,
//         set_ftp_data,
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
//         &setFtpData
//     );
//     return exports;
// }

// NAPI_MODULE(NODE_GYP_MODULE_NAME, init);