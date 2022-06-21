project "AlmondNut"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	libdirs { "%{wks.location}/Almond/dependencies/lib/%{cfg.buildcfg}"}

	files
	{
		
		"src/**.h",
		"src/**.cpp",
        
		"vendor/stb/**.h",
		"vendor/stb/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Almond/src",
		"%{wks.location}/Almond/vendor",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.GL}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"Almond",
		"SDL2",
		"SDL2main",
		"glew32s",
		"glu32",
		"opengl32",
		"ImGui",
		"box2d",
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