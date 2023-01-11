if (NOT TARGET SDL2_ttf)
    if (WIN32)  # Windows
        if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC" OR # use MSVC
            (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND
             CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "MSVC"))
            if (NOT SDL2_TTF_ROOT)
                set(SDL2_TTF_ROOT "" CACHE PATH "SDL2_ttf root directory")
            endif()
            set(SDL2_TTF_LIB_DIR "${SDL2_TTF_ROOT}/lib/x64")
            set(SDL2_TTF_DYNAMIC_LIB_DIR "${SDL2_TTF_ROOT}/lib/x64" CACHE PATH "SDL2_ttf.dll directory")
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
        elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR
                (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND
                 CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "GNU")) # use MINGW
            if (NOT SDL2_TTF_ROOT)
                set(SDL2_TTF_ROOT "" CACHE PATH "SDL2_ttf root directory")
            endif()
            set(SDL2_TTF_STATIC_LIB_DIR "${SDL2_TTF_ROOT}/x86_64-w64-mingw32/lib")
            set(SDL2_TTF_DYNAMIC_LIB_DIR "${SDL2_TTF_ROOT}/x86_64-w64-mingw32/bin" CACHE PATH "SDL2_ttf.dll directory")
            set(SDL2_TTF_INCLUDE_DIR "${SDL2_TTF_ROOT}/x86_64-w64-mingw32/include/SDL2")
            add_library(SDL2::TTF SHARED IMPORTED GLOBAL)
            set_target_properties(
                SDL2::TTF
                PROPERTIES
                    IMPORTED_LOCATION "${SDL2_TTF_DYNAMIC_LIB_DIR}/SDL2_ttf.dll"
                    IMPORTED_IMPLIB "${SDL2_TTF_STATIC_LIB_DIR}/libSDL2_ttf.a"
                    INTERFACE_INCLUDE_DIRECTORIES ${SDL2_TTF_INCLUDE_DIR}
            )
            add_library(SDL2_ttf INTERFACE IMPORTED GLOBAL)
            target_link_libraries(SDL2_ttf INTERFACE SDL2::TTF)
        else()
            message(FATAL_ERROR "your compiler don't support, please use MSVC, Clang++ or MinGW")
        endif()
    else()  # Linux, MacOSX
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2_ttf SDL2_ttf REQUIRED IMPORTED_TARGET)
        add_library(SDL2_ttf ALIAS PkgConfig::SDL2_ttf)
    endif()
endif()