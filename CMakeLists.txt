cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    grogue
    VERSION 0.1.0
    LANGUAGES CXX C
)

# fetch SDL under Windows
# for Appveyor CI/CD
if (WIN32 AND NOT EMSCRIPTEN)
    include(cmake/fetch_sdl.cmake)
    option(FETCH_SDL "fetch SDL2 SDL2_image SDL2_ttf SDL2_mixer from web" FALSE)
    if (FETCH_SDL)
        FetchSDL()
    endif()
endif()

include(cmake/utility.cmake)

# find SDL
if (NOT EMSCRIPTEN)
    include(cmake/FindSDL2_image.cmake)
    include(cmake/FindSDL2_mixer.cmake)
    include(cmake/FindSDL2_ttf.cmake)
    include(cmake/FindSDL2.cmake)
endif()

# compile 3rdlibs
add_subdirectory(3rdlibs/lua)
add_subdirectory(3rdlibs/sol)
add_subdirectory(3rdlibs/expected)

# some useful utiilities
include(cmake/copydll.cmake)
include(cmake/cppcheck.cmake)
AttachCppCheck()

# build game

set(GAME_NAME ${PROJECT_NAME})
set(LOWLEVEL_NAME lowlevel)

aux_source_directory(src/core LOWLEVEL_SRC)
aux_source_directory(src/app  LOWLEVEL_SRC)
aux_source_directory(src/game LOWLEVEL_SRC)

find_program(PYTHON python3)
if (${PYTHON} MATCHES PYTHON-NOTFOUND)
    find_program(PYTHON python)
    if (${PYTHON} MATCHES PYTHON-NOTFOUND)
        message(FATAL_ERROR "you must install python 3.0 and pip 3.0")
    endif()
endif()

# run python parser to generate luabind codes
execute_process(
    COMMAND "${PIP} install" "-r utilities/ecs_parser/requirements.txt"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
execute_process(
    COMMAND ${PYTHON} "${CMAKE_SOURCE_DIR}/utilities/ecs_parser/parser.py"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

aux_source_directory(luabind LOWLEVEL_SRC)


add_library(${LOWLEVEL_NAME} STATIC ${LOWLEVEL_SRC})
target_include_directories(${LOWLEVEL_NAME} PRIVATE luabind)

add_custom_command(
    TARGET ${LOWLEVEL_NAME}
    PRE_BUILD
    COMMAND ${PYTHON} "${CMAKE_SOURCE_DIR}/utilities/ecs_parser/parser.py"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

target_include_directories(${LOWLEVEL_NAME} PUBLIC include)
target_compile_features(${LOWLEVEL_NAME} PUBLIC cxx_std_17)
if (MSVC)
    target_compile_options(${LOWLEVEL_NAME} PUBLIC /bigobj)
endif()

if (NOT ${is_mingw_backend})
    target_precompile_headers(${LOWLEVEL_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/core/pch.hpp)
endif()

# judge platform
if (UNIX)
    target_compile_definitions(${LOWLEVEL_NAME} PUBLIC GROGUE_PLATFORM_UNIX)
elseif (WIN32)
    target_compile_definitions(${LOWLEVEL_NAME} PUBLIC GROGUE_PLATFORM_WIN32)
    target_link_libraries(${LOWLEVEL_NAME} PUBLIC DbgHelp.Lib)
elseif (APPLE)
    target_compile_definitions(${LOWLEVEL_NAME} PUBLIC GROGUE_PLATFORM_APPLE)
elseif (EMSCRIPTEN)
    target_compile_definitions(${LOWLEVEL_NAME} PUBLIC GROGUE_PLATFORM_EMSCRIPTEN)
else()
    target_compile_definitions(${LOWLEVEL_NAME} PUBLIC GROGUE_PLATFORM_UNKNOWN)
endif()

add_executable(${GAME_NAME} ./src/main.cpp)
target_link_libraries(${GAME_NAME} PRIVATE ${LOWLEVEL_NAME})

CopyDLL(${GAME_NAME})

# run parser.py to generate luabind codes
find_program(PIP pip3)
if (PIP-NOTFOUND)
    find_program(PIP pip3)
    if (PIP-NOTFOUND)
        message(FATAL_ERROR "you must install python 3.0 and pip 3.0")
    endif()
endif()

# emscripten special link
if (NOT EMSCRIPTEN)
    target_link_libraries(${LOWLEVEL_NAME} PUBLIC SDL2 SDL2_image SDL2_ttf SDL2_mixer lua sol2 expected)
    CopyDefScript(${GAME_NAME})
else()
    message("-- build for wasm")
    set(USE_SDL "-sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -sUSE_SDL_MIXER=2 -sSDL2_IMAGE_FORMATS=\"[\"png\", \"jpg\", \"bmp\"]\"" )
    set(PRELOAD "--preload-file resources --use-preload-plugins")
    set(OTHER_FLAGS "-s WASM=1 -s ALLOW_MEMORY_GROWTH=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_SDL}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_SDL}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_SDL} ${PRELOAD} ${OTHER_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
    target_link_libraries(${LOWLEVEL_NAME} PUBLIC lua sol2 expected)
    CopyResForWASM(${GAME_NAME})
endif()

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


# doc generate
option(DOC_GENERATE "generate doc" ON)

if (DOC_GENERATE)
    find_program(DOXYGEN_PROGRAM doxygen)
    if (DOXYGEN_PROGRAM)
        message("found doxygen, enable build doc")
        execute_process(
            COMMAND doxygen
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_QUIET)
    endif()
endif()

# utilities
option(ENABLE_UTILITIES "enable build utilities" ON)
if (ENABLE_UTILITIES AND NOT EMSCRIPTEN)
    add_subdirectory(utilities)
endif()

# unittest
option(ENABLE_TEST "enable build unit test" ON)
if (ENABLE_TEST AND NOT EMSCRIPTEN)
    enable_testing()
    add_subdirectory(test)
endif()
