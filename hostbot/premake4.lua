project "hostbot"
    kind "ConsoleApp"

    files { "**.cpp", "**.h" }
    includedirs { "..", ".", "../common" }
    links { "common", "interface", "mxml", "event" }

    configuration "vs* or codeblocks"
        files { "../Config.cpp", "../Config.h" }
        pchsource "../Config.cpp"
        pchheader "Config.h"
    configuration "vs*"
        links { "../storm/StormLib" }
    configuration "not vs*"
    	links { "Storm" }
    configuration { "windows", "not vs*" }
	libdirs {"../storm", "../zlib" }
    configuration "not vs*"
        links { "z" }
    configuration "bsd"
        links { "pthread" }
    configuration "windows"
	includedirs { "../zlib" }
        links { "ws2_32", "mswsock", "kernel32", "user32" }
    configuration { "Debug", "vs*" }
        links { "../zlib/zlibd" }
    configuration { "Release", "vs*" }
        links { "../zlib/zlib" }
