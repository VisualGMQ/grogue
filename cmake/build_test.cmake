option(BUILD_UNITTEST "enable build unittest" OFF)

if (BUILD_UNITTEST)
    enable_testing()
    add_subdirectory(test)
endif()


