if (NOT TARGET SDL2_image)
    if (WIN32)  # Windows
        if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC" OR
            (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND
             CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "MSVC")) # use MSVC
            if (NOT SDL2_IMAGE_ROOT)
                set(SDL2_IMAGE_ROOT "" CACHE PATH "SDL2_image root directory")
            endif()
            set(SDL2_IMAGE_LIB_DIR "${SDL2_IMAGE_ROOT}/lib/x64")
            set(SDL2_IMAGE_INCLUDE_DIR "${SDL2_IMAGE_ROOT}/include")
            set(SDL2_IMAGE_DYNAMIC_LIB_DIR "${SDL2_IMAGE_ROOT}/lib/x64" CACHE PATH "SDL2_image directory" FORCE)
            add_library(SDL2::IMAGE SHARED IMPORTED GLOBAL)
            set_target_properties(
                SDL2::IMAGE
                PROPERTIES
                    IMPORTED_LOCATION "${SDL2_IMAGE_LIB_DIR}/SDL2_image.dll"
                    IMPORTED_IMPLIB "${SDL2_IMAGE_LIB_DIR}/SDL2_image.lib"
                    INTERFACE_INCLUDE_DIRECTORIES ${SDL2_IMAGE_INCLUDE_DIR}
            )
            add_library(SDL2_image INTERFACE IMPORTED GLOBAL)
            target_link_libraries(SDL2_image INTERFACE SDL2::IMAGE)
        elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR
                (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND
                 CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "GNU")) # use MINGW
            if (NOT SDL2_IMAGE_ROOT)
                set(SDL2_IMAGE_ROOT "" CACHE PATH "SDL2_image root directory")
            endif()
            set(SDL2_IMAGE_STATIC_LIB_DIR "")
            set(SDL2_IMAGE_INCLUDE_DIR "")

            if(CMAKE_SIZEOF_VOID_P EQUAL 8)
                set(SDL2_IMAGE_INCLUDE_DIR "${SDL2_IMAGE_ROOT}/x86_64-w64-mingw32/include")
                set(SDL2_IMAGE_STATIC_LIB_DIR "${SDL2_IMAGE_ROOT}/x86_64-w64-mingw32/lib")
                set(SDL2_IMAGE_DYNAMIC_LIB_DIR "${SDL2_IMAGE_ROOT}/x86_64-w64-mingw32/bin" CACHE PATH "SDL2_image directory" FORCE)
            else()
                set(SDL2_IMAGE_INCLUDE_DIR "${SDL2_IMAGE_ROOT}/i686-w64-mingw32/include")
                set(SDL2_IMAGE_STATIC_LIB_DIR "${SDL2_IMAGE_ROOT}/i686-w64-mingw32/lib")
                set(SDL2_IMAGE_DYNAMIC_LIB_DIR "${SDL2_IMAGE_ROOT}/i686-w64-mingw32/bin" CACHE PATH "SDL2_image directory" FORCE)
            endif()

            add_library(SDL2::IMAGE SHARED IMPORTED GLOBAL)
            set_target_properties(
                SDL2::IMAGE
                PROPERTIES
                    IMPORTED_LOCATION "${SDL2_IMAGE_DYNAMIC_LIB_DIR}/SDL2_image.dll"
                    IMPORTED_IMPLIB "${SDL2_IMAGE_STATIC_LIB_DIR}/libSDL2_image.a"
                    INTERFACE_INCLUDE_DIRECTORIES ${SDL2_IMAGE_INCLUDE_DIR}
            )
            add_library(SDL2_image INTERFACE IMPORTED GLOBAL)
            target_link_libraries(SDL2_image INTERFACE SDL2::IMAGE)

        endif()
        mark_as_advanced(SDL2_IMAGE_DYNAMIC_LIB_DIR)
    else()  # Linux, MacOSX
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2_image SDL2_image REQUIRED IMPORTED_TARGET)
        add_library(SDL2_image ALIAS PkgConfig::SDL2_image)
    endif()
endif()