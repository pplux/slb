-- Copyright 2012 Jose L. Hidalgo <pplux@pplux.com>
-- Script to generate SLB solution/makefile/...
-- Used manily for SLB's development and testing

config = {
    location = "build/",
    target = "build/bin/",
    debug_level = 5,
}

if _ACTION == "test" then
    print("RUNNING TESTS...........")
    local count, failed = 0,0
    for k,v in pairs(os.matchfiles("tests/scripts/*.lua")) do
        count = count +1
        print("\n")
        print("_______________________________________________________________")
        print("***************************************************************")
        print("** Running Test: ("..k..")",v)
        print("***************************************************************")
        if os.execute("build/bin/SLB-test "..v) ~= 0 then 
            print("***************************************************************")
            print("** FAIL:", v)
            print("***************************************************************")
            print("_______________________________________________________________")
            failed = failed +1
        else
            print("***************************************************************")
            print("** OK:", v)
            print("***************************************************************")
            print("_______________________________________________________________")
        end 
    end
    print("RESULTS: total tests:",count," failed: ", failed)
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

project "SLB-static-verbose"
    targetname("SLB")
    targetdir(config.target.."static/")
    defines { "SLB_LIBRARY", "SLB_DYNAMIC_LIBRARY=0" }
    kind "StaticLib" 
    defines { "SLB_DEBUG_LEVEL="..config.debug_level }
    files { "src/*.c*" , "include/SLB/*.hpp" }


project "SLB-test"
    kind "ConsoleApp"
    files { "tests/src/*.cpp", "tests/src/*.h" }
    defines { "SLB_DEBUG_LEVEL="..config.debug_level }
    links "SLB-static-verbose"

-- examples
do
    for _, example in pairs(os.matchdirs("examples/*")) do
        project(path.getname(example))
        kind "ConsoleApp"
        files { example.."/**.cpp", example.."/**.h" }
        links "SLB-static"
    end
end




 
