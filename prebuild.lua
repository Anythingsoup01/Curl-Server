workspace "Video-Scraper-Server"

project "Server"
    kind "ConsoleApp"
    language "C++"
    dialect "20"

    files {
        "Server/src/*.cpp",
        "Server/src/*.h"
    }

    includedirs {
        "Server/src",
        "Network/src"
    }

    links {
    }

project "Client"
    kind "ConsoleApp"
    language "C++"
    dialect "20"

    files {
        "Client/src/*.cpp",
        "Client/src/*.h"
    }

    includedirs {
        "Client/src",
        "Network/src",
    }

    links {
    }

