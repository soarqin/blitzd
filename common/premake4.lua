project "common"
    kind "StaticLib"
    files { "**.cpp", "**.h" }
    excludes { "utils/Event_*.*", "utils/Mutex_*.*", "utils/RWLock_*.*", "utils/Thread_*.*", "network/*.iocp.*" }
    includedirs { "..", "." }
    configuration "vs*"
        includedirs { "../libevent/include", "../mxml" }
    configuration "not vs*"
        buildoptions { "-std=c++11" }
        linkoptions { "-std=c++11" }
    configuration "vs* or codeblocks"
        files { "../Config.cpp", "../Config.h" }
        pchsource "../Config.cpp"
        pchheader "Config.h"
