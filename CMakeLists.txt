cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Grogue
        VERSION 0.1.0
        LANGUAGES CXX)

################################
# Find Depedencies
################################
include(cmake/FindSDL2.cmake)
include(cmake/FindSDL2_image.cmake)
include(cmake/FindSDL2_ttf.cmake)
include(cmake/FindSpdLog.cmake)

set(GROGUE_CORE_LIB_NAME core)
