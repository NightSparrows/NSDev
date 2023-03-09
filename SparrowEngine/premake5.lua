project "SparrowEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/engine/**.cpp",
		"src/engine/**.c",
		"include/**.h",
		"include/**.hpp"
	}
	
	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"NS_ENGINE",
		"SFML_STATIC"					-- 你必須要加入此編譯器材不會出錯
	}
	
	includedirs {
		"include",
		"src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.yaml}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stduuid}",
		"%{IncludeDir.SFML}",
		"%{IncludeDir.OpenCL}"
	}
	
	links {
		"GLFW",
		"stb_image",
		"yaml-cpp",
		"ImGui",
		"SFML"
	}
	
	filter "system:windows"
		systemversion "latest"
		files { "src/platform/Windows/**.cpp", "src/platform/Windows/**.h" }
		links {
			"%{wks.location}/SparrowEngine/vender/OpenCL/lib/OpenCL.lib"
		}

	filter "configurations:Debug"
		defines "NS_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "NS_RELEASE"
		optimize "on"
		
	newoption {
		trigger = "renderAPI",
		value = "API",
		description = "Choose a particular Render API for rendering",
		allowed = {
			{ "opengl", "OpenGL" },
			{ "vulkan", "Vulkan" }
		},
		default = "opengl"
	}
	
	filter { "options:renderAPI=opengl" }
		files {
			-- OpenGL implemnted.
			"src/platform/OpenGL/**.cpp",
			"src/platform/OpenGL/**.h"
		}
		defines {
			"NS_RENDER_API_OPENGL"
		}
		links {
			"Glad"
		}
		
	filter { "options:renderAPI=vulkan" }
		files {
			-- Vulkan implemnted.
			"src/platform/Vulkan/**.cpp",
			"src/platform/Vulkan/**.h"
		}
		defines {
			"NS_RENDER_API_VULKAN"
		}
		includedirs {
			"D:/Program Files/VulkanSDK/1.2.189.2/Include"
		}
		links {
			"D:/Program Files/VulkanSDK/1.2.189.2/Lib/vulkan-1.lib"
		}