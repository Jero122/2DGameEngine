workspace "Almond"
	architecture "x64"
	startproject "AlmondNut"
	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["ImGui"] = "%{wks.location}/Almond/vendor/imgui"
IncludeDir["SDL2"] = "%{wks.location}/Almond/dependencies/include/SDL"
IncludeDir["GL"] = "%{wks.location}/Almond/dependencies/include/GL"
IncludeDir["Box2D"] = "%{wks.location}/Almond/dependencies/include/box2d"
IncludeDir["stb"] = "%{wks.location}/Almond/vendor/stb"
IncludeDir["glm"] = "%{wks.location}/Almond/vendor/glm"
IncludeDir["yaml"] = "%{wks.location}/Almond/vendor/yaml-cpp/include"
IncludeDir["assimp"] = "%{wks.location}/Almond/dependencies/include/assimp"

include "Almond/vendor/imgui"
include "Almond/vendor/yaml-cpp"
include "AlmondNut"

project "Almond"
	location "Almond"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

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
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.GL}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.yaml}",
		"%{IncludeDir.assimp}"
	}

	links
	{
		"SDL2",
		"SDL2main",
		"glew32s",
		"opengl32",
		"ImGui",
		"box2d",
		"ImGui",
		"yaml-cpp",
		"assimp-vc143-mt"
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
