if (NOT TARGET SDL2_image)
    if (NOT SDL2_IMAGE_ROOT)
        set(SDL2_IMAGE_ROOT "" CACHE PATH "SDL2_image root directory")
    endif()
    if (WIN32)  # Windows
        IsMSVCBackend(is_msvc_backend)
        IsMinGWBackend(is_mingw_backend)
        IsX64Compiler(is_x64_compiler)
        if (${is_msvc_backend}) # use MSVC
            set(INCLUDE_DIR "${SDL2_IMAGE_ROOT}/include")
            if (${is_x64_compiler})
                set(LIB_DIR "${SDL2_IMAGE_ROOT}/lib/x64")
                set(SDL2_IMAGE_DYNAMIC_LIB_DIR "${SDL2_IMAGE_ROOT}/lib/x64" CACHE PATH "SDL2_image directory" FORCE)
            else()
                set(LIB_DIR "${SDL2_IMAGE_ROOT}/lib/x86")
                set(SDL2_IMAGE_DYNAMIC_LIB_DIR "${SDL2_IMAGE_ROOT}/lib/x86" CACHE PATH "SDL2_image directory" FORCE)
            endif()
            set(LIB_PATH "${LIB_DIR}/SDL2_image.lib")
            set(DYNAMIC_LIB_PATH "${SDL2_IMAGE_DYNAMIC_LIB_DIR}/SDL2_image.dll")
        elseif (${is_mingw_backend}) # use MINGW
            if(${is_x64_compiler})
                set(INCLUDE_DIR "${SDL2_IMAGE_ROOT}/x86_64-w64-mingw32/include/SDL2")
                set(LIB_DIR "${SDL2_IMAGE_ROOT}/x86_64-w64-mingw32/lib")
                set(SDL2_IMAGE_DYNAMIC_LIB_DIR "${SDL2_IMAGE_ROOT}/x86_64-w64-mingw32/bin" CACHE PATH "SDL2_image directory" FORCE)
            else()
                set(INCLUDE_DIR "${SDL2_IMAGE_ROOT}/i686-w64-mingw32/include/SDL2")
                set(LIB_DIR "${SDL2_IMAGE_ROOT}/i686-w64-mingw32/lib")
                set(SDL2_IMAGE_DYNAMIC_LIB_DIR "${SDL2_IMAGE_ROOT}/i686-w64-mingw32/bin" CACHE PATH "SDL2_image directory" FORCE)
            endif()
            set(LIB_PATH "${LIB_DIR}/libSDL2_image.a")
            set(DYNAMIC_LIB_PATH "${SDL2_IMAGE_DYNAMIC_LIB_DIR}/SDL2_image.dll")
        endif()
        mark_as_advanced(SDL2_IMAGE_DYNAMIC_LIB_DIR)

        add_library(SDL2::IMAGE SHARED IMPORTED GLOBAL)
        set_target_properties(
            SDL2::IMAGE
            PROPERTIES
                IMPORTED_LOCATION ${DYNAMIC_LIB_PATH}
                IMPORTED_IMPLIB ${LIB_PATH}
                INTERFACE_INCLUDE_DIRECTORIES ${INCLUDE_DIR}
        )
        add_library(SDL2_image INTERFACE IMPORTED GLOBAL)
        target_link_libraries(SDL2_image INTERFACE SDL2::IMAGE)
    else()  # Linux, MacOSX
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2_image SDL2_image REQUIRED IMPORTED_TARGET)
        add_library(SDL2_image ALIAS PkgConfig::SDL2_image)
    endif()
endif()