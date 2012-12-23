solution "Blizzard"
    location "build"
    language "C++"
    objdir "obj"

    configurations { "Debug", "Release" }

    configuration "windows"
        defines { "WIN32", "_WIN32" }

    configuration "bsd"
        includedirs { "/usr/local/include" }
        libdirs { "/usr/local/lib" }

    configuration "vs*"
        flags { "StaticRuntime" }

    configuration "not vs*"
        buildoptions { "-Wall", "-fexceptions", "-fno-strict-aliasing", "-Wno-multichar" }

    configuration "Debug"
        defines { "_DEBUG" }
        flags { "Symbols" }
	targetdir "bin/Debug"
 
    configuration "Release"
        flags { "Optimize" }
	targetdir "bin/Release"

    if os.is("windows") then
        include "libevent"
	include "sqlite3"
    end
    include "common"
    include "interface"
    include "mxml"
    include "hostbot"
    include "blitzd"
    include "baald"
    include "shald"
