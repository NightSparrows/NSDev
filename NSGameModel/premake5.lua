project "NSGameModel"
	kind "StaticLib"
	disablewarnings { "4819" }	-- Disable that fucking shit encoding warnings
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
		"include",
		"vender/MMDFormats/MikuMikuFormats",
		"vender/assimp/include"
	}
	
	links {
		"MMDFormats"
	}
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "NS_DEBUG"
		symbols "on"
		links {
			"vender/assimp/libs/Debug/assimp-vc141-mtd.lib"
		}

	filter "configurations:Release"
		defines "NS_RELEASE"
		optimize "on"
		links {
			"vender/assimp/libs/Release/assimp-vc141-mt.lib"
		}
	
	group "loaderLibraries"
		include "vender/MMDFormats"
	group ""
	
	