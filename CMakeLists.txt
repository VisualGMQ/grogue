cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

# find SDL
include(cmake/FindSDL2.cmake)
include(cmake/FindSDL2_image.cmake)
include(cmake/FindSDL2_mixer.cmake)
include(cmake/FindSDL2_ttf.cmake)

# some useful utiilities
include(cmake/copydll.cmake)
include(cmake/cppcheck.cmake)
include(cmake/format.cmake)
AttachCppCheck()

# build game

set(GAME_NAME ${PROJECT_NAME})

aux_source_directory(src/core CORE_SRC)
aux_source_directory(src/app  APP_SRC)
aux_source_directory(src/game GAME_SRC)

add_executable(${GAME_NAME})
target_sources(${GAME_NAME} PRIVATE
                ${CORE_SRC}
                ${APP_SRC}
                ${GAME_SRC}
)
target_include_directories(${GAME_NAME} PRIVATE include)
target_link_libraries(${GAME_NAME} PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)
target_compile_features(${GAME_NAME} PRIVATE cxx_std_17)
CopyDLL(${GAME_NAME})

# auto format

file(GLOB_RECURSE FILES
     ${CMAKE_SOURCE_DIR}/include/**.hpp
     ${CMAKE_SOURCE_DIR}/src/**.cpp
     ${CMAKE_SOURCE_DIR}/include/**.c
     ${CMAKE_SOURCE_DIR}/src/**.h)
DoFormats(FILES)