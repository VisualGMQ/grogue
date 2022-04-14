cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(grogue
        VERSION 0.1.0
        LANGUAGES CXX)

################################
# Find Depedencies
################################
include(cmake/FindSDL2.cmake)
include(cmake/FindSDL2_image.cmake)
include(cmake/FindSDL2_ttf.cmake)

set(GROGUE_CORE_LIB_NAME core)

aux_source_directory(src/core CORE_SRC)

set(GROGUE_CORE_LIB_NAME core)
add_library(${GROGUE_CORE_LIB_NAME} SHARED ${CORE_SRC})
target_link_libraries(
    ${GROGUE_CORE_LIB_NAME}
    PUBLIC SDL2::SDL2 SDL2::SDL2_image SDL2::SDL2_ttf
)
target_include_directories(
    ${GROGUE_CORE_LIB_NAME}
    PUBLIC include 3rdlib
)
target_compile_features(${GROGUE_CORE_LIB_NAME} PUBLIC cxx_std_17)

target_precompile_headers(${GROGUE_CORE_LIB_NAME} PUBLIC include/core/pch.hpp)

aux_source_directory(src/game GAME_SRC)
add_executable(${PROJECT_NAME} ${GAME_SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC ${GROGUE_CORE_LIB_NAME})
