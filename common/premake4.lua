project "common"
    kind "StaticLib"
    files { "**.cpp", "**.h" }
    excludes { "utils/Event_*.*", "utils/Mutex_*.*", "utils/RWLock_*.*", "utils/Thread_*.*", "network/*.iocp.*" }
    includedirs { "..", "." }
    configuration "vs*"
        includedirs "../libevent/include"
    configuration "vs* or codeblocks"
        files { "../Config.cpp", "../Config.h" }
        pchsource "../Config.cpp"
        pchheader "Config.h"
