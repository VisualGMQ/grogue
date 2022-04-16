find_package(SDL2_ttf QUIET)

if (NOT SDL2_TTF_FOUND)
    if (WIN32) # find SDL2_ttf on Windows
        find_path(SDL2TTF_HEADER_PATH
              SDL_ttf.h
              PATHS 3rdlib/SDL2_ttf/include/SDL2 REQUIRED)
        message(STATUS "found SDL2_ttf include dir ${SDL2TTF_HEADER_PATH}")

        find_path(SDL2TTF_LIB_PATH
                  libSDL2_ttf.a
                  PATHS 3rdlib/SDL2_ttf/lib REQUIRED)
        message(STATUS "found SDL2_ttf link dir ${SDL2TTF_LIB_PATH}")

        add_library(SDL2::SDL2_ttf INTERFACE IMPORTED)
        if (NOT MSVC) # if is mingw
            set_target_properties(
                SDL2::SDL2_ttf
                PROPERTIES
                    INTERFACE_INCLUDE_DIRECTORIES "${SDL2TTF_HEADER_PATH}"
                    INTERFACE_LINK_LIBRARIES "-L${SDL2TTF_LIB_PATH} -lSDL2_ttf"
                    IMPORTED_LINK_INTERFACE_LANGUAGES "C"
            )
            file(
                COPY 3rdlib/SDL2_ttf/bin/SDL2_ttf.dll
                DESTINATION ${CMAKE_BINARY_DIR}
            )
        else() # compile under MSVC
        endif()
    else()  # find SDL2_ttf on Linux and MacOSX
        message(STATUS "cmake can't find SDL2_ttf, use pkg-config")
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2_ttf sdl2_ttf REQUIRED IMPORTED_TARGET)
        add_library(SDL2::SDL2_ttf ALIAS PkgConfig::SDL2_ttf)
    endif()
else()
    add_library(SDL2::SDL2_ttf INTERFACE)
    set_target_properties(
        SDL2::SDL2_ttf
        PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${SDL2_TTF_INCLUDE_DIRS}"
            INTERFACE_LINK_LIBRARIES "${SDL2_TTF_LIBRARIES}"
            IMPORTED_LINK_INTERFACE_LANGUAGES "C"
    )
endif()
