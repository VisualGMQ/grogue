cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(
  grogue
  VERSION 0.1.0
  LANGUAGES CXX
)

################################
# Find Depedencies
################################
include(cmake/FindSDL2.cmake)
include(cmake/FindSDL2_image.cmake)
include(cmake/FindSDL2_ttf.cmake)

################################
# Compile
################################
set(GROGUE_CORE_LIB_NAME core)

# Compile Core
aux_source_directory(src/core CORE_SRC)
file(GLOB_RECURSE CORE_HEADER_FILES "include/core/*.hpp" "include/core/*.h")

source_group("Header Files" FILES ${CORE_HEADER_FILES})
source_group("Source Files" FILES ${CORE_SRC})

set(GROGUE_CORE_LIB_NAME core)
add_library(${GROGUE_CORE_LIB_NAME} SHARED ${CORE_SRC} ${CORE_HEADER_FILES})
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
target_compile_options(${GROGUE_CORE_LIB_NAME} PUBLIC -Wall -Wformat -Wextra -pedantic)

# Compile Game
aux_source_directory(src/game GAME_SRC)
file(GLOB_RECURSE GAME_HEADER_FILES "include/game/*.hpp" "include/game/*.h")

source_group("Header Files" FILES ${GAME_HEADER_FILES})
source_group("Source Files" FILES ${GAME_SRC})

add_executable(${PROJECT_NAME} ${GAME_SRC} ${GAME_HEADER_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${GROGUE_CORE_LIB_NAME})

set_target_properties(${PROJECT_NAME} ${GROGUE_CORE_LIB_NAME}
                      PROPERTIES 
                        FOLDER "Game")

################################
# Set other folders
################################
# Set assets Folder
file(GLOB_RECURSE GAME_ASSETS "${CMAKE_SOURCE_DIR}/assets/*")
add_library(assets INTERFACE ${GAME_ASSETS})

# Set others Folder
file(GLOB GAME_OTHER_ASSETS "*.md")
add_library(others INTERFACE ${GAME_OTHER_ASSETS})

# Set cmake Folder
file(GLOB_RECURSE CMAKE_FILES "cmake/*.cmake")
add_library(cmake INTERFACE ${CMAKE_FILES})

# Set 3rdlib Folder
file(GLOB_RECURSE 3RDLIB_FILES "3rdlib/*.hpp" "3rdlib/*.h++")
add_library(3rdlib INTERFACE ${3RDLIB_FILES})

################################
# add test
################################
enable_testing()

add_subdirectory(test)
