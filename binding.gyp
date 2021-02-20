{
  "targets": [{
    "target_name": "ddsnapi",
    "include_dirs": [
      './'
    ],
    "sources": [
        "dds-napi.c",
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
    "conditions": [
		[
			"OS=='win'",
			{
			    "link_settings": {
					"libraries": [
						"../prebuilds/x64/libUptime.a",
						"../prebuilds/x64/libKiller.a",
						"../prebuilds/x64/libUtilityTar.a",
						"../prebuilds/x64/libSystem.a",
						"../prebuilds/x64/libLANSetting.a",
						"../prebuilds/x64/libCLI.a",
						"../prebuilds/x64/libDatetimeSetting.a",
						"../prebuilds/x64/libFTPClient.a",
						"../prebuilds/x64/libFWValidate.a",
						"../prebuilds/x64/libLed.a",
						"../prebuilds/x64/libFtpSetting.a",
						"../prebuilds/x64/libHdd.a",
						"../prebuilds/x64/libDB.a",
						"../prebuilds/x64/libConsoleSetting.a",
					]
				}			
			}			
		],
		[
			"OS=='linux'",
			{
			    "link_settings": {
					"libraries": [
						"/lib/dnh/libUptime.so",
						"/lib/dnh/libKiller.so",
						"/lib/dnh/libUtilityTar.so",
						"/lib/dnh/libSystem.so",
						"/lib/dnh/libLed.so",
						"/lib/dnh/libFtpSetting.so",
						"/lib/dnh/libHdd.so",
						"/lib/dnh/libCLI.so",
						"/lib/dnh/libDB.so",
						"/lib/dnh/libDatetimeSetting.so",
						"/lib/dnh/libConsoleSetting.so",
						"/lib/dnh/libFTPClient.so",
						"/lib/dnh/libFWValidate.so",
						"/lib/dnh/libLANSetting.so"
					]
				}
			}			
		]
	]
  }]
}
