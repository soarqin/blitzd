project "interface"
    kind "StaticLib"
    files { "**.cpp", "**.h" }
    includedirs { "..", "../common" }
    configuration "not vs*"
	buildoptions { "-std=c++11" }
	linkoptions { "-std=c++11" }
