set(SDL2_MIXER_ROOT "" CACHE PATH "SDL2_mixer root directory")
set(SDL2_MIXER_LIB_DIR "${SDL2_MIXER_ROOT}/lib/x64")
set(SDL2_MIXER_INCLUDE_DIR "${SDL2_MIXER_ROOT}/include")

if (NOT TARGET SDL2_mixer)
    if (WIN32)  # Windows, use clang or MSVC
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
    else()  # Linux, MacOSX
        # TODO not finish
        message(FATAL_ERROR "don't implement FindSDL_mixer under Unix")
    endif()
endif()
