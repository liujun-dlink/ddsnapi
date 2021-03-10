// libFTPClient
#include <stdlib.h>
#include "dds-napi-common.h"
#ifdef CACHE_SO_LIB
#else
    #include "libFTPClient.h"
#endif

//---int download(char* strIpAddr, int nPort, char* strlogin, char* strPassword, char* strSrcFilePath, char* strFileName, char* strDesFilePath);
napi_value ftpDownloadSync(napi_env env, napi_callback_info info) {
    size_t argc = 8;
    napi_value argv[8];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 检查参数个数
    if (argc < 7)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 获取参数
    size_t strLength;
    // strIpAddr
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* strIpAddr = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], strIpAddr, strLength, &strLength));
    // int nPort,
    int nPort;
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &nPort));
    // char* strlogin
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    char* strlogin = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], strlogin, strLength, &strLength));
    // char* strPassword
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    char* strPassword = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], strPassword, strLength, &strLength));
    // char* strSrcFilePath
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[4], NULL, NULL, &strLength));
    char* strSrcFilePath = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[4], strSrcFilePath, strLength, &strLength));
    // char* strFileName
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[5], NULL, NULL, &strLength));
    char* strFileName = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[5], strFileName, strLength, &strLength));
    // char* strDesFilePath
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[6], NULL, NULL, &strLength));
    char* strDesFilePath = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[6], strDesFilePath, strLength, &strLength));
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->download)
    {
        valuePtr = addonDataSo->download(strIpAddr, nPort, strlogin, strPassword, strSrcFilePath, strFileName, strDesFilePath);
    }
#else
    valuePtr = download(strIpAddr, nPort, strlogin, strPassword, strSrcFilePath, strFileName, strDesFilePath);
#endif
    free(strIpAddr);
    free(strlogin);
    free(strPassword);
    free(strSrcFilePath);
    free(strFileName);
    free(strDesFilePath);

    // so函数返回值
    napi_value methodReult;
    napi_get_null(env, &methodReult);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    // 判断是否有回调函数
    if (argc >= 8) {
        napi_valuetype valueTypeLast;
        napi_typeof(env, argv[7], &valueTypeLast);
        if (valueTypeLast == napi_function) {
            // 构造返回信息
            napi_value errorInfo;
            napi_get_null(env, &errorInfo);
            errorInfo = getErrorMsg(env);
            napi_value callbackParams[2];
            callbackParams[0] = errorInfo;
            callbackParams[1] = methodReult;
            napi_value global;
            napi_get_global(env, &global);
            napi_value returnVal;
            napi_call_function(env, global, argv[7], 2, callbackParams, &returnVal);
        }
    }
    return methodReult;
}

struct AddonDataFtpDownload
{
    char* strIpAddr;
    int nPort;
    char* strlogin;
    char* strPassword;
    char* strSrcFilePath;
    char* strFileName;
    char* strDesFilePath;
};

void freeParamsFtpDownload(void* params)
{
    if (params != NULL)
    {
        struct AddonDataFtpDownload* data = (struct AddonDataFtpDownload*)params;
        if (data != NULL) {
            if (data->strIpAddr != NULL)
            {
                free(data->strIpAddr);
            }
            if (data->strlogin != NULL)
            {
                free(data->strlogin);
            }
            if (data->strPassword != NULL)
            {
                free(data->strPassword);
            }
            if (data->strSrcFilePath != NULL)
            {
                free(data->strSrcFilePath);
            }
            if (data->strFileName != NULL)
            {
                free(data->strFileName);
            }
            if (data->strDesFilePath != NULL)
            {
                free(data->strDesFilePath);
            }            
            free(data);
        }
    }
}

struct AddonDataCBInParams* callSoFtpDownload(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    struct AddonDataFtpDownload* data = (struct AddonDataFtpDownload*)params;
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->download)
    {
        valuePtr = addonDataSo->download(data->strIpAddr, data->nPort, data->strlogin, data->strPassword, data->strSrcFilePath, data->strFileName, data->strDesFilePath);
    }
#else
    valuePtr = download(data->strIpAddr, data->nPort, data->strlogin, data->strPassword, data->strSrcFilePath, data->strFileName, data->strDesFilePath);
#endif

    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value ftpDownload(napi_env env, napi_callback_info info) {
    size_t argc = 8;
    napi_value argv[8];
    // 检查参数个数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc < 8)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 判断是否有回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[7], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, "EINVAL", "There is not callback function.");
        return NULL;
    }
    // 获取参数
    size_t strLength;
    struct AddonDataFtpDownload* params = (struct AddonDataFtpDownload*)malloc(sizeof(struct AddonDataFtpDownload));
    // strIpAddr
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    params->strIpAddr = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], params->strIpAddr, strLength, &strLength));
    // int nPort
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &params->nPort));
    // char* strlogin
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    params->strlogin = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], params->strlogin, strLength, &strLength));
    // char* strPassword
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    params->strPassword = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], params->strPassword, strLength, &strLength));
    // char* strSrcFilePath
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[4], NULL, NULL, &strLength));
    params->strSrcFilePath = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[4], params->strSrcFilePath, strLength, &strLength));
    // char* strFileName
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[5], NULL, NULL, &strLength));
    params->strFileName = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[5], params->strFileName, strLength, &strLength));
    // char* strDesFilePath
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[6], NULL, NULL, &strLength));
    params->strDesFilePath = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[6], params->strDesFilePath, strLength, &strLength));

    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = params;
    addon_data->freeOthersFPtr = &freeParamsFtpDownload;
    // 设置调用so方法指针
    addon_data->callSoMehtodFPtr = &callSoFtpDownload;

    // 初始化
    initWorkTsfn(env, argv[7], addon_data);
    return NULL;
}

//---int upload(char* strIpAddr,int nPort,char* strlogin,char* strPassword,char* strFileName,char* strSrcFilePath, int nBuildPath);
napi_value ftpUploadSync(napi_env env, napi_callback_info info) {
    size_t argc = 8;
    napi_value argv[8];
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    // 检查参数个数
    if (argc < 7)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 获取参数
    size_t strLength;
    // strIpAddr
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    char* strIpAddr = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], strIpAddr, strLength, &strLength));
    // int nPort,
    int nPort;
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &nPort));
    // char* strlogin
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    char* strlogin = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], strlogin, strLength, &strLength));
    // char* strPassword
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    char* strPassword = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], strPassword, strLength, &strLength));
    // char* strFileName
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[4], NULL, NULL, &strLength));
    char* strFileName = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[4], strFileName, strLength, &strLength));
    // char* strSrcFilePath
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[5], NULL, NULL, &strLength));
    char* strSrcFilePath = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[5], strSrcFilePath, strLength, &strLength));
    // int nBuildPath,
    int nBuildPath;
    catchErrorMsg(env, napi_get_value_int32(env, argv[6], &nBuildPath));
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->upload)
    {
        valuePtr = addonDataSo->upload(strIpAddr, nPort, strlogin, strPassword, strFileName, strSrcFilePath, nBuildPath);
    }
#else
    valuePtr = upload(strIpAddr, nPort, strlogin, strPassword, strFileName, strSrcFilePath, nBuildPath);
#endif
    free(strIpAddr);
    free(strlogin);
    free(strPassword);
    free(strFileName);
    free(strSrcFilePath);

    // so函数返回值
    napi_value methodReult;
    napi_get_null(env, &methodReult);
    catchErrorMsg(env, napi_create_int32(env, valuePtr, &methodReult));
    // 判断是否有回调函数
    if (argc >= 8) {
        napi_valuetype valueTypeLast;
        napi_typeof(env, argv[7], &valueTypeLast);
        if (valueTypeLast == napi_function) {
            // 构造返回信息
            napi_value errorInfo;
            napi_get_null(env, &errorInfo);
            errorInfo = getErrorMsg(env);
            napi_value callbackParams[2];
            callbackParams[0] = errorInfo;
            callbackParams[1] = methodReult;
            napi_value global;
            napi_get_global(env, &global);
            napi_value returnVal;
            napi_call_function(env, global, argv[7], 2, callbackParams, &returnVal);
        }
    }
    return methodReult;
}

struct AddonDataFtpUpload
{
    char* strIpAddr;
    int nPort;
    char* strlogin;
    char* strPassword;
    char* strFileName;
    char* strSrcFilePath;
    int nBuildPath;
};

void freeParamsFtpUpload(void* params)
{
    if (params != NULL)
    {
        struct AddonDataFtpUpload* data = (struct AddonDataFtpUpload*)params;
        if (data != NULL) {
            if (data->strIpAddr != NULL) {
                free(data->strIpAddr);
            }
            if (data->strlogin != NULL) {
                free(data->strlogin);
            }
            if (data->strPassword != NULL) {
                free(data->strPassword);
            }
            if (data->strFileName != NULL) {
                free(data->strFileName);
            }
            if (data->strSrcFilePath != NULL) {
                free(data->strSrcFilePath);
            }
            free(data);
        }
    }
}

struct AddonDataCBInParams* callSoFtpUpload(struct AddonDataSo* addonDataSo, void* params)
{
    struct AddonDataCBInParams* addonDataCBInParams = (struct AddonDataCBInParams*)malloc(sizeof(struct AddonDataCBInParams));
    addonDataCBInParams->transferNapiValueFPtr = &transferNapiInt32;
    addonDataCBInParams->error = NULL;
    addonDataCBInParams->value = NULL;
    struct AddonDataFtpUpload* data = (struct AddonDataFtpUpload*)params;
    // 执行so函数
    int valuePtr = 0;
#ifdef CACHE_SO_LIB
    if (addonDataSo && addonDataSo->upload)
    {
        valuePtr = addonDataSo->upload(data->strIpAddr, data->nPort, data->strlogin, data->strPassword, data->strFileName, data->strSrcFilePath, data->nBuildPath);
    }
#else
    valuePtr = upload(data->strIpAddr, data->nPort, data->strlogin, data->strPassword, data->strFileName, data->strSrcFilePath, data->nBuildPath);
#endif

    addonDataCBInParams->value = malloc(sizeof(int));
    *((int*)(addonDataCBInParams->value)) = valuePtr;
    return addonDataCBInParams;
}

napi_value ftpUpload(napi_env env, napi_callback_info info) {
    size_t argc = 8;
    napi_value argv[8];
    // 检查参数个数
    struct AddonDataSo* addonDataSo;
    napi_get_cb_info(env, info, &argc, argv, NULL, (void**)(&addonDataSo));
    if (argc < 8)
    {
        napi_throw_error(env, "EINVAL", "Argument count mismatch.");
        return NULL;
    }
    // 判断是否有回调函数
    napi_valuetype valueTypeLast;
    napi_typeof(env, argv[7], &valueTypeLast);
    if (valueTypeLast != napi_function) {
        napi_throw_error(env, "EINVAL", "There is not callback function.");
        return NULL;
    }
    // 获取参数
    size_t strLength;
    struct AddonDataFtpUpload* params = (struct AddonDataFtpUpload*)malloc(sizeof(struct AddonDataFtpUpload));
    // strIpAddr
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], NULL, NULL, &strLength));
    params->strIpAddr = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[0], params->strIpAddr, strLength, &strLength));
    // int nPort
    catchErrorMsg(env, napi_get_value_int32(env, argv[1], &params->nPort));
    // char* strlogin
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], NULL, NULL, &strLength));
    params->strlogin = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[2], params->strlogin, strLength, &strLength));
    // char* strPassword
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], NULL, NULL, &strLength));
    params->strPassword = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[3], params->strPassword, strLength, &strLength));
    // char* strFileName
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[4], NULL, NULL, &strLength));
    params->strFileName = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[4], params->strFileName, strLength, &strLength));
    // char* strSrcFilePath
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[5], NULL, NULL, &strLength));
    params->strSrcFilePath = (char*)malloc((++strLength) * sizeof(char));
    catchErrorMsg(env, napi_get_value_string_utf8(env, argv[5], params->strSrcFilePath, strLength, &strLength));
    // int nBuildPath
    catchErrorMsg(env, napi_get_value_int32(env, argv[6], &params->nBuildPath));

    struct AddonData* addon_data = (struct AddonData*)malloc(sizeof(struct AddonData));
    struct AddonDataWorkTsfn* addonDataWorkTsfn = (struct AddonDataWorkTsfn*)malloc(sizeof(struct AddonDataWorkTsfn));
    addon_data->addonDataSo = addonDataSo;
    addon_data->addonDataWorkTsfn = addonDataWorkTsfn;
    addon_data->addonDataWorkTsfn->work = NULL;
    addon_data->addonDataWorkTsfn->tsfn = NULL;
    // 其余参数
    addon_data->others = params;
    addon_data->freeOthersFPtr = &freeParamsFtpUpload;
    // 设置调用so方法指针
    addon_data->callSoMehtodFPtr = &callSoFtpUpload;

    // 初始化
    initWorkTsfn(env, argv[7], addon_data);
    return NULL;
}