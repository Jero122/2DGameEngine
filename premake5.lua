workspace "Almond"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["ImGui"] = "Almond/vendor/imgui"
IncludeDir["stb"] = "Almond/vendor/stb"
IncludeDir["glm"] = "Almond/vendor/glm"

include "Almond/vendor/imgui"

project "Almond"
	location "Almond"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	libdirs { "Almond/dependencies/lib/%{cfg.buildcfg}"}

	files
	{
		
		"Almond/src/**.h",
		"Almond/src/**.cpp",
		"Almond/vendor/stb/**.h",
		"Almond/vendor/stb/**.cpp"
	}

	includedirs
	{
		"Almond/src",
		"Almond/src/vendor",
		"Almond/dependencies/include",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"ImGui",
		"SDL2",
		"SDL2main",
		"glew32",
		"opengl32",
		"ImGui",
		"box2d"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ALM_PLATFORM_WINDOWS",
			"ALM_BUILD_DLL"
		}

	filter "configurations:Debug"
		defines "ALM_DEBUG"
		runtime "Debug"
		symbols "on"
		
		
	filter "configurations:Release"
		defines "ALM_RELEASE"
		runtime "Release"
		optimize "on"