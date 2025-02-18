local ProjectName = "ExpAll"
local SolutionDir = "./"
local ProjectDir = SolutionDir .. "src"

workspace (ProjectName)

    configurations { "Debug", "Release"  }
    platforms      { "x64" }
    location       (SolutionDir)

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter {}

    filter "configurations:Release"
        defines  { "NDEBUG" }
        optimize "On"
        linkoptions { "/ENTRY:mainCRTStartup" }
    filter {}

    filter "platforms:x64"
        architecture "x64"
    filter {}

project (ProjectName)

    targetname "expall"
    
    filter "configurations:Debug"
        kind "ConsoleApp"
    filter {}

    filter "configurations:Release"
        kind "WindowedApp"   
    filter {}
    
    language   "C++"
    cppdialect "C++20"
    targetdir  "$(SolutionDir)bin/$(Configuration)-$(Platform)/$(ProjectName)"
    objdir     "!$(SolutionDir)bin-int/$(Configuration)-$(Platform)/$(ProjectName)"
    location   (ProjectDir)

    includedirs { "$(SolutionDir)/_Raylib/include" }
    libdirs     "$(SolutionDir)/_Raylib/lib"
    links       { "raylib", "winmm" }

    files { "src/*.c", "src/*.cpp", "src/*.h" }