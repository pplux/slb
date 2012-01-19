-- Copyright 2012 Jose L. Hidalgo <pplux@pplux.com>
-- Script to generate SLB solution/makefile/...
-- Used manily for SLB's development and testing

config = {
    location = "build/",
    target = "build/bin/",
    debug_level = 0,
}

if _ACTION == "test" then
    os.exit(0)
end

solution "SLB"
    language "c++"
    configurations {"Debug", "Release"}
    includedirs("include")
    location(config.location)
    targetdir(config.target)
    configuration "Debug"
        flags { "Symbols" }
        defines { "SLB_DEBUGL_LEVEL="..config.debug_level }
    configuration "Release"
        flags { "Optimize" }

project "SLB-static"
    targetname("SLB")
    targetdir(config.target.."static/")
    defines { "SLB_LIBRARY", "SLB_DYNAMIC_LIBRARY=0" }
    kind "StaticLib" 
    files { "src/*.c*" , "include/SLB/*.hpp" }

project "SLB-dynamic"
    targetname("SLB")
    targetdir(config.target.."dynamic/")
    defines { "SLB_LIBRARY", "SLB_DYNAMIC_LIBRARY=1" }
    kind "SharedLib" 
    files { "src/*.c*" , "include/SLB/*.hpp" }

-- examples
do
    for _, example in pairs(os.matchdirs("examples/*")) do
        project(path.getname(example))
        kind "ConsoleApp"
        files { example.."/**.cpp", example.."/**.h" }
        links "SLB-static"
    end
end



 
