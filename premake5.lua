-- Solution Chorium
workspace "Chorium"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    location "./"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter {}

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        linkoptions { "/ENTRY:mainCRTStartup" }
    filter {}

    filter "platforms:x64"
        architecture "x64"
    filter {}

    startproject "Examples"

-- Project Chorium
project "Chorium"
    filter "configurations:Debug"
        libdirs { "./_SDL3/lib/Debug-x64" }
        kind "StaticLib"
        filter {}
        
    filter "configurations:Release"
        libdirs { "./_SDL3/lib/Release-x64" }
        kind "StaticLib"
    filter {}
    
    language "C++"
    cppdialect "C++20"
    targetdir "./_Chorium/lib/$(Configuration)-$(Platform)"
    objdir "!./bin-int/$(Configuration)-$(Platform)/$(ProjectName)"
    location "./src/Chorium"

    includedirs { "./_SDL3/include" }
    
    links { 
        "SDL3-static",
        "SDL3_test",
        "winmm",
        "imm32",
        "version",
        "setupapi",
        "shell32",
        "user32",
        "gdi32",
        "advapi32"
    }

    files {
        "./src/Chorium/**.c",
        "./src/Chorium/**.cpp",
        "./src/Chorium/**.h",
        "./src/Chorium/**.hpp",
    }
    
    targetname "chorium"

-- Project Examples
project "Examples"

    filter "configurations:Debug"
        libdirs { "./_SDL3/lib/Debug-x64" }
        kind "ConsoleApp"
        filter {}
        
    filter "configurations:Release"
        libdirs { "./_SDL3/lib/Release-x64" }
        kind "WindowedApp"
    filter {}
    
    language "C++"
    cppdialect "C++20"
    targetdir "./bin/$(Configuration)-$(Platform)/$(ProjectName)"
    objdir "!./bin-int/$(Configuration)-$(Platform)/$(ProjectName)"
    location "./src/Examples"

    includedirs { "./_SDL3/include", "./src/Chorium" }
    
    links { 
        "chorium",
        "SDL3-static",
        "SDL3_test",
        "winmm",
        "imm32",
        "version",
        "setupapi",
        "shell32",
        "user32",
        "gdi32",
        "advapi32"
    }

    files {
        "./src/Examples/**.c",
        "./src/Examples/**.cpp",
        "./src/Examples/**.h",
        "./src/Examples/**.hpp",
    }
    
    targetname "examples"

    dependson { "Chorium" }