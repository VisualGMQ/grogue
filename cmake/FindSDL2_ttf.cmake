find_package(SDL2_ttf QUIET)

if (NOT SDL2_TTF_FOUND)
    if (WIN32) # find SDL2_ttf on Windows
        # TODO find SDL2_ttf on Windows
    else()  # find SDL2_ttf on Linux and MacOSX
        message(STATUS "cmake can't find SDL2_ttf, use pkg-config")
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2_ttf sdl2_ttf REQUIRED IMPORTED_TARGET)
        add_library(SDL2::SDL2_ttf ALIAS PkgConfig::SDL2_ttf)
    endif()
else()
    add_library(SDL2::SDL2_ttf INTERFACE)
    target_link_libraries(
        SDL2::SDL2_ttf
        INTERFACE ${SDL_TTF_LIBRARIES})
    target_link_libraries(
        SDL2::SDL2_ttf
        INTERFACE ${SDL_TTF_INCLUDE_DIRS})
endif()
