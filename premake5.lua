workspace "TrainSim"
    configurations { "Debug", "Release" }
    language "C++"

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
        targetdir "out/%{cfg.buildcfg}"

        includedirs {
            "src-engine",
            "shared-include"
        }

        files {
            "shared-include/**.h",
            "shared-include/**.hpp",
            "src-engine/**.h", 
            "src-engine/**.hpp", 
            "src-engine/**.cpp" 
        }

    project "TrainSimGame"
        kind "ConsoleApp"
        targetdir "out/%{cfg.buildcfg}"

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

        links { "TrainSimEngine" }
