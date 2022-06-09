find_package(Lua QUIET)

if (NOT LUA_FOUND)
    if (WIN32)
        # TODO find lua under windows
    else() # under MacOS and Linux
        message(STATUS "cmake can't find lua, use pkg-config")
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(Lua lua REQUIRED IMPORTED_TARGET)
        add_library(Lua ALIAS PkgConfig::Lua)
    endif()
else()
    message(STATUS "found lua")
    add_library(Lua INTERFACE)
    target_include_directories(Lua INTERFACE ${LUA_INCLUDE_DIR})
    target_link_libraries(Lua INTERFACE ${LUA_LIBRARIES})
endif()
