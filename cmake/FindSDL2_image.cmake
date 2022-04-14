find_package(SDL2_image QUIET)

if (NOT SDL2_IMAGE_FOUND)
    if (WIN32) # find SDL2_image on Windows
        # TODO find SDL2 on Windows
    else() # find SDL2 on Linux and MacOSX
        message(STATUS "cmake can't find SDL2_image, use pkg-config")
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2_image sdl2_image REQUIRED IMPORTED_TARGET)
        add_library(SDL2::SDL2_image ALIAS PkgConfig::SDL2_image)
    endif()
else()
    add_library(SDL2::SDL2_image INTERFACE)
    target_link_libraries(
        SDL2::SDL2_image
        INTERFACE ${SDL2_IMAGE_LIBRARIES})
    target_link_libraries(
        SDL2::SDL2_image
        INTERFACE ${SDL2_IMAGE_INCLUDE_DIRS})
endif()
