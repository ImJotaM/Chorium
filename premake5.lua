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
    filter {}

    filter "platforms:x64"
        architecture "x64"
    filter {}

project (ProjectName)

    targetname "expall"
    kind       "ConsoleApp"
    language   "C"
    targetdir  "$(SolutionDir)bin/$(Configuration)-$(Platform)/$(ProjectName)"
    objdir     "!$(SolutionDir)bin-int/$(Configuration)-$(Platform)/$(ProjectName)"
    location   (ProjectDir)

    includedirs { "$(SolutionDir)/_Raylib/include" }
    libdirs     "$(SolutionDir)/_Raylib/lib"
    links       { "raylib", "winmm" }

    files { "src/*.c", "src/*.h" }