solution "Blizzard"
    location "build"
    language "C++"
    objdir "obj"

    configurations { "Debug", "Release" }

    configuration "windows"
        defines { "WIN32", "_WIN32" }

    configuration "bsd"
        includedirs { "/usr/local/include", "/opt/local/include" }
        libdirs { "/usr/local/lib", "/opt/local/lib" }

    configuration "not vs*"
        buildoptions { "-Wall", "-fexceptions", "-fno-strict-aliasing", "-Wno-multichar" }

    configuration "vs*"
        buildoptions { "/wd4996" }
        defines { "_CRT_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_DEPRECATE" }

    configuration "Debug"
        defines { "_DEBUG" }
        flags { "Symbols" }
        targetdir "bin/Debug"
 
    configuration "Release"
        flags { "Optimize" }
        targetdir "bin/Release"

    if os.is("windows") then
        include "zlib"
        include "sqlite3"
        include "libevent"
        include "mxml"
    end
    include "common"
    include "interface"
    include "blitzd"
    include "baald"
    include "shald"
