if (NOT TARGET SDL2_ttf)
    if (WIN32)  # Windows, use clang or MSVC
        set(SDL2_TTF_ROOT "" CACHE PATH "SDL2_ttf root directory")
        set(SDL2_TTF_LIB_DIR "${SDL2_TTF_ROOT}/lib/x64")
        set(SDL2_TTF_INCLUDE_DIR "${SDL2_TTF_ROOT}/include")
        add_library(SDL2::TTF SHARED IMPORTED GLOBAL)
        set_target_properties(
            SDL2::TTF
            PROPERTIES
                IMPORTED_LOCATION "${SDL2_TTF_LIB_DIR}/SDL2_ttf.dll"
                IMPORTED_IMPLIB "${SDL2_TTF_LIB_DIR}/SDL2_ttf.lib"
                INTERFACE_INCLUDE_DIRECTORIES ${SDL2_TTF_INCLUDE_DIR}
        )
        add_library(SDL2_ttf INTERFACE IMPORTED GLOBAL)
        target_link_libraries(SDL2_ttf INTERFACE SDL2::TTF)
    else()  # Linux, MacOSX
        # TODO not finish
        message(FATAL_ERROR "don't implement FindSDL_ttf under Unix")
    endif()
endif()
