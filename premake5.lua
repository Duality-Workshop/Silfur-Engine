include "scripts/FindVulkan.lua"

checkVulkanDynamicLibWin32()

IncludeDir = {}
IncludeDir["SDL2"] = "%{wks.location}/Silfur/vendor/SDL2/include"
IncludeDir["ImGui"] = "%{wks.location}/Silfur/vendor/imgui"
IncludeDir["ImgGuiExamples"] = "%{wks.location}/Silfur/vendor/imgui/examples"
IncludeDir["glm"] = "%{wks.location}/Silfur/vendor/glm"
IncludeDir["spdlog"] = "%{wks.location}/Silfur/vendor/spdlog/include"
IncludeDir["stb"] = "%{wks.location}/Silfur/vendor/stb"
IncludeDir["portableSnippets"] = "%{wks.location}/Silfur/vendor/portable-snippets"
IncludeDir["tinyobjloader"] = "%{wks.location}/Silfur/vendor/tinyobjloader"

workspace "Silfur Engine"
	architecture "x86_64"
	characterset "Default"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Development"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}
	
	removefiles
	{
		"**.spv"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Silfur/vendor/imgui"
group ""

group "Tools"
	project "CompileShaders"
		location "tools"
		kind "Utility"
		files
		{
			"%{wks.location}/**.vert",
			"%{wks.location}/**.frag",
			"%{wks.location}/**.tesc",
			"%{wks.location}/**.tese",
			"%{wks.location}/**.geom",
			"%{wks.location}/**.comp"
		}
		
		local vulkanPath = GetVulkanPath()
		local glslCompiler = ""
		if (VULKAN_SDK_FOUND) then
			glslCompiler = vulkanPath .. '/bin/glslangValidator'
		end
		
		filter "files:**/*"
			buildmessage "Compiling %{file.name}..."
			buildcommands(glslCompiler .. ' -V -o "%{file.directory}%{file.name}.spv" "%{file.relpath}"')
			buildoutputs "%{file.directory}/%{file.name}.spv"
group ""

project "Silfur"
	location "Silfur"
	language "C++"
	cppdialect "C++17"
	kind "StaticLib"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "sfpch.hpp"
	pchsource "%{wks.location}/Silfur/src/sfpch.cpp"

	files
	{
		"%{prj.location}/src/**.hpp",
		"%{prj.location}/src/**.inl",
		"%{prj.location}/src/**.cpp",
		"%{prj.name}/vendor/stb/**.h",
		"%{prj.name}/vendor/stb/**.cpp",
		"%{prj.location}/**.vert",
		"%{prj.location}/**.frag",
		"%{prj.location}/**.tesc",
		"%{prj.location}/**.tese",
		"%{prj.location}/**.geom",
		"%{prj.location}/**.comp"
	}

	defines
	{
		"SF_CORE",
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImgGuiExamples}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.portableSnippets}",
		"%{IncludeDir.tinyobjloader}"
	}
	
	includeVulkanSDKWin32()
	linkVulkanStaticWin32()
	
	libdirs
	{
		"%{wks.location}/Silfur/vendor/SDL2/lib"
	}
	
	links
	{
		"SDL2",
		"SDL2main",
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"
		defines "_WIN32"
		
	filter "action:vs2019"
		disablewarnings
		{
			"6323", -- https://docs.microsoft.com/en-US/cpp/code-quality/c6323?view=vs-2019
			"6385", -- https://docs.microsoft.com/en-US/cpp/code-quality/c6385?view=vs-2019
			"6387", -- https://docs.microsoft.com/en-US/cpp/code-quality/c6387?view=vs-2019
			"26812", -- https://docs.microsoft.com/en-US/cpp/code-quality/c26812?view=vs-2019
			"26495", -- https://docs.microsoft.com/en-US/cpp/code-quality/c26495?view=vs-2019
			"26451" -- https://docs.microsoft.com/en-US/cpp/code-quality/c26451?view=vs-2019
		}

	filter "configurations:Debug"
		defines "SF_CONFIG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines
		{
			"SF_NDEBUG",
			"_NDEBUG"
		}
		runtime "Release"
		optimize "Full"

	filter "configurations:Development"
		defines "SF_CONFIG_DEBUG"
		runtime "Debug"
		optimize "Debug"

project "Sandbox"
	location "examples/Sandbox"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.location}/src/**.hpp",
		"%{prj.location}/src/**.inl",
		"%{prj.location}/src/**.cpp",
		"%{prj.location}/**.vert",
		"%{prj.location}/**.frag",
		"%{prj.location}/**.tesc",
		"%{prj.location}/**.tese",
		"%{prj.location}/**.geom",
		"%{prj.location}/**.comp"
	}

	includedirs
	{
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.portableSnippets}",
		"%{IncludeDir.tinyobjloader}",
		"%{wks.location}/Silfur/src"
	}
	
	includeVulkanSDKWin32()

	links
	{
		"Silfur",
		"CompileShaders"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		kind "ConsoleApp"
		defines "_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Development"
		kind "ConsoleApp"
		defines "_DEBUG"
		runtime "Debug"
		optimize "Debug"

	filter "configurations:Release"
		kind "WindowedApp"
		defines "_NDEBUG"
		runtime "Release"
		optimize "Full"