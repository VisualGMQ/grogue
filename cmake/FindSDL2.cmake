find_package(SDL2 QUIET)

if (NOT SDL2_FOUND)
    if (WIN32)  # find SDL2 on Windows
        # TODO find SDL2 on Windows
    else()      # find SDL2 on Linux and MacOSX
        message(STATUS "cmake can't find SDL2, use pkg-config")
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2 sdl2 REQUIRED IMPORTED_TARGET)
        add_library(SDL2::SDL2 ALIAS PkgConfig::SDL2)
    endif()
endif()
