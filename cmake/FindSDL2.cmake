find_package(SDL2 QUIET)

if (NOT SDL2_FOUND)
    if (WIN32)  # find SDL2 on Windows
    	find_path(SDL2_HEADER_PATH
                  SDL.h
                  PATHS 3rdlib/SDL2/include/SDL2 REQUIRED)
        message(STATUS "found SDL2 include dir ${SDL2_HEADER_PATH}")

        find_path(SDL2_LIB_PATH
                  libSDL2.a
                  PATHS 3rdlib/SDL2/lib REQUIRED)
        message(STATUS "found SDL2 link dir ${SDL2_LIB_PATH}")

        add_library(SDL2::SDL2 INTERFACE IMPORTED)

        if (NOT MSVC) # if is mingw
            set_target_properties(
                SDL2::SDL2
                PROPERTIES
                    INTERFACE_INCLUDE_DIRECTORIES "${SDL2_HEADER_PATH}"
                    INTERFACE_LINK_LIBRARIES "-L${SDL2_LIB_PATH} -lmingw32 -lSDL2main -lSDL2 -mwindows"
                    IMPORTED_LINK_INTERFACE_LANGUAGES "C"
            )
            file(
                COPY 3rdlib/SDL2/bin/SDL2.dll
                DESTINATION ${CMAKE_BINARY_DIR}
            )
        else() # TODO compile under MSVC
        endif()
    else()      # find SDL2 on Linux and MacOSX
        message(STATUS "cmake can't find SDL2, use pkg-config")
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2 sdl2 REQUIRED IMPORTED_TARGET)
        add_library(SDL2::SDL2 ALIAS PkgConfig::SDL2)
    endif()
endif()
