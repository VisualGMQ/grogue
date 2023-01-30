cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_policy(SET CMP0135 NEW)

# begin project

project(
    grogue
    VERSION 0.1.0
    LANGUAGES CXX C
)

# fetch SDL under Windows
# for Appveyor CI/CD
if (WIN32)
    include(cmake/fetch_sdl.cmake)
    option(FETCH_SDL "fetch SDL2 SDL2_image SDL2_ttf SDL2_mixer from web" FALSE)
    if (FETCH_SDL)
        FetchSDL()
    endif()
endif()

include(cmake/utility.cmake)

# find SDL
include(cmake/FindSDL2.cmake)
include(cmake/FindSDL2_image.cmake)
include(cmake/FindSDL2_mixer.cmake)
include(cmake/FindSDL2_ttf.cmake)

# some useful utiilities
include(cmake/copydll.cmake)
include(cmake/cppcheck.cmake)
AttachCppCheck()

# build game

set(GAME_NAME ${PROJECT_NAME})
set(LOWLEVEL_NAME lowlevel)

aux_source_directory(src/core CORE_SRC)
aux_source_directory(src/app  APP_SRC)

add_library(${LOWLEVEL_NAME} STATIC ${CORE_SRC} ${APP_SRC})
target_include_directories(${LOWLEVEL_NAME} PUBLIC include)
target_link_libraries(${LOWLEVEL_NAME} PUBLIC SDL2 SDL2_image SDL2_ttf SDL2_mixer)
target_compile_features(${LOWLEVEL_NAME} PUBLIC cxx_std_17)

aux_source_directory(src/game GAME_SRC)

add_executable(${GAME_NAME})
target_sources(${GAME_NAME} PRIVATE ${GAME_SRC})
target_link_libraries(${GAME_NAME} PRIVATE ${LOWLEVEL_NAME})
CopyDLL(${GAME_NAME})

# auto format
option(AUTO_FORMAT "should auto-format before build using clang-format?" ON)
mark_as_advanced(FORCE AUTO_FORMAT)
if (AUTO_FORMAT)
    message("enable auto format")
    add_custom_command(TARGET ${GAME_NAME} PRE_BUILD
                    COMMAND ${CMAKE_COMMAND} -P cmake/format.cmake
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    COMMENT "start format")
endif()

# unittest
option(ENABLE_TEST "enable build unit test" ON)
if (ENABLE_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
