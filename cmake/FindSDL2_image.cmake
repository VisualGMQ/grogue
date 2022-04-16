find_package(SDL2_image QUIET)

if (NOT SDL2_IMAGE_FOUND)
    if (WIN32) # find SDL2_image on Windows
        find_path(SDL2IMG_HEADER_PATH
              SDL_image.h
              PATHS 3rdlib/SDL2_image/include/SDL2 REQUIRED)
        message(STATUS "found SDL2_image include dir ${SDL2IMG_HEADER_PATH}")

        find_path(SDL2IMG_LIB_PATH
                  libSDL2_image.a
                  PATHS 3rdlib/SDL2_image/lib REQUIRED)
        message(STATUS "found SDL2_image link dir ${SDL2IMG_LIB_PATH}")

        add_library(SDL2::SDL2_image INTERFACE IMPORTED)
        if (NOT MSVC) # if is mingw
            set_target_properties(
                SDL2::SDL2_image
                PROPERTIES
                    INTERFACE_INCLUDE_DIRECTORIES "${SDL2IMG_HEADER_PATH}"
                    INTERFACE_LINK_LIBRARIES "-L${SDL2IMG_LIB_PATH} -lSDL2_image"
                    IMPORTED_LINK_INTERFACE_LANGUAGES "C"
            )
            file(
                COPY
                    3rdlib/SDL2_image/bin/SDL2_image.dll
                    3rdlib/SDL2_image/bin/libjpeg-9.dll
                    3rdlib/SDL2_image/bin/libpng16-16.dll
                    3rdlib/SDL2_image/bin/libtiff-5.dll
                    3rdlib/SDL2_image/bin/libwebp-7.dll
                    3rdlib/SDL2_image/bin/zlib1.dll
                DESTINATION ${CMAKE_BINARY_DIR}
            )
        else() # TODO compile under MSVC
        endif()
    else() # find SDL2 on Linux and MacOSX
        message(STATUS "cmake can't find SDL2_image, use pkg-config")
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2_image sdl2_image REQUIRED IMPORTED_TARGET)
        add_library(SDL2::SDL2_image ALIAS PkgConfig::SDL2_image)
    endif()
else()
    add_library(SDL2::SDL2_image INTERFACE IMPORTED)
    set_target_properties(
        SDL2::SDL2_image
        PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${SDL2_IMAGE_INCLUDE_DIRS}"
            INTERFACE_LINK_LIBRARIES "${SDL2_IMAGE_LIBRARIES}"
            IMPORTED_LINK_INTERFACE_LANGUAGES "C"
    )
endif()
