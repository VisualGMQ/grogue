if (NOT TARGET SDL2_mixer)
    if (WIN32)  # Windows
        if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC" OR
            (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND
             CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "MSVC")) # use MSVC
            if (NOT SDL2_MIXER_ROOT)
                set(SDL2_MIXER_ROOT "" CACHE PATH "SDL2_mixer root directory")
            endif()
            set(SDL2_MIXER_LIB_DIR "${SDL2_MIXER_ROOT}/lib/x64")
            set(SDL2_MIXER_INCLUDE_DIR "${SDL2_MIXER_ROOT}/include")
            set(SDL2_MIXER_DYNAMIC_LIB_DIR "${SDL2_MIXER_ROOT}/lib/x64" CACHE PATH "SDL2_mixer directory" FORCE)
            add_library(SDL2::MIXER SHARED IMPORTED GLOBAL)
            set_target_properties(
                SDL2::MIXER
                PROPERTIES
                    IMPORTED_LOCATION "${SDL2_MIXER_LIB_DIR}/SDL2_mixer.dll"
                    IMPORTED_IMPLIB "${SDL2_MIXER_LIB_DIR}/SDL2_mixer.lib"
                    INTERFACE_INCLUDE_DIRECTORIES ${SDL2_MIXER_INCLUDE_DIR}
            )
            add_library(SDL2_mixer INTERFACE IMPORTED GLOBAL)
            target_link_libraries(SDL2_mixer INTERFACE SDL2::MIXER)
        elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR
                (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND
                 CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "GNU")) # use MINGW
            if (NOT SDL2_MIXER_ROOT)
                set(SDL2_MIXER_ROOT "" CACHE PATH "SDL2_mixer root directory")
            endif()
            set(SDL2_MIXER_STATIC_LIB_DIR "${SDL2_MIXER_ROOT}/x86_64-w64-mingw32/lib")
            set(SDL2_MIXER_DYNAMIC_LIB_DIR "${SDL2_MIXER_ROOT}/x86_64-w64-mingw32/bin" CACHE PATH "SDL2_mixer directory" FORCE)
            set(SDL2_MIXER_INCLUDE_DIR "${SDL2_MIXER_ROOT}/x86_64-w64-mingw32/include/SDL2")
            add_library(SDL2::MIXER SHARED IMPORTED GLOBAL)
            set_target_properties(
                SDL2::MIXER
                PROPERTIES
                    IMPORTED_LOCATION "${SDL2_MIXER_DYNAMIC_LIB_DIR}/SDL2_mixer.dll"
                    IMPORTED_IMPLIB "${SDL2_MIXER_STATIC_LIB_DIR}/libSDL2_mixer.a"
                    INTERFACE_INCLUDE_DIRECTORIES ${SDL2_MIXER_INCLUDE_DIR}
            )
            add_library(SDL2_mixer INTERFACE IMPORTED GLOBAL)
            target_link_libraries(SDL2_mixer INTERFACE SDL2::MIXER)
        else()
            message(FATAL_ERROR "your compiler don't support, please use MSVC, Clang++ or MinGW")
        endif()
        mark_as_advanced(SDL2_MIXER_DYNAMIC_LIB_DIR)
    else()  # Linux, MacOSX
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2_mixer SDL2_mixer REQUIRED IMPORTED_TARGET)
        add_library(SDL2_mixer ALIAS PkgConfig::SDL2_mixer)
    endif()
endif()