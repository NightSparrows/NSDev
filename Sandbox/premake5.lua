project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.cpp",
		"src/**.c",
		"src/**.h"
	}
	
	includedirs {
		"%{wks.location}/SparrowEngine/include",
		"%{wks.location}/NSGameModel/include",
		"%{wks.location}/BasicRenderer/include",
		"%{wks.location}/NS3DEngine/include",
	}
	
	links {
		"SparrowEngine",
		"BasicRenderer",
		"NSGameModel",
		"NS3DEngine"
	}
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "NS_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "NS_RELEASE"
		optimize "on"