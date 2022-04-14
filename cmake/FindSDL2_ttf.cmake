find_package(SDL2_ttf QUIET)

if (NOT SDL2_IMAGE_FOUND)
    message(STATUS "cmake can't find SDL2_ttf, use pkg-config")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2_ttf sdl2_ttf REQUIRED IMPORTED_TARGET)
    add_library(SDL2::SDL2_ttf ALIAS PkgConfig::SDL2_ttf)
else()
    add_library(SDL2::SDL2_image INTERFACE)
    target_link_libraries(
        SDL2::SDL2_image
        INTERFACE ${SDL_IMAGE_LIBRARIES})
    target_link_libraries(
        SDL2::SDL2_image
        INTERFACE ${SDL_IMAGE_INCLUDE_DIRS})
endif()
