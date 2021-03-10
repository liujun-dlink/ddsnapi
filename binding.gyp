{
  "targets": [{
    "target_name": "ddsnapi",
    "include_dirs": [
      '.'
    ],
    "defines": [
      #是否缓存动态链接库相关的信息
      'CACHE_SO_LIB',
      'LIB_UPTIME_PATH="<(LIB_PATH)libUptime<(LIB_SUFFIX)"',
      'LIB_CLI_PATH="<(LIB_PATH)libCLI<(LIB_SUFFIX)"',
      'LIB_KILLER_PATH="<(LIB_PATH)libKiller<(LIB_SUFFIX)"',
      'LIB_UTILITYTAR_PATH="<(LIB_PATH)libUtilityTar<(LIB_SUFFIX)"',
      'LIB_SYSTEM_PATH="<(LIB_PATH)libSystem<(LIB_SUFFIX)"',
      'LIB_LED_PATH="<(LIB_PATH)libLed<(LIB_SUFFIX)"',
      'LIB_FTPSETTING_PATH="<(LIB_PATH)libFtpSetting<(LIB_SUFFIX)"',
      'LIB_HDD_PATH="<(LIB_PATH)libHdd<(LIB_SUFFIX)"',
      'LIB_DB_PATH="<(LIB_PATH)libDB<(LIB_SUFFIX)"',
      'LIB_DATETIMESETTING_PATH="<(LIB_PATH)libDatetimeSetting<(LIB_SUFFIX)"',
      'LIB_CONSOLESETTING_PATH="<(LIB_PATH)libConsoleSetting<(LIB_SUFFIX)"',
      'LIB_FTPCLIENT_PATH="<(LIB_PATH)libFTPClient<(LIB_SUFFIX)"',
      'LIB_FWVALIDATE_PATH="<(LIB_PATH)libFWValidate<(LIB_SUFFIX)"',
      'LIB_LANSETTING_PATH="<(LIB_PATH)libLANSetting<(LIB_SUFFIX)"'
    ],
    "link_settings": {
      "libraries": [
        "'<(LIB_PATH)'libUptime'<(LIB_SUFFIX)'",
        "'<(LIB_PATH)'libKiller'<(LIB_SUFFIX)'",
        "'<(LIB_PATH)'libUtilityTar'<(LIB_SUFFIX)'",
        "'<(LIB_PATH)'libSystem'<(LIB_SUFFIX)'",
        "'<(LIB_PATH)'libLed'<(LIB_SUFFIX)'",
        "'<(LIB_PATH)'libFtpSetting'<(LIB_SUFFIX)'",
        "'<(LIB_PATH)'libHdd'<(LIB_SUFFIX)'",
        "'<(LIB_PATH)'libCLI'<(LIB_SUFFIX)'",
        "'<(LIB_PATH)'libDB'<(LIB_SUFFIX)'",
        "'<(LIB_PATH)'libDatetimeSetting'<(LIB_SUFFIX)'",
        "'<(LIB_PATH)'libConsoleSetting'<(LIB_SUFFIX)'",
        "'<(LIB_PATH)'libFTPClient'<(LIB_SUFFIX)'",
        "'<(LIB_PATH)'libFWValidate'<(LIB_SUFFIX)'",
        "'<(LIB_PATH)'libLANSetting'<(LIB_SUFFIX)'"
      ]
    },
    "sources": [
      "dds-napi.c",
      "dds-napi-common.c",
	  "libUptime.c",
	  "libKiller.c",
      "libUtilityTar.c",
      "libSystem.c",
      "libLed.c",
      "libFtpSetting.c",
      "libHdd.c",
      "libCLI.c",
      "libDB.c",
      "libDatetimeSetting.c",
      "libConsoleSetting.c",
      "libFTPClient.c",
      "libFWValidate.c",
      "libLANSetting.c"
    ],
	"cflags!": [
	  "-fno-exceptions"
    ],
    "cflags_cc!": [
	  "-fno-exceptions"
    ],
	"xcode_settings": {
      'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
      'CLANG_CXX_LIBRARY': 'libc++',
      'MACOSX_DEPLOYMENT_TARGET': '10.7'
    },
    "msvs_settings": {
      'VCCLCompilerTool': { 'ExceptionHandling': 1 },
    },
    "conditions": [
      [
        #根据不同操作系统为变量定义不同的值
        #注意LIB_PATH目录的值要以/结尾
        "OS=='win'",
        {
          "variables": {
            'LIB_PATH': '../prebuilds/x64/',
            'LIB_SUFFIX': '.a'
          }
        }			
      ],
      [
        "OS=='linux'",
        {
          "variables": {
            'LIB_PATH': '/lib/dnh/',
            'LIB_SUFFIX': '.so'
          }
        }			
      ],
      [
        "OS=='mac'",
        {
          "variables": {
            'LIB_PATH': '/lib/dnh/',
            'LIB_SUFFIX': '.dylib'
          }
        }
      ]
	]
  }]
}
