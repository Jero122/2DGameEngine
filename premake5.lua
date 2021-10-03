workspace "Almond"
	architecture "x64"
		startproject "AlmondNut"
	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["ImGui"] = "Almond/vendor/imgui"
IncludeDir["SDL2"] = "Almond/dependencies/include/SDL"
IncludeDir["GL"] = "Almond/dependencies/include/GL"
IncludeDir["box2d"] = "Almond/dependencies/include/box2d"
IncludeDir["stb"] = "Almond/vendor/stb"
IncludeDir["glm"] = "Almond/vendor/glm"

include "Almond/vendor/imgui"

project "Almond"
	location "Almond"
	kind "SharedLib"
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
		"Almond/vendor",
		"Almond/dependencies/include",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.SDL}",
		"%{IncludeDir.GL}",
		"%{IncludeDir.box2d}"
	}

	links
	{
		"SDL2",
		"SDL2main",
		"glew32s",
		"opengl32",
		"ImGui",
		"box2d",
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ALM_PLATFORM_WINDOWS",
			"ALM_BUILD_DLL"
		}

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/AlmondNut")
	}

	filter "configurations:Debug"
		defines "ALM_DEBUG"
		runtime "Debug"
		symbols "on"
		
		
	filter "configurations:Release"
		defines "ALM_RELEASE"
		runtime "Release"
		optimize "on"



project "AlmondNut"
	location "AlmondNut"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	libdirs { "Almond/dependencies/lib/%{cfg.buildcfg}"}

	files
	{
		
		"AlmondNut/src/**.h",
		"AlmondNut/src/**.cpp",
		"AlmondNut/vendor/stb/**.h",
		"AlmondNut/vendor/stb/**.cpp"
	}

	includedirs
	{
		"Almond/src",
		"Almond/vendor",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.GL}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Almond",
		"SDL2",
		"SDL2main",
		"glew32s",
		"opengl32",
		"ImGui",
		"box2d",
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ALM_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "ALM_DEBUG"
		runtime "Debug"
		symbols "on"
		
		
	filter "configurations:Release"
		defines "ALM_RELEASE"
		runtime "Release"
		optimize "on"