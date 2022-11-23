if (NOT TARGET SDL2_mixer)
    if (WIN32)  # Windows, use clang or MSVC
        if (NOT SDL2_MIXER_ROOT)
            set(SDL2_MIXER_ROOT "" CACHE PATH "SDL2_mixer root directory")
        endif()
        set(SDL2_MIXER_LIB_DIR "${SDL2_MIXER_ROOT}/lib/x64")
        set(SDL2_MIXER_INCLUDE_DIR "${SDL2_MIXER_ROOT}/include")
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
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2_mixer SDL2_mixer REQUIRED IMPORTED_TARGET)
        add_library(SDL2_mixer ALIAS PkgConfig::SDL2_mixer)
    endif()
endif()
