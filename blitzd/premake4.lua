project "blitzd"
    kind "ConsoleApp"

    files { "**.cpp", "**.h" }
    includedirs { "..", ".", "../common" }
    links { "common", "interface", "event", "mxml", "sqlite3" }

    configuration "vs* or codeblocks"
        files { "../Config.cpp", "../Config.h" }
        pchsource "../Config.cpp"
        pchheader "Config.h"
    configuration "vs*"
        includedirs { "../zlib", "../libevent", "../sqlite3", "../mxml", "../prebuilt/include" }
        links { "zlib", "mpir" }
    configuration "not vs*"
	buildoptions { "-std=c++11" }
	linkoptions { "-std=c++11" }
        links { "gmp", "z" }
    configuration "windows"
        links { "ws2_32", "mswsock", "kernel32", "user32" }
    configuration { "Debug", "vs*" }
        libdirs {"../prebuilt/lib/Debug"}
    configuration { "Release", "vs*" }
        libdirs {"../prebuilt/lib/Release"}
