workspace "TrainSim"
    configurations { "Debug", "Release" }
    language "C++"
    cppdialect "C++17"

    location ("projects/" .. os.host() .. "/" .. _ACTION)

    platforms {"x86_64", "x86"}

    startproject "TrainSimGame"

    filter {"configurations:Debug"}
        defines { "DEBUG", "_DEBUG" }
        symbols "On"
        runtime "Debug"

    filter {"configurations:Release"}
        defines { "NDEBUG" }
        runtime "Release"

    project "TrainSimEngine"
        kind "SharedLib"

        includedirs {
            "src-engine",
            "shared-include",
            "external/dxtcompress",
            "external/glad/include",
            "external/glm_include"
        }

        files {
            "shared-include/**.h",
            "shared-include/**.hpp",
            "src-engine/**.h", 
            "src-engine/**.hpp", 
            "src-engine/**.cpp",
            "src-engine/**.c"
        }

        links { "glfw3" }

        filter { "architecture:x86" }
            targetdir "out/x86/%{cfg.buildcfg}"
            includedirs { "external/glfw-3.3.8.bin.WIN32/include" }
            
            libdirs { "external/glfw-3.3.8.bin.WIN32/lib-" .. string.gsub(_ACTION, "vs", "vc") }

        filter { "architecture:x86_64" }
            targetdir "out/x86_64/%{cfg.buildcfg}"
            includedirs { "external/glfw-3.3.8.bin.WIN64/include" }

            libdirs { "external/glfw-3.3.8.bin.WIN64/lib-" .. string.gsub(_ACTION, "vs", "vc") }
            
    project "TrainSimGame"
        kind "ConsoleApp"

        includedirs {
            "src-game",
            "shared-include"
        }

        files {
            "shared-include/**.h",
            "shared-include/**.hpp",
            "src-game/**.h", 
            "src-game/**.hpp", 
            "src-game/**.cpp" 
        }

        links { "glfw3", "TrainSimEngine" }
        runpathdirs { "./", "./bin" }

        filter { "architecture:x86" }
            targetdir "out/x86/%{cfg.buildcfg}"
            includedirs { "external/glfw-3.3.8.bin.WIN32/include" }
            libdirs { "external/glfw-3.3.8.bin.WIN32/lib-" .. string.gsub(_ACTION, "vs", "vc") }

        filter { "architecture:x86_64" }
            targetdir "out/x86_64/%{cfg.buildcfg}"
            includedirs { "external/glfw-3.3.8.bin.WIN64/include" }
            libdirs { "external/glfw-3.3.8.bin.WIN64/lib-" .. string.gsub(_ACTION, "vs", "vc") }
