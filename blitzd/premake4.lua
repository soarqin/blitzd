project "blitzd"
    kind "ConsoleApp"

    files { "**.cpp", "**.h" }
    includedirs { "..", ".", "../common" }
    links { "common", "interface", "mxml", "sqlite3", "event" }

    configuration "vs* or codeblocks"
        files { "../Config.cpp", "../Config.h" }
        pchsource "../Config.cpp"
        pchheader "Config.h"
    configuration "vs*"
	includedirs { "../gmp" }
    configuration { "windows", "not vs*" }
	libdirs {"../zlib" }
    configuration "not vs*"
        links { "gmp", "z" }
    configuration "windows"
	includedirs { "../zlib" }
        links { "ws2_32", "mswsock", "kernel32", "user32" }
    configuration { "Debug", "vs*" }
        links { "../zlib/zlibd" }
    configuration { "Release", "vs*" }
        links { "../zlib/zlib" }
