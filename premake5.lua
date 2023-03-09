workspace "NSDev"
	characterset ("Unicode")
	architecture "x64"
	startproject "Sandbox"
	
	configurations {
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/SparrowEngine/vender/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/SparrowEngine/vender/Glad/include"
IncludeDir["stb_image"] = "%{wks.location}/SparrowEngine/vender/stb_image"
IncludeDir["yaml"] = "%{wks.location}/SparrowEngine/vender/yaml-cpp/include"
IncludeDir["ImGui"] = "%{wks.location}/SparrowEngine/vender/ImGui"
IncludeDir["stduuid"] = "%{wks.location}/SparrowEngine/vender/stduuid/include"
IncludeDir["SFML"] = "%{wks.location}/SparrowEngine/vender/SFML/include"
IncludeDir["OpenCL"] = "%{wks.location}/SparrowEngine/vender/OpenCL/include"

group "Dependencies"
		include "SparrowEngine/vender/GLFW"
		include "SparrowEngine/vender/Glad"
		include "SparrowEngine/vender/stb_image"
		include "SparrowEngine/vender/yaml-cpp"
		include "SparrowEngine/vender/ImGui"
		include "SparrowEngine/vender/SFML"
group ""

group "Engine"
	include "SparrowEngine"
	include "NS3DEngine"
	include "BasicRenderer"
	include "NSGameModel"
group ""
include "Sandbox"