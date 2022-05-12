option(BUILD_TOOL "enable build tool" ON)

if (BUILD_TOOL)
    add_subdirectory(tool)
endif()


