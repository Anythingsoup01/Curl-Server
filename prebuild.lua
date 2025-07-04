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
        "Network"
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
        "Network"
    }

project "Network"
    kind "StaticLib"
    language "C++"
    dialect "20"

    files {
        "Network/src/*.cpp",
        "Network/src/*.h"
    }

    includedirs {
        "Network/src",
    }
