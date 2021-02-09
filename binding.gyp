{
	"targets": [
		{
			"target_name": "ddsnapi",
			"include_dirs": [
				"."
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
				"libDatetimeSetting.c"
			],
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
					"/lib/dnh/libDatetimeSetting.so"
				]
			}
		}
	]
}