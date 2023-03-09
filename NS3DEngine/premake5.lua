project "NS3DEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.cpp",
		"include/**.h"
	}
	
	includedirs {
		"%{wks.location}/SparrowEngine/include",
		"include"
	}
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "NS_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "NS_RELEASE"
		optimize "on"