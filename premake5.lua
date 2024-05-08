workspace "TrainSim"
    configurations { "Debug", "Release" }
    language "C++"
    cppdialect "C++17"

    location ("projects/" .. os.host() .. "/" .. _ACTION)

    platforms {"x86_64", "x86"}

    startproject "TrainSimGame"
    
    defines { 
        "_CRT_SECURE_NO_WARNINGS", 
        "_CRT_NON_CONFORMING_SWPRINTFS",
        "_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING",
        "GLFW_INCLUDE_NONE",
        "GLM_FORCE_DEFAULT_ALIGNED_GENTYPES",
        "GLM_FORCE_LEFT_HANDED",
        "GLM_FORCE_AVX",
        "CURL_STATICLIB"
    }

    disablewarnings { "4267", "4244", "4305", "4005", "5082" }

    filter {"system:windows"}
        buildoptions { "/utf-8" }
        flags { "MultiProcessorCompile" }

    filter {"configurations:Debug"}
        defines { "DEBUG", "_DEBUG" }
        symbols "On"
        runtime "Debug"

    filter {"configurations:Release"}
        defines { "NDEBUG" }
        runtime "Release"
        optimize "Speed"

    project "TrainSimEngine"
        kind "SharedLib"

        includedirs {
            "src-engine",
            "shared-include",
            "external/stb_image",
            "external/dxtcompress",
            "external/glad/include",
            "external/glm-include",
            "external/imgui-master",
            "external/imgui-master/backends",
            "external/ultralight/include",
            "external/json",
            "external/spsc_queue/include",
            "external/mpmc_queue/include"
            -- "external/curl/include",
        }

        files {
            "external/imgui-master/cpp/**.cpp",
            "shared-include/**.h",
            "shared-include/**.hpp",
            "src-engine/**.h", 
            "src-engine/**.hpp", 
            "src-engine/**.cpp",
            "src-engine/**.c"
        }

        nuget { 
            "curl-vc140-static-64:7.52.1",
            "openssl-vc140-static-64:1.1.0"
        }

        links { "glfw3" }

        filter { "architecture:x86" }
            targetdir "out/x86/%{cfg.buildcfg}"
            includedirs { "external/glfw-3.3.8.bin.WIN32/include" }
            
            libdirs { 
                "external/glfw-3.3.8.bin.WIN32/lib-" .. string.gsub(_ACTION, "vs", "vc")
            }

        filter { "architecture:x86_64" }
            targetdir "out/x86_64/%{cfg.buildcfg}"
            includedirs { "external/glfw-3.3.8.bin.WIN64/include" }

            links { 
                "AppCore", 
                "Ultralight", 
                "UltralightCore", 
                "WebCore",

                -- "libbrotlicommon",
                -- "libbrotlidec",
                -- "libcrypto",
                -- "libcurl",
                -- "libnghttp2",
                -- "libnghttp3",
                -- "libngtcp2",
                -- "libngtcp2_crypto_quictls",
                -- "libpsl",
                -- "libssh2",
                -- "libssl",
                -- "libz",
                -- "libzstd",

                "wldap32",
                "crypt32",
                "Ws2_32",
            }

            libdirs { 
                "external/glfw-3.3.8.bin.WIN64/lib-" .. string.gsub(_ACTION, "vs", "vc"),
                "external/ultralight/lib",
                -- "external/curl/lib"
            }
            
    project "TrainSimGame"
        kind "ConsoleApp"

        includedirs {
            "src-game",
            "shared-include",
            "external/stb_image",
            "external/imgui-master",
            "external/imgui-master/backends",
            "external/ultralight/include",
            "external/glm-include",
            "external/json",
        }

        files {
            "external/imgui-master/cpp/**.cpp",
            "shared-include/**.h",
            "shared-include/**.hpp",
            "src-game/**.h", 
            "src-game/**.hpp", 
            "src-game/**.cpp" 
        }

        links { "glfw3", "TrainSimEngine" }

        filter { "architecture:x86" }
            targetdir "out/x86/%{cfg.buildcfg}"
            debugdir "out/x86/%{cfg.buildcfg}"
            includedirs { "external/glfw-3.3.8.bin.WIN32/include" }
            libdirs { "external/glfw-3.3.8.bin.WIN32/lib-" .. string.gsub(_ACTION, "vs", "vc") }

        filter { "architecture:x86_64" }
            targetdir "out/x86_64/%{cfg.buildcfg}"
            debugdir "out/x86_64/%{cfg.buildcfg}"
            includedirs { "external/glfw-3.3.8.bin.WIN64/include" }
            links { "AppCore", "Ultralight", "UltralightCore", "WebCore" }

            libdirs { 
                "external/glfw-3.3.8.bin.WIN64/lib-" .. string.gsub(_ACTION, "vs", "vc"),
                "external/ultralight/lib"
            }

    project "TrainSimTools"
        kind "ConsoleApp"

        includedirs {
            "src-tools",
            "shared-include",
            "external/stb_image",
            "external/imgui-master",
            "external/imgui-master/backends",
            "external/ultralight/include",
            "external/glm-include",
            "external/json",
            "external/portable-file-dialogs"
        }

        files {
            "external/imgui-master/cpp/**.cpp",
            "shared-include/**.h",
            "shared-include/**.hpp",
            "src-tools/**.h", 
            "src-tools/**.hpp", 
            "src-tools/**.cpp" 
        }

        links { "glfw3", "TrainSimEngine" }

        filter { "architecture:x86" }
            targetdir "out/x86/%{cfg.buildcfg}"
            debugdir "out/x86/%{cfg.buildcfg}"
            includedirs { "external/glfw-3.3.8.bin.WIN32/include" }
            libdirs { "external/glfw-3.3.8.bin.WIN32/lib-" .. string.gsub(_ACTION, "vs", "vc") }

        filter { "architecture:x86_64" }
            targetdir "out/x86_64/%{cfg.buildcfg}"
            debugdir "out/x86_64/%{cfg.buildcfg}"
            includedirs { "external/glfw-3.3.8.bin.WIN64/include" }
            links { "AppCore", "Ultralight", "UltralightCore", "WebCore" }

            libdirs { 
                "external/glfw-3.3.8.bin.WIN64/lib-" .. string.gsub(_ACTION, "vs", "vc"),
                "external/ultralight/lib"
            }

    project "TrainSimExampleTrain"
        kind "SharedLib"
        
        includedirs {
            "src-example-train",
            "shared-include",
            "external/stb_image",
            "external/imgui-master",
            "external/imgui-master/backends",
            "external/glm-include",
            "external/json",
        }

        files {
            "external/imgui-master/cpp/**.cpp",
            "shared-include/**.h",
            "shared-include/**.hpp",
            "src-example-train/**.h", 
            "src-example-train/**.hpp", 
            "src-example-train/**.cpp" 
        }

        links { "glfw3", "TrainSimEngine" }

        filter { "architecture:x86" }
            targetdir "out/x86/%{cfg.buildcfg}"
            debugdir "out/x86/%{cfg.buildcfg}"
            includedirs { "external/glfw-3.3.8.bin.WIN32/include" }
            libdirs { "external/glfw-3.3.8.bin.WIN32/lib-" .. string.gsub(_ACTION, "vs", "vc") }

        filter { "architecture:x86_64" }
            targetdir "out/x86_64/%{cfg.buildcfg}"
            debugdir "out/x86_64/%{cfg.buildcfg}"
            includedirs { "external/glfw-3.3.8.bin.WIN64/include" }
            libdirs { "external/glfw-3.3.8.bin.WIN64/lib-" .. string.gsub(_ACTION, "vs", "vc") }